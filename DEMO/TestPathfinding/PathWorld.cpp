
#include "ByteBuffer.h"
#include "File.h"
#include "PathWorld.h"

void PathWorld::initialize(BT::File& file)
{
    BT_PRE_CONDITION(isNull());     // DEV Issue: initialize() already called !

    // Read graph
    loadGraph(file);

    // Parse each node and register
    auto walkTerrains = boost::vertices(_worldGraph);
    auto walkTerrainID = walkTerrains.first;
    //   Parse all
    while(walkTerrainID != walkTerrains.second)
    {
        BT_ASSERT(*walkTerrainID < num_vertices(_worldGraph));
        const auto& walkTerrain = _worldGraph[*walkTerrainID];

        // Compute box and set inside rTree
        BT_ASSERT(boost::geometry::is_valid(walkTerrain._polygon));  // DEV Issue: must be valid before !

        // Compute bounding box
        BoostBox box;
        boost::geometry::envelope(walkTerrain._polygon, box);

        // Add to rTree
        _quadTree.insert(AreaBox(box, *walkTerrainID));

        ++walkTerrainID;
    }

    BT_POST_CONDITION(!isNull());   // DEV Issue: MapFile is empty !
}

bool PathWorld::isNull() const
{
    return _quadTree.empty() && num_vertices(_worldGraph) == 0;
}

void PathWorld::release()
{
    BT_PRE_CONDITION(!isNull());    // DEV Issue: Need to call initialize before

    _quadTree.clear();

    // Brutal replace
    TerrainGraph empty;
    _worldGraph = std::move(empty);

    BT_POST_CONDITION(isNull());                        // DEV Issue: Clean operation failed ?
}

void PathWorld::loadGraph(BT::File& file)
{
    BT_PRE_CONDITION(file.isOpened());
    BT_PRE_CONDITION(num_vertices(_worldGraph) == 0); // DEV Issue: load() already called !

    //Read Buffer
    BT::ByteBuffer buffer;
    file.read(buffer);

    // Read points
    size_t nbVertices = 0;
    buffer >> nbVertices;
    _points.resize(nbVertices);
    for(auto& point : _points)
    {
        float x, y;
        buffer >> x >> y;
        point = BoostPoint(x, y);
    }

    // Read number of nodes
    size_t nbNode = 0;
    buffer >> nbNode;
    BT_ASSERT(nbNode > 0);

#ifdef LINK
    _link.resize(nbNode);
#endif
    // Parse each node and create into graph
    for(size_t nodeID=0; nodeID < nbNode; ++nodeID)
    {
        WalkTerrain node;
        size_t terrainID=0;
        std::vector<BT::uint32> indices;
        float x, y;
        buffer >> x >> y >> terrainID >> node._subgraphID >> indices;
        // Terrain extract
        buffer >> node._terrain._type >> node._terrain._height;

        node._centroid.set<0>(x);
        node._centroid.set<1>(y);

        // Create polygon from indices
        for(const auto& index : indices)
        {
            boost::geometry::append(node._polygon, _points[index]);
        }
#ifndef NDEBUG
#ifdef  GEOMETRY_VALIDATION
        std::string reason;
        BT_ASSERT(bg::is_valid(node._polygon, reason)); // DEV Issue: Corrupt data, please fix input data !
#endif
#endif

#ifdef LINK
        _link[nodeID] = boost::add_vertex(node, _worldGraph);
#else
        boost::add_vertex(node, _worldGraph);
#endif
    }

    // Parse each edge
    size_t nbEdges = 0;
    buffer >> nbEdges;
    BT_ASSERT(nbEdges > 0);

    for(size_t edgeID=0; edgeID < nbEdges; ++edgeID)
    {
        BT::uint32 nodeID0;
        BT::uint32 nodeID1;
        float distance;
        buffer >> nodeID0 >> nodeID1 >> distance;

#ifdef LINK
        const WalkTerrainID wtNodeId0 = _link[nodeID0];
        const WalkTerrainID wtNodeId1 = _link[nodeID1];
        BT_ASSERT(0 <= nodeID0 && nodeID0 < num_vertices(_worldGraph)); // DEV Issue: Check valid data !
        BT_ASSERT(0 <= nodeID1 && nodeID1 < num_vertices(_worldGraph)); // DEV Issue: Check valid data !
        
#else
        const WalkTerrainID wtNodeId0 = static_cast<WalkTerrainID>(nodeID0);
        const WalkTerrainID wtNodeId1 = static_cast<WalkTerrainID>(nodeID1);
#endif
        WalkTerrain& node0 = _worldGraph[wtNodeId0];
        WalkTerrain& node1 = _worldGraph[wtNodeId1];

        // Add edges
        EdgeInfo info0(distance, &node0, &node1);
        EdgeInfo info1(distance, &node1, &node0);

        boost::add_edge(wtNodeId0, wtNodeId1, info0, _worldGraph);
        boost::add_edge(wtNodeId1, wtNodeId0, info1, _worldGraph);

        BT_ASSERT((node0._terrain._type == Ocean && node1._terrain._type == Ocean) ||
                  (node0._terrain._type != Ocean && node1._terrain._type != Ocean)); // DEV Issue: Check valid data !

        BT_ASSERT(0.0f < distance);                               // DEV Issue: Check valid data !
    }

    BT_POST_CONDITION(num_vertices(_worldGraph) > 0); // DEV Issue: Valid file !
}

