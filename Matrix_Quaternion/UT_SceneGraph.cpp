///------------------------------------------------------------------------------------------------
/// All demonstrations and codes about http://mouca.fr/wordpress/2018/05/31/matrice-quaternion-et-graphe-de-scene-partie-3/
///
/// Author: Rominitch
///------------------------------------------------------------------------------------------------
#include "Dependancies.h"

#include "Common/Transformation.h"

///------------------------------------------------------------------------------------------------
/// Test: all scene graph data with orientation system.
class SceneGraphTransformation
{
public:
    SceneGraphTransformation() :
        P0( glm::vec3( 0.0f, 2.0f/9.0f, 0.0f ), glm::vec3( 2.0f/3.0f ) ),
        P1( glm::vec3( 2.0f/3.0f, 4.0f/3.0f, 0.0f ) ),
        T0( glm::vec3( -5.0f/3.0f, -2.0f/3.0f, 0.0f ), glm::vec3( 2.0f ) ),
        T1( glm::vec3( 10.0f/3.0f, 1.0f/3.0f, 0.0f ) ),
        V1(),
        C0( glm::vec3( -1.0f, 1.0f/3.0f, 0.0f ) )
    {}

    Transformation P0, P1, T0, T1, V1, C0;
};

TEST( SceneGraphTransformation, computeModel )
{
    const float precision = 1e-5f;
    const SceneGraphTransformation scene;

    // Compute composed orientation and apply to C.
    const glm::vec3 C( 0.0f, 1.0f/3.0f, 0.0f );
    const Transformation model = scene.V1 * (scene.T1 * (scene.P1 * scene.P0));
    const glm::vec3 CinW = model * C;
    EXPECT_VEC3_NEAR( glm::vec3( 4.0f, 19.0f/9.0f, 0.0f ), CinW, precision );

    // All step with each result
    const glm::vec3 C0 = (scene.P0 * C);
    EXPECT_VEC3_NEAR( glm::vec3( 0.0f, 4.0f/9.0f, 0.0f ), C0, precision );
    const glm::vec3 C1 = (scene.P1 * scene.P0) * C;
    EXPECT_VEC3_NEAR( glm::vec3( 2.0f/3.0f, 16.0f/9.0f, 0.0f ), C1, precision );
    const glm::vec3 C2 = (scene.T1 * (scene.P1 * scene.P0)) * C;
    EXPECT_VEC3_NEAR( glm::vec3( 4.0f, 19.0f/9.0f, 0.0f ), C2, precision );

    // Compute inverse to retrieve C
    Transformation modelInv( model );
    modelInv.inverse();
    EXPECT_VEC3_NEAR( C, modelInv * CinW, precision );

    // Other example with a point on train
    const glm::vec3 A( 4.0f/3.0f, 1.0f, 0.0f );
    const glm::vec3 A0  = (scene.T0 * A);
    EXPECT_VEC3_NEAR( glm::vec3( 1.0f, 4.0f/3.0f, 0.0f ), A0, precision );
}

TEST( SceneGraphTransformation, computeCAfterMoveT1 )
{
    const float precision = 1e-5f;
    SceneGraphTransformation scene;

    // Move T1 to example position
    scene.T1 = Transformation( glm::vec3( 7.0f/3.0f, 1.0f/3.0f, 0.0f ) );

    const glm::vec3 C( 0.0f, 1.0f/3.0f, 0.0f );
    // Compose model
    const Transformation model = scene.V1 * (scene.T1 * (scene.P1 * scene.P0));
    // Apply Model-view on C
    const glm::vec3 CinW = model * C;
    EXPECT_VEC3_NEAR( glm::vec3( 3.0f, 19.0f/9.0f, 0.0f ), CinW, precision );
}

TEST( SceneGraphTransformation, computeModelView )
{
    const float precision = 1e-5f;
    SceneGraphTransformation scene;
    
    // Move T1 to example position
    scene.T1 = Transformation( glm::vec3( 7.0f/3.0f, 1.0f/3.0f, 0.0f ) );

    const glm::vec3 C( 0.0f, 1.0f/3.0f, 0.0f );
    // Compose model
    const Transformation model = scene.V1 * (scene.T1 * (scene.P1 * scene.P0));
    // Compose view
    const Transformation view  = (Transformation::inverse(scene.C0) * (Transformation::inverse(scene.P1) * (Transformation::inverse(scene.T1) * Transformation::inverse(scene.V1))));
    // Apply Model-view on C
    const glm::vec3 CinCamera = view * (model * C);
    // Test it !
    EXPECT_VEC3_NEAR( glm::vec3( 1.0f, 1.0f/9.0f, 0.0f ), CinCamera, precision );

    // Another way is to compute "model orientation" of camera and inverse at the end !
    const Transformation viewAlternative  = Transformation::inverse( scene.V1 * (scene.T1 * (scene.P1 * scene.C0)) );
    EXPECT_VEC3_NEAR( viewAlternative._position,   view._position,   precision );
    EXPECT_VEC3_NEAR( viewAlternative._homothetie, view._homothetie, precision );
}

