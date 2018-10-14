#include "Dependancies.h"

///------------------------------------------------------------------------------------------------

TEST(Matrix, translation)
{
    glm::vec4 b( 3.0f, 2.0f, 0.0f, 1.0f );

    glm::vec4 a( 1.0f, 1.0f, 0.0f, 1.0f );
    glm::mat4 t( 1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 2.0f, 1.0f, 0.0f, 1.0f);
    EXPECT_EQ( b, t * a );

    glm::mat4 t2( 1.0f ); // Identity
    t2 = glm::translate( t2, glm::vec3( 2.0f, 1.0f, 0.0f ) );
    EXPECT_EQ( b, t2 * a );
}

TEST(Matrix, compare_composition)
{
    // My result
    glm::mat4 m( 0.0f, 1.0f, 0.0f, 0.0f,
                -1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 3.0f, 1.0f, 0.0f, 1.0f );

    // Try to find it with API
    glm::mat4 op = glm::translate( glm::mat4( 1.0f ), glm::vec3( 1.0f, 2.0f, 0.0f ) );
    glm::mat4 po = glm::inverse( op );
    glm::mat4 r  = glm::rotate(glm::mat4(1.0f), RM::Maths::PI*0.5f, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 c = op * r * po;

    // Near expected
    EXPECT_MAT4_NEAR( m, c, RM::Maths::precision );
}

TEST(Matrix, applyToVector)
{
    glm::mat4 m( 0.0f, 1.0f, 0.0f, 0.0f,
                -1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 3.0f, 1.0f, 0.0f, 1.0f );

    glm::vec4 a( 1.0f,       2.0f,      0.0f, 1.0f );
    glm::vec4 b( 4.0f/3.0f,  4.0f/3.0f, 0.0f, 1.0f );
    glm::vec4 c( 1.0f,       2.0f,      0.0f, 1.0f );
    glm::vec4 d( 5.0f/3.0f,  7.0f/3.0f, 0.0f, 1.0f );

    EXPECT_EQ( c, m * a );

    glm::vec4 computed = m * b;
    // Near expected
    EXPECT_VEC4_NEAR( d, computed, RM::Maths::precision );
}

TEST(Matrix, composition)
{
    glm::mat4 m(0.5f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.5f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.5f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 v(0.0f, 1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 p(1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                1.0f, 2.0f, 3.0f, 1.0f);

    glm::mat4 result = p * v * m;
    glm::mat4 mvp(0.0f, 0.5f, 0.0f, 0.0f,
                  0.5f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.5f, 0.0f,
                  1.0f, 2.0f, 3.0f, 1.0f);
    EXPECT_EQ(mvp, result);
    EXPECT_NE(mvp, m * v * p);
}

TEST(Matrix, rotate)
{
    const float C = 1.0f / RM::Maths::sqrt_2;
    glm::mat4 r( C, C, 0.0f, 0.0f,
                -C, C, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 r1(1.0f); // Identity
    r1 = glm::rotate(r1, RM::Maths::PI_4, glm::vec3(0.0f, 0.0f, 1.0f));

    EXPECT_EQ(r, r1);
}

TEST(Matrix, rotate2)
{
    const float C = 1.0f / sqrt(2.0f);
    glm::mat4 r(C, C, 0.0f, 0.0f,
                -C, C, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

    glm::vec4 a(2.0f/3.0f, 1.0f/3.0f, 0.0f, 1.0f);
    glm::vec4 b(C/3.0f, C, 0.0f, 1.0f);

    EXPECT_EQ(b, r * a);
}

TEST(Matrix, scaling)
{
    glm::vec3 h(0.5f, 0.5f, 0.5f);
    glm::mat4 m(h.x, 0.0f, 0.0f, 0.0f,
                0.0f, h.y, 0.0f, 0.0f,
                0.0f, 0.0f, h.z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 m1(1.0f); // Identity
    m1 = glm::scale(m1, h);
    EXPECT_EQ(m1, m);

    glm::vec4 a(2.0f/3.0f, 1.0f/3.0f, 0.0f, 1.0f);
    glm::vec4 b(1.0f, 1.0f/3.0f, 0.0f, 1.0f);

    glm::vec4 c(1.0f/3.0f, 1.0f/6.0f, 0.0f, 1.0f);
    glm::vec4 d(1.0f/2.0f, 1.0f/6.0f, 0.0f, 1.0f);

    EXPECT_EQ(c, m * a);
    EXPECT_EQ(d, m * b);
}

TEST(Matrix, composed)
{
    glm::mat4 s = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    glm::mat4 r = glm::rotate(glm::mat4(1.0f), RM::Maths::PI*0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(11.0f/6.0f, 0.5f, 0.0f));

    glm::mat4 m(0.0f, 0.5f, 0.0f, 0.0f,
                -0.5f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.5f, 0.0f,
                11.0f/6.0f, 0.5f, 0.0f, 1.0f);
    glm::mat4 c = t * r * s;

    // Near except
    EXPECT_MAT4_NEAR( m, c, RM::Maths::precision );
}

TEST(Matrix, mulComposed)
{
    glm::mat4 m(0.0f, 0.5f, 0.0f, 0.0f,
                -0.5f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.5f, 0.0f,
                11.0f/6.0f, 0.5f, 0.0f, 1.0f);
    glm::vec4 a(2.0f/3.0f, 1.0f/3.0f, 0.0f, 1.0f);
    glm::vec4 b(5.0f/3.0f, 5.0f/6.0f, 0.0f, 1.0f);

    glm::vec4 mul = m * a;

    // Near except
    EXPECT_VEC4_NEAR( b, mul, RM::Maths::precision );
}