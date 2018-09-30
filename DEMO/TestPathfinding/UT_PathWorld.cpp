#include "gtest/gtest.h"

#include "File.h"
#include "PathWorld.h"

#define DEBUG_PRINT

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

            // Check some path
            for(BT::uint32 query = 0; query < nbQuery; ++query)
            {
                std::vector<BoostPoint> pathWay;
                bool valid = false;

                const PathWorld::WalkTerrainID n0 = static_cast<PathWorld::WalkTerrainID>(start);
                const PathWorld::WalkTerrainID n1 = static_cast<PathWorld::WalkTerrainID>(start + query);
                ASSERT_NO_THROW(valid = path.computePath(newAgent, n0, n1, pathWay, speedMode));

                if(valid)
                {
                    ++nbValid;
                    minPath = std::min(minPath, static_cast<BT::uint32>(pathWay.size()));
                    maxPath = std::max(maxPath, static_cast<BT::uint32>(pathWay.size()));
                    
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

            std::cout << std::endl << "Statistics:" << std::endl;
            std::cout << "  nbValid: " << nbValid << "/" << nbQuery << std::endl;
            std::cout << "  min / max: " << minPath << "/" << maxPath << std::endl;
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

    computePath(path, 550,   15000);
    computePath(path, 1050,  1500);
    computePath(path, 3050,  1000);

    ASSERT_NO_THROW(path.release());
}
/*
TEST_F(PathWorldTest, EASY_computePath_SPEED)
{
    PathWorld path;
    read(L"map/Map_easy.map", path);

    computePath(path, 10, 20, true);
    computePath(path, 2,  20, true);
    computePath(path, 9,  20, true);

    ASSERT_NO_THROW(path.release());
}

TEST_F(PathWorldTest, SMALL_computePath_SPEED)
{
    PathWorld path;
    read(L"map/Map_small.map", path);

    computePath(path, 30, 160, true);
    computePath(path, 2,  160, true);
    computePath(path, 60, 160, true);

    ASSERT_NO_THROW(path.release());
}

TEST_F(PathWorldTest, COMPLEX_computePath_SPEED)
{
    PathWorld path;
    read(L"map/Map_complex.map", path);

    computePath(path, 550,  15000, true);
    computePath(path, 1005, 15000, true);
    computePath(path, 3050, 11000, true);

    ASSERT_NO_THROW(path.release());
}
*/