PathWorld::AreaBox PathWorld::findNearest(const BoostPoint& localization) const
{
    // Read all data under points
    std::vector<AreaBox> result;
    _quadTree.query(bgi::intersects(localization), std::back_inserter(result));

    // Check first where polygon is inside
    for(const auto &r: result)
    {
        if(bg::within(localization, getWalkTerrain(r)._polygon))
        {
            return r;
        }
    }
    throw std::exception(u8"BadPolygon");
}

float PathWorld::updateWeight(const Agent& agent, const EdgeInfo& edgeInfo) const
{
    const Agent::Navigation& navigation = agent.getNavigation();

    const auto& fromTerrain = edgeInfo._from->_terrain;
    const auto& toTerrain   = edgeInfo._to->_terrain;
    BT_PRE_CONDITION(navigation._speed.at(fromTerrain._type) >= 0.0f);
    BT_PRE_CONDITION(navigation._speed.at(toTerrain._type)   >= 0.0f);

    const float mean = (navigation._speed.at(fromTerrain._type) + navigation._speed.at(toTerrain._type)) * 0.5f;
    const float diff = BT::Maths::clamp((1000.0f + toTerrain._height - fromTerrain._height) / 1000.0f, 0.5f, 2.0f);

    //Compute weight in hour
    return edgeInfo._distance / mean * diff;
}

float PathWorld::updateSpeedWeight(const EdgeInfo& edgeInfo) const
{
    return edgeInfo._distance;
}

bool PathWorld::computePath(const Agent& agent, const WalkTerrainID from, const WalkTerrainID to, std::vector<BoostPoint>& pathWay, bool speedMode)
{
    BT_PRE_CONDITION(!isNull());                        // DEV Issue: No data into file !
    BT_PRE_CONDITION(num_vertices(_worldGraph) > 0);    // DEV Issue: Valid file !

    BT_PRE_CONDITION(from < num_vertices(_worldGraph)); // DEV Issue: Invalid input !
    BT_PRE_CONDITION(to   < num_vertices(_worldGraph)); // DEV Issue: Invalid input !
    BT_PRE_CONDITION(pathWay.empty());                  // DEV Issue: Need an empty result!

    bool valid = false;
    const WalkTerrain& goal  = _worldGraph[to];
    const WalkTerrain& start = _worldGraph[from];
    BT_ASSERT(start._subgraphID != 0);
    BT_ASSERT(goal._subgraphID != 0);

    // Parse only if inside same sub-graph (else never solution so don't waste time)
    if(start._subgraphID == goal._subgraphID)
    {
        std::vector<WalkTerrainID> path(num_vertices(_worldGraph));
        try
        {
            if(speedMode)
            {
                // Compute agent weight
                const auto weightMap = make_transform_value_property_map([&](EdgeInfo& e){ return updateSpeedWeight(e); }, get(boost::edge_bundle, _worldGraph));
                // call astar named parameter interface
                boost::astar_search_tree( _worldGraph, from, TerrainHeuristic(goal, _worldGraph),
                                          boost::predecessor_map(path.data()).visitor(GoalVisitor(to)).weight_map(weightMap)
                                        );
            }
            else
            {
                // Compute agent weight
                const auto weightMap = make_transform_value_property_map([&](EdgeInfo& e){ return updateWeight(agent, e); }, get(boost::edge_bundle, _worldGraph));
                // call astar named parameter interface
                boost::astar_search_tree(_worldGraph, from, TerrainHeuristic(goal, _worldGraph),
                                    boost::predecessor_map(path.data()).visitor(GoalVisitor(to)).weight_map(weightMap)
                );
            }

            // NEVER Happens !!!
            throw std::exception(u8"PathWorld_Corruption");
        }
        catch(GoalException&)
        {
            // Extract
            pathWay.reserve(path.size());
            for(WalkTerrainID v = to;; v = path[v])
            {
                pathWay.emplace_back(_worldGraph[v]._centroid);
                if(path[v] == v)
                    break;
            }
            valid = true;
        }
    }
    return valid;
}

const PathWorld::WalkTerrain& PathWorld::getWalkTerrain(const AreaBox& area) const
{
    BT_PRE_CONDITION(!isNull());                                // DEV Issue: No data into file !
    BT_PRE_CONDITION(num_vertices(_worldGraph) > 0);            // DEV Issue: Valid file !

    BT_PRE_CONDITION(area.second < num_vertices(_worldGraph));  // DEV Issue: Check in range !

    return _worldGraph[area.second];
}

const PathWorld::WalkTerrain& PathWorld::getWalkTerrain(const WalkTerrainID& id) const
{
    BT_PRE_CONDITION(!isNull());                         // DEV Issue: No data into file !
    BT_PRE_CONDITION(num_vertices(_worldGraph) > 0);   // DEV Issue: Valid file !
    BT_PRE_CONDITION(id < num_vertices(_worldGraph));  // DEV Issue: Check in range !

    return _worldGraph[id];
}

