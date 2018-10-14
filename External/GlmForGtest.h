#pragma once

///------------------------------------------------------------------------------------------------
namespace testing
{

/// GLM for GoogleTest
inline AssertionResult AssertVec3Helper( const char* e1,
                                  const char* e2,
                                  const char* prec,
                                  const glm::vec3& v1,
                                  const glm::vec3& v2,
                                  const float precision )
{
    if( fabs( v1.x-v2.x ) < precision
     && fabs( v1.y-v2.y ) < precision
     && fabs( v1.z-v2.z ) < precision )
        return AssertionSuccess();

    return AssertionFailure() << "error: The difference between("
        << e1 << ", "
        << e2 << "), where"
        << "\n" << e1 << " evaluates to " << "glm::vec3( " << v1.x << ", " << v1.y << ", " << v1.z << " )"
        << "\n" << e2 << " evaluates to " << "glm::vec3( " << v2.x << ", " << v2.y << ", " << v2.z << " )"
        << "\n which exceeds " << prec << " evaluates to " << precision;
}

inline AssertionResult AssertVec4Helper( const char* e1,
                                  const char* e2,
                                  const char* prec,
                                  const glm::vec4& v1,
                                  const glm::vec4& v2,
                                  const float precision )
{
    if( fabs( v1.x-v2.x ) < precision
     && fabs( v1.y-v2.y ) < precision
     && fabs( v1.z-v2.z ) < precision
     && fabs( v1.w-v2.w ) < precision)
        return AssertionSuccess();

    return AssertionFailure() << "error: The difference between("
        << e1 << ", "
        << e2 << "), where"
        << "\n" << e1 << " evaluates to " << "glm::vec4( " << v1.x << ", " << v1.y << ", " << v1.z << ", " << v1.w << " )"
        << "\n" << e2 << " evaluates to " << "glm::vec4( " << v2.x << ", " << v2.y << ", " << v2.z << ", " << v2.w << " )"
        << "\n which exceeds " << prec << " evaluates to " << precision;
}

inline AssertionResult AssertMat4Helper( const char* e1,
                                  const char* e2,
                                  const char* prec,
                                  const glm::mat4& m1,
                                  const glm::mat4& m2,
                                  const float precision )
{
    bool valid = true;
    for( int rawID=0; rawID < 4; ++rawID )
    {
        for( int columnID=0; columnID < 4; ++columnID )
        {
            valid &= fabs( m1[rawID][columnID]-m2[rawID][columnID] ) < precision;
        }
    }

    if( valid )
        return AssertionSuccess();

    return AssertionFailure() << "error: The difference between("
        << e1 << ", "
        << e2 << "), where"
        << "\n" << e1 << " evaluates to\n" //<< print( v1 ).c_str()
        << "\n" << e2 << " evaluates to\n" //<< print( v2 ).c_str()
        << "\n which exceeds " << prec << " evaluates to " << precision;
}

#ifndef EXPECT_VEC3_NEAR
#define EXPECT_VEC3_NEAR(vectorRef, vectorCmp, precision) \
            GTEST_ASSERT_(testing::AssertVec3Helper(#vectorRef, #vectorCmp, #precision, vectorRef, vectorCmp, precision), GTEST_NONFATAL_FAILURE_)
#endif

#ifndef EXPECT_VEC4_NEAR
#define EXPECT_VEC4_NEAR(vectorRef, vectorCmp, precision) \
            GTEST_ASSERT_(testing::AssertVec4Helper(#vectorRef, #vectorCmp, #precision, vectorRef, vectorCmp, precision), GTEST_NONFATAL_FAILURE_)
#endif

#ifndef EXPECT_MAT4_NEAR
#define EXPECT_MAT4_NEAR(matrixRef, matrixCmp, precision) \
            GTEST_ASSERT_(testing::AssertMat4Helper(#matrixRef, #matrixCmp, #precision, matrixRef, matrixCmp, precision), GTEST_NONFATAL_FAILURE_)
#endif
}