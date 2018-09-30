#pragma once

#include "Define.h"

//#define LINK

/// Terrain type
enum Type
{
    Ocean,
    Swamp,
    Forest,
    Clay,
    Linestone,
    Sand,
    Rock,
    NbType
};

class Agent
{
    BT_NOCOPY_NOMOVE(Agent);
    public:
        //----------------------------------------------------------------------------
        /// \brief Navigation data. All speed by kind of terrain.
        struct Navigation
        {
            std::array<float, NbType> _speed; ///< Speed
        };

        /// Constructor
        Agent(const Navigation& navigation):
        _navigation(navigation)
        {}

        /// Destructor
        ~Agent() = default;

        //------------------------------------------------------------------------
        /// \brief  Get navigation information of agent (speed by kind of terrain).
        /// 
        /// \returns Navigation information.
        const Navigation& getNavigation() const
        {
            return _navigation;
        }

    private:
        Navigation  _navigation;        ///< Navigation information (speed).
};

class Terrain
{
    public:
        Terrain():
        _type(NbType), _height(0.0f)
        {}

        void initialize(Type type, float height)
        {
            _type   = type;
            _height = height;
        }

        Type    _type;
        float   _height;
};

//----------------------------------------------------------------------------
/// \brief Allow to manage movement in world.
///
/// \see   
class PathWorld final
{
    BT_NOCOPY_NOMOVE(PathWorld);

    public:
        static const BT::int32  _version    = 010001; ///< File version XX Major XXXX Minor

        struct EdgeInfo;

        //----------------------------------------------------------------------------
        /// \brief Subdivision of Terrain to navigate properly.
        struct WalkTerrain
        {
            Terrain         _terrain;       ///< Terrain info.
            BoostPoint      _centroid;      ///< Center of polygon in 2D world coordinate.
            BoostPolygon    _polygon;       ///< Form of model.
            BT::uint16      _subgraphID;    ///< Connectivity between node.

            /// Constructor
            WalkTerrain() :
            _subgraphID(0)
            {}

            /// Destructor
            ~WalkTerrain()
            {}
        };

        using TerrainGraph      = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
            WalkTerrain,
            EdgeInfo>;   ///< Graph

        using WalkTerrainID     = boost::graph_traits<TerrainGraph>::vertex_descriptor;
        using TerrainEdgeID     = boost::graph_traits<TerrainGraph>::edge_descriptor;

        struct EdgeInfo
        {
            // Constructor: invalid by default
            EdgeInfo(const float distance = -1.0f, const WalkTerrain* from=nullptr, const WalkTerrain* to=nullptr) :
                _from(from), _to(to), _distance(distance)
            {}

            const WalkTerrain* _from;   ///< [LINK] Link to WorldGraph node. WARNING: No destruction.
            const WalkTerrain* _to;     ///< [LINK] Link to WorldGraph node. WARNING: No destruction.
            float        _distance;     ///< Distance between from/to centroid.
        };

        using AreaBox           = std::pair<BoostBox, WalkTerrainID>;

        using QuadTree          = bgi::rtree< AreaBox, bgi::rstar<16> >;
        using CostType          = float;

        // euclidean distance heuristic
        class TerrainHeuristic : public boost::astar_heuristic<TerrainGraph, CostType>
        {
            public:
            TerrainHeuristic(const WalkTerrain& goal, const TerrainGraph& worldGraph) :
                _goal(goal), _worldGraph(worldGraph)
            {}

            CostType operator()(WalkTerrainID current)
            {
                const WalkTerrain& node = _worldGraph[current];
                // Compute dot = length * length
                return bg::dot_product(node._centroid, _goal._centroid);
            }

            private:
            const WalkTerrain&  _goal;
            const TerrainGraph& _worldGraph;
        };

        struct GoalException{}; // exception for termination

        // visitor that terminates when we find the goal
        class GoalVisitor : public boost::default_astar_visitor
        {
            public:
            GoalVisitor(WalkTerrainID goal) :
                _goal(goal)
            {}

            template <class Graph>
            void examine_vertex(WalkTerrainID u, Graph& /*g*/)
            {
                if(u == _goal)
                    throw GoalException();
            }

            private:
            WalkTerrainID _goal;
        };

        /// Constructor
        PathWorld()
        {
            BT_PRE_CONDITION(isNull());
        }

        /// Destructor
        ~PathWorld()
        {
            BT_POST_CONDITION(isNull());
        }

        void initialize(BT::File& file);

        //------------------------------------------------------------------------
        /// \brief  Check if QuadTree is null.
        /// 
        /// \returns True if null, false otherwise.
        bool isNull() const;

        //------------------------------------------------------------------------
        /// \brief  Release all data.
        /// 
        void release();

        //------------------------------------------------------------------------
        /// \brief  
        /// 
        /// \param[in]  localization: where to search 
        /// \returns List of nearest elements 
        AreaBox findNearest(const BoostPoint& localization) const;

        //------------------------------------------------------------------------
        /// \brief  Get associated data from Area to WalkTerrain
        /// 
        /// \param[in] area: result of findNearest and ID to find WalkTerrain.
        /// \returns Associated WalkTerrain.
        const WalkTerrain& getWalkTerrain(const AreaBox& area) const;
        const WalkTerrain& getWalkTerrain(const WalkTerrainID& id) const;

        bool computePath(const Agent& agent, const WalkTerrainID from, const WalkTerrainID to, std::vector<BoostPoint>& pathWay, bool speedMode = false);

        float updateWeight(const Agent& agent, const EdgeInfo& edgeInfo) const;
        float updateSpeedWeight(const EdgeInfo& edgeInfo) const;

#ifdef LINK        
        WalkTerrainID convert(const size_t& id) const
        {
            BT_ASSERT(id < _link.size());
            return _link[id];
        }

        std::vector<WalkTerrainID>  _link;
#endif
    private:
        void loadGraph(BT::File& file);

        QuadTree                _quadTree;      ///< Speed data to get where agent is in world.
        TerrainGraph            _worldGraph;    ///< All connexion in world (road, terrain).

        std::vector<BoostPoint>     _points;        ///< [OWNERSHIP]
};
