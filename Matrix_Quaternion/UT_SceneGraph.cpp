///------------------------------------------------------------------------------------------------
/// All demonstrations and codes about http://mouca.fr/wordpress/2018/05/31/matrice-quaternion-et-graphe-de-scene-partie-3/
///
/// Author: Rominitch
///------------------------------------------------------------------------------------------------
#include "Dependancies.h"

#include "Common/Orientation.h"

///------------------------------------------------------------------------------------------------
/// Test: all scene graph data with orientation system.
class SceneGraphOrientation
{
    public:
        SceneGraphOrientation():
        P0(glm::vec3(0.0f, 2.0f/9.0f, 0.0f), glm::vec3(2.0f/3.0f)),
        P1(glm::vec3(2.0f/3.0f, 4.0f/3.0f, 0.0f)),
        T0(glm::vec3(-5.0f/3.0f, -2.0f/3.0f, 0.0f), glm::vec3(2.0f)),
        T1(glm::vec3(10.0f/3.0f, 1.0f/3.0f, 0.0f)),
        V1()
        {}

        const Orientation P0, P1, T0, T1, V1;
};

TEST(SceneGraphOrientation, computeModel)
{
    const float precision = 1e-5f;
    const SceneGraphOrientation scene;

    // Compute composed orientation and apply to C.
    const glm::vec3 C(0.0f, 1.0f/3.0f, 0.0f);
    const Orientation model = scene.V1 * (scene.T1 * (scene.P1 * scene.P0));
    const glm::vec3 CinW = model * C;
    EXPECT_VEC3_NEAR(glm::vec3(4.0f, 19.0f/9.0f, 0.0f), CinW, precision);

    // All step with each result
    const glm::vec3 C0 = (scene.P0 * C);
    EXPECT_VEC3_NEAR(glm::vec3(0.0f, 4.0f/9.0f, 0.0f),       C0, precision);
    const glm::vec3 C1 = (scene.P1 * scene.P0) * C;
    EXPECT_VEC3_NEAR(glm::vec3(2.0f/3.0f, 16.0f/9.0f, 0.0f), C1, precision);
    const glm::vec3 C2 = (scene.T1 * (scene.P1 * scene.P0)) * C;
    EXPECT_VEC3_NEAR(glm::vec3(4.0f, 19.0f/9.0f, 0.0f), C2, precision);

    // Compute inverse to retrieve C
    Orientation modelInv(model);
    modelInv.inverse();
    EXPECT_VEC3_NEAR(C, modelInv * CinW, precision);

    // Other example with a point on train
    const glm::vec3 A(4.0f/3.0f, 1.0f, 0.0f);
    const glm::vec3 A0  = (scene.T0 * A);
    EXPECT_VEC3_NEAR(glm::vec3(1.0f, 4.0f/3.0f, 0.0f), A0, precision);
}

class SceneGraphMatrix
{
    public:
        SceneGraphMatrix() :
        P0(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f/9.0f, 0.0f)), glm::vec3(2.0f/3.0f) )),
        P1(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f/3.0f, 4.0f/3.0f, 0.0f))),
        T0(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f/3.0f, -2.0f/3.0f, 0.0f)), glm::vec3(2.0f) )),
        T1(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f/3.0f, 1.0f/3.0f, 0.0f))),
        V1(glm::mat4(1.0f))
    {}

    const glm::mat4 P0, P1, T0, T1, V1;
};

TEST(SceneGraphMatrix, computeModel)
{
    const float precision = 1e-5f;
    const SceneGraphMatrix scene;

    // Compute composed orientation and apply to C.
    const glm::vec4 C(0.0f, 1.0f/3.0f, 0.0f, 1.0f);
    const glm::mat4 model = scene.V1 * (scene.T1 * (scene.P1 * scene.P0));
    const glm::vec4 CinW = model * C;
    EXPECT_VEC3_NEAR(glm::vec4(4.0f, 19.0f/9.0f, 0.0f, 1.0f), CinW, precision);

    // All step with each result
    const glm::vec4 C0 = (scene.P0 * C);
    EXPECT_VEC3_NEAR(glm::vec4(0.0f, 4.0f/9.0f, 0.0f, 1.0f), C0, precision);
    const glm::vec4 C1 = (scene.P1 * scene.P0) * C;
    EXPECT_VEC3_NEAR(glm::vec4(2.0f/3.0f, 16.0f/9.0f, 0.0f, 1.0f), C1, precision);
    const glm::vec4 C2 = (scene.T1 * (scene.P1 * scene.P0)) * C;
    EXPECT_VEC3_NEAR(glm::vec4(4.0f, 19.0f/9.0f, 0.0f, 1.0f), C2, precision);

    // Compute inverse to retrieve C
    glm::mat4 modelInv = glm::inverse(model);
    EXPECT_VEC3_NEAR(C, modelInv * CinW, precision);

    // Other example with a point on train
    const glm::vec4 A(4.0f/3.0f, 1.0f, 0.0f, 1.0f);
    const glm::vec4 A0  = (scene.T0 * A);
    EXPECT_VEC3_NEAR(glm::vec4(1.0f, 4.0f/3.0f, 0.0f, 1.0f), A0, precision);
}