class SceneGraphMatrix
{
public:
    SceneGraphMatrix() :
        P0( glm::scale( glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 2.0f/9.0f, 0.0f ) ), glm::vec3( 2.0f/3.0f ) ) ),
        P1( glm::translate( glm::mat4( 1.0f ), glm::vec3( 2.0f/3.0f, 4.0f/3.0f, 0.0f ) ) ),
        T0( glm::scale( glm::translate( glm::mat4( 1.0f ), glm::vec3( -5.0f/3.0f, -2.0f/3.0f, 0.0f ) ), glm::vec3( 2.0f ) ) ),
        T1( glm::translate( glm::mat4( 1.0f ), glm::vec3( 10.0f/3.0f, 1.0f/3.0f, 0.0f ) ) ),
        V1( glm::mat4( 1.0f ) ),
        C0( glm::translate( glm::mat4( 1.0f ), glm::vec3( -1.0f, 1.0f/3.0f, 0.0f ) ) )
    {}

    glm::mat4 P0, P1, T0, T1, V1, C0;
};

TEST( SceneGraphMatrix, computeModel )
{
    const float precision = 1e-5f;
    const SceneGraphMatrix scene;

    // Compute composed orientation and apply to C.
    const glm::vec4 C( 0.0f, 1.0f/3.0f, 0.0f, 1.0f );
    const glm::mat4 model = scene.V1 * (scene.T1 * (scene.P1 * scene.P0));
    const glm::vec4 CinW = model * C;
    EXPECT_VEC4_NEAR( glm::vec4( 4.0f, 19.0f/9.0f, 0.0f, 1.0f ), CinW, precision );

    // All step with each result
    const glm::vec4 C0 = (scene.P0 * C);
    EXPECT_VEC4_NEAR( glm::vec4( 0.0f, 4.0f/9.0f, 0.0f, 1.0f ), C0, precision );
    const glm::vec4 C1 = (scene.P1 * scene.P0) * C;
    EXPECT_VEC4_NEAR( glm::vec4( 2.0f/3.0f, 16.0f/9.0f, 0.0f, 1.0f ), C1, precision );
    const glm::vec4 C2 = (scene.T1 * (scene.P1 * scene.P0)) * C;
    EXPECT_VEC4_NEAR( glm::vec4( 4.0f, 19.0f/9.0f, 0.0f, 1.0f ), C2, precision );

    // Compute inverse to retrieve C
    glm::mat4 modelInv = glm::inverse( model );
    EXPECT_VEC4_NEAR( C, modelInv * CinW, precision );

    // Other example with a point on train
    const glm::vec4 A( 4.0f/3.0f, 1.0f, 0.0f, 1.0f );
    const glm::vec4 A0 = (scene.T0 * A);
    EXPECT_VEC4_NEAR( glm::vec4( 1.0f, 4.0f/3.0f, 0.0f, 1.0f ), A0, precision );
}

TEST( SceneGraphMatrix, computeCAfterMoveT1 )
{
    const float precision = 1e-5f;
    SceneGraphMatrix scene;

    // Move T1 to example position
    scene.T1 = glm::mat4( glm::translate( glm::mat4( 1.0f ), glm::vec3( 7.0f/3.0f, 1.0f/3.0f, 0.0f ) ) );

    const glm::vec4 C( 0.0f, 1.0f/3.0f, 0.0f, 1.0f );
    // Compose model
    const glm::mat4 model = scene.V1 * (scene.T1 * (scene.P1 * scene.P0));
    // Apply Model-view on C
    const glm::vec4 CinW = model * C;
    EXPECT_VEC4_NEAR( glm::vec4( 3.0f, 19.0f/9.0f, 0.0f, 1.0f ), CinW, precision );
}

TEST( SceneGraphMatrix, computeModelView )
{
    const float precision = 1e-5f;
    SceneGraphMatrix scene;

    // Move T1 to example position
    scene.T1 = glm::mat4( glm::translate( glm::mat4( 1.0f ), glm::vec3( 7.0f/3.0f, 1.0f/3.0f, 0.0f ) ) );

    const glm::vec4 C( 0.0f, 1.0f/3.0f, 0.0f, 1.0f );
    // Compose model
    const glm::mat4 model = scene.V1 * (scene.T1 * (scene.P1 * scene.P0));
    // Compose view
    const glm::mat4 view  = (glm::inverse( scene.C0 ) * (glm::inverse( scene.P1 ) * (glm::inverse( scene.T1 ) * glm::inverse( scene.V1 ))));
    // Apply Model-view on C
    const glm::vec4 CinCamera = view * (model * C);
    // Test it !
    EXPECT_VEC4_NEAR( glm::vec4( 1.0f, 1.0f/9.0f, 0.0f, 1.0f ), CinCamera, precision );

    // Another way is to compute "model orientation" of camera and inverse at the end !
    const glm::mat4 viewAlternative = glm::inverse( scene.V1 * (scene.T1 * (scene.P1 * scene.C0)) );
    EXPECT_MAT4_NEAR( viewAlternative, view, precision );
}