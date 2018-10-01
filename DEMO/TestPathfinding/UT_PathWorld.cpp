#include "gtest/gtest.h"

#include "File.h"
#include "PathWorld.h"

//#define DEBUG_PRINT

class PathWorldTest : public ::testing::Test
{
    public:
        static void SetUpTestCase()
        {}

        static void TearDownTestCase()
        {}

        void read(const BT::StringOS& fileMap, PathWorld& path)
        {
            // Build data
            BT::File file(fileMap);
            ASSERT_NO_THROW(file.open());
            ASSERT_NO_THROW(path.initialize(file));
            ASSERT_NO_THROW(file.close());
        }

        void computePath(PathWorld& path, const size_t start, const size_t nbQuery, bool speedMode = false)
        {
            Agent newAgent({ 80.0f, 60.0f, 50.0f, 30.0f, 20.0f, 40.0f, 40.0f });

            BT::uint32 minPath = std::numeric_limits<BT::uint32>::max();
            BT::uint32 maxPath = 0;
            BT::uint32 nbValid = 0;

            std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
            int minElapsed = std::numeric_limits<int>::max();
            int maxElapsed = 0;

            const PathWorld::WalkTerrainID n0 = static_cast<PathWorld::WalkTerrainID>(start);

            // Check some path
            for(BT::uint32 query = 0; query < nbQuery; ++query)
            {
                std::vector<BoostPoint> pathWay;
                bool valid = false;

                const PathWorld::WalkTerrainID n1 = static_cast<PathWorld::WalkTerrainID>(start + query);
                
                startTime = std::chrono::system_clock::now();
                ASSERT_NO_THROW(valid = path.computePath(newAgent, n0, n1, pathWay, speedMode));
                endTime = std::chrono::system_clock::now();
                if(valid)
                {
                    ++nbValid;
                    minPath = std::min(minPath, static_cast<BT::uint32>(pathWay.size()));
                    maxPath = std::max(maxPath, static_cast<BT::uint32>(pathWay.size()));
                    int elapse = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
                    maxElapsed = std::max<int>( maxElapsed, elapse );
                    minElapsed = std::min<int>( minElapsed, elapse );
                    
#ifdef DEBUG_PRINT
                    std::cout << start + query << "/" << start + nbQuery
                              << " = " << static_cast<BT::uint32>(pathWay.size());
                    
                    if((nbValid % 5) == 0)
                        std::cout << std::endl;
                    else
                        std::cout << "\t";
#endif
                }
            }
            const PathWorld::WalkTerrain& startNode = path.getWalkTerrain( n0 );
            const std::array<BT::String, NbType> _names = { "Ocean", "Swamp", "Forest", "Clay", "Linestone", "Sand", "Rock" };

            std::cout << std::endl << "Statistics:" << std::endl;
            std::cout << "  Start node: "<< _names[startNode._terrain._type] << " H= " << startNode._terrain._height << " SubGraph= " << startNode._subgraphID << std::endl;
            std::cout << "  nbValid: " << nbValid << "/" << nbQuery << std::endl;
            std::cout << "  min / max: " << minPath << "/" << maxPath << std::endl;
            std::cout << "  min time for one path: " << minElapsed << " ms" << std::endl;
            std::cout << "  max time for one path: " << maxElapsed << " ms" << std::endl;
        }
};

TEST_F(PathWorldTest, EASY_computePath)
{
    PathWorld path;
    read(L"map/Map_easy.map", path);

    computePath(path, 10, 20);
    computePath(path, 2,  20);
    computePath(path, 9,  20);

    ASSERT_NO_THROW(path.release());
}

TEST_F(PathWorldTest, SMALL_computePath)
{
    PathWorld path;
    read(L"map/Map_small.map", path);

    computePath(path, 30, 160);
    computePath(path, 2,  160);
    computePath(path, 60, 160);

    ASSERT_NO_THROW(path.release());
}

TEST_F(PathWorldTest, COMPLEX_computePath)
{
    PathWorld path;
    read(L"map/Map_complex.map", path);

    // World See
    computePath(path, 550,   15000);

    // Main continent
    computePath(path, 1050,  1500);
    computePath(path, 3251,  1000);

    // On Island
    computePath(path, 3067, 15000);

    ASSERT_NO_THROW(path.release());
}