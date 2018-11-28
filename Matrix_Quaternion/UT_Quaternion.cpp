///------------------------------------------------------------------------------------------------
/// All demonstrations and codes about http://mouca.fr/wordpress/2018/05/31/matrice-quaternion-et-graphe-de-scene-partie-2/
///
/// Author: Rominitch
///------------------------------------------------------------------------------------------------
#include "Dependancies.h"

#include "Common/Orientation.h"

///------------------------------------------------------------------------------------------------

TEST(Quaternion, quaternion_mul)
{
    const float angle = RM::Maths::PI * 0.5f;
    glm::vec3 om(11.0f/6.0f, 0.5f, 0.0f );
    glm::vec3 scale( 0.5f, 0.5f, 0.5f );

    glm::vec4 a( 4.0f/6.0f, 2.0f/6.0f, 0.0f, 1.0f );
    glm::vec4 c( 10.0f/6.0f, 5.0f/6.0f, 0.0f, 1.0f );

    glm::mat4 mc;
    {
        glm::mat4 s = glm::scale( glm::mat4( 1.0f ), scale );
        glm::mat4 r = glm::rotate( glm::mat4( 1.0f ), angle, glm::vec3( 0.0f, 0.0f, 1.0f ) );
        glm::mat4 t = glm::translate( glm::mat4( 1.0f ), om );

        mc = t * r * s;

        // Near except
        glm::vec4 cal_c = mc * a;
        EXPECT_VEC4_NEAR( c, cal_c, 1e-6f );
    }

    Orientation qc;
    {
        qc._position = om;
        qc._homothetie  = scale;
        qc._rotation = glm::angleAxis( angle, glm::vec3( 0.0f, 0.0f, 1.0f ) );

        // Near except
        glm::vec3 cal_c = qc * a;
        EXPECT_VEC3_NEAR( c, cal_c, RM::Maths::precision );
    }
}

TEST(Quaternion, quaternion_mul_non_uniform)
{
    const float angle = RM::Maths::PI * 0.5f;
    glm::vec3 om( 11.0f/6.0f, 0.5f, 0.0f );
    glm::vec3 scale( 0.5f, 1.0f, 0.5f );

    glm::vec4 a( 4.0f/6.0f, 2.0f/6.0f, 0.0f, 1.0f );
    glm::vec4 c( 9.0f/6.0f, 5.0f/6.0f, 0.0f, 1.0f );

    glm::mat4 mc;
    {
        glm::mat4 s = glm::scale( glm::mat4( 1.0f ), scale );
        glm::mat4 r = glm::rotate( glm::mat4( 1.0f ), angle, glm::vec3( 0.0f, 0.0f, 1.0f ) );
        glm::mat4 t = glm::translate( glm::mat4( 1.0f ), om );

        mc = t * r * s;

        // Near except
        glm::vec4 cal_c = mc * a;
        EXPECT_VEC4_NEAR( c, cal_c, RM::Maths::precision );
    }

    Orientation qc;
    {
        qc._position = om;
        qc._homothetie  = scale;
        qc._rotation = glm::angleAxis( angle, glm::vec3( 0.0f, 0.0f, 1.0f ) );

        // Near except
        glm::vec3 cal_c = qc * a;
        EXPECT_VEC3_NEAR( c, cal_c, RM::Maths::precision );
    }
}

TEST(Quaternion, quaternion_mul_non_uniform_PI_4)
{
    const float angle = RM::Maths::PI * 0.25f;
    glm::vec3 om( 11.0f/6.0f, 0.5f, 0.0f );
    glm::vec3 scale( 0.5f, 1.0f, 0.5f );

    glm::vec4 a( 4.0f/6.0f, 2.0f/6.0f, 0.0f, 1.0f );
    glm::vec4 c( 1.83333337f, 0.971404552f, 0.0f, 1.0f );
    glm::mat4 mc;
    {
        glm::mat4 s = glm::scale( glm::mat4( 1.0f ), scale );
        glm::mat4 r = glm::rotate( glm::mat4( 1.0f ), angle, glm::vec3( 0.0f, 0.0f, 1.0f ) );
        glm::mat4 t = glm::translate( glm::mat4( 1.0f ), om );

        mc = t * r * s;

        // Near except
        glm::vec4 cal_c = mc * a;
        EXPECT_VEC4_NEAR( c, cal_c, RM::Maths::precision );
    }

    Orientation qc;
    {
        qc._position = om;
        qc._homothetie  = scale;
        qc._rotation = glm::angleAxis( angle, glm::vec3( 0.0f, 0.0f, 1.0f ) );

        // Near except
        glm::vec3 cal_c = qc * a;
        EXPECT_VEC3_NEAR( c, cal_c, RM::Maths::precision );
    }
}

TEST(Quaternion, quaternion_multiplication_way)
{
    const float angle = RM::Maths::PI * 0.5f;
    glm::quat q = glm::angleAxis( angle, glm::vec3( 0.0f, 0.0f, 1.0f ) );

    glm::vec4 a(  1.0f, 1.0f, 0.0f, 1.0f );
    glm::vec4 c( -1.0f, 1.0f, 0.0f, 1.0f );

    glm::vec3 cal_c = q * a;
    EXPECT_VEC3_NEAR( c, cal_c, RM::Maths::precision );

    glm::vec3 cal_a = c * q;
    EXPECT_VEC3_NEAR( a, cal_a, RM::Maths::precision );

    cal_a = (q * a) * q;
    EXPECT_VEC3_NEAR( a, cal_a, RM::Maths::precision );
    cal_a = q * (a * q);
    EXPECT_VEC3_NEAR( a, cal_a, RM::Maths::precision );
}

TEST(Quaternion, quaternion_inverse)
{
    const float angle = RM::Maths::PI * 0.5f;
    glm::vec3 om( 11.0f/6.0f, 0.5f, 0.0f );
    glm::vec3 scale( 0.5f, 0.5f, 0.5f );

    glm::vec4 a( 4.0f/6.0f, 2.0f/6.0f, 0.0f, 1.0f );
    glm::vec4 c( 10.0f/6.0f, 5.0f/6.0f, 0.0f, 1.0f );

    Orientation qc;
    qc._position = om;
    qc._homothetie  = scale;
    qc._rotation = glm::angleAxis( angle, glm::vec3( 0.0f, 0.0f, 1.0f ) );

    Orientation qa(qc);
    qa.inverse();

    glm::vec3 cal_c = qc * a;
    EXPECT_VEC3_NEAR( c, cal_c, RM::Maths::precision );

    glm::vec3 cal_a = qa * c;
    EXPECT_VEC3_NEAR( a, cal_a, RM::Maths::precision );
}

TEST(Quaternion, quaternion)
{
    glm::vec3 oa( 1.0f, 2.0f, 0.0f );

    glm::vec4 b(4.0f/3.0f, 4.0f/3.0f, 0.0f, 1.0f);
    glm::vec4 d(5.0f/3.0f, 7.0f/3.0f, 0.0f, 1.0f);

    glm::mat4 mc;
    {
        glm::mat4 m( 0.0f, 1.0f, 0.0f, 0.0f,
                    -1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     3.0f, 1.0f, 0.0f, 1.0f );

        glm::mat4 op = glm::translate( glm::mat4( 1.0f ), oa );
        glm::mat4 po = glm::inverse( op );
        glm::mat4 r  = glm::rotate( glm::mat4( 1.0f ), RM::Maths::PI*0.5f, glm::vec3( 0.0f, 0.0f, 1.0f ) );

        mc = op * r * po;

        // Near except
        glm::vec4 cal_d = mc * b;
        EXPECT_VEC4_NEAR( d, cal_d, RM::Maths::precision );
    }

    Orientation qc;
    {
        Orientation op;
        op._position = oa;
        Orientation po;
        po._position = -oa;
        Orientation r;
        r._rotation = glm::angleAxis(RM::Maths::PI_4, glm::vec3(0.0f, 0.0f, 1.0f));
        
        qc = op * r * po;

        // Near except
        glm::vec3 cal_d = mc * b;
        EXPECT_VEC3_NEAR( d, cal_d, RM::Maths::precision );
    }
}

TEST(Quaternion, quaternion_SRT)
{
    glm::vec3 homothetie( 0.5f, 0.5f, 0.5f );

    float angle = RM::Maths::PI / 2.0f;
    glm::vec3 axis( 0.0f, 0.0f, 1.0f );

    glm::vec3 pos( 11.0f/6.0f, 0.5f, 0.0f );
    
    glm::vec4 a( 2.0f/3.0f, 1.0f/3.0f, 0.0f, 1.0f );
    glm::vec4 c( 5.0f/3.0f, 5.0f/6.0f, 0.0f, 1.0f );
    
    {
        glm::mat4 s = glm::scale( glm::mat4( 1.0f ), homothetie );
        glm::mat4 r = glm::rotate( glm::mat4( 1.0f ), angle, axis );
        glm::mat4 t = glm::translate( glm::mat4( 1.0f ), pos );
        glm::mat4 mc = t * (r * s);

        glm::vec4 cal_c = mc * a;
        EXPECT_VEC4_NEAR( c, cal_c, RM::Maths::precision );
    }

    {
        Orientation s;
        s._homothetie = homothetie;
        Orientation r;
        r._rotation = glm::angleAxis( angle, axis );
        Orientation t;
        t._position = pos;

        Orientation qc = t * (r * s);

        // Near except
        glm::vec3 cal_c = qc * a;
        EXPECT_VEC3_NEAR( c, cal_c, RM::Maths::precision );
    }
}

TEST(Quaternion, quaternion_SRT_non_uniform)
{
    float angle = RM::Maths::PI / 2.0f;
    const glm::vec3 axis( 0.0f, 0.0f, 1.0f );

    const glm::vec3 pos( 11.0f/6.0f, 0.5f, 0.0f );

    const glm::vec3 homothetie( 0.5f, 1.0f, 0.5f );

    const glm::vec4 a( 4.0f/6.0f, 2.0f/6.0f, 0.0f, 1.0f );
    const glm::vec4 c( 9.0f/6.0f, 5.0f/6.0f, 0.0f, 1.0f );

    {
        glm::mat4 s = glm::scale( glm::mat4( 1.0f ), homothetie );
        glm::mat4 r = glm::rotate( glm::mat4( 1.0f ), angle, axis );
        glm::mat4 t = glm::translate( glm::mat4( 1.0f ), pos );
        glm::mat4 mc = t * (r * s);

        glm::vec4 cal_c = mc * a;
        EXPECT_VEC4_NEAR( c, cal_c, RM::Maths::precision );
    }

    {
        Orientation s;
        s._homothetie = homothetie;
        Orientation r;
        r._rotation = glm::angleAxis( angle, axis );
        Orientation t;
        t._position = pos;

        Orientation qc = t * (r * s);

        // Near except
        glm::vec3 cal_c = qc * a;
        EXPECT_VEC3_NEAR( c, cal_c, RM::Maths::precision );
    }
}

TEST(Quaternion, quaternion_final_composition)
{
    float angleM =  RM::Maths::PI / 2.0f;
    float angleR = -RM::Maths::PI_4;
    const glm::vec3 axis( 0.0f, 0.0f, 1.0f );

    const glm::vec4 a( 1.0f, 0.0f, 0.0f, 1.0f );
    const glm::vec4 c( 1.0f, 7.0f/3.0f, 0.0f, 1.0f );


    Orientation qm;
    qm._homothetie = glm::vec3( 0.5f, 1.0f, 1.0f );
    qm._rotation   = glm::angleAxis( angleM, axis );
    qm._position   = glm::vec3(2.0f/3.0f, 1.0f/2.0f, 0.0f);

    Orientation qr;
    qr._homothetie = glm::vec3(RM::Maths::sqrt_2 * 2.0f/3.0f, RM::Maths::sqrt_2 / 3.0f, 1.0f );
    qr._rotation   = glm::angleAxis( angleR, axis );
    qr._position   = glm::vec3(3.0f, 0.0f, 0.0f);

    EXPECT_VEC3_NEAR( glm::vec3(2.0f/3.0f, 2.0f, 0.0f),
                        qm * qr._position, RM::Maths::precision );

    // Composition
    Orientation qc = qm * qr;

    // Near except
    glm::vec3 cal_c = qc * a;
    EXPECT_VEC3_NEAR( c, cal_c, RM::Maths::precision );
}

TEST(Quaternion, quaternion_interpolate )
{
    float angle = RM::Maths::PI;
    const glm::vec3 axis( 0.0f, 0.0f, 1.0f );

    const glm::vec4 a( 1.0f, 1.0f, 0.0f, 1.0f );

    Orientation qo;

    Orientation qm;
    qm._homothetie = glm::vec3( 2.0f, 2.0f, 2.0f );
    qm._rotation   = glm::angleAxis( angle, axis );
    qm._position   = glm::vec3(4.0f, 4.0f, 0.0f);

    const int animation = 30;
    for( int i=0; i < animation; ++i)
    {
        float f = float( i ) / float( animation - 1 );
        Orientation qc = Orientation::slerp( qo, qm, f );

        // Near except
        glm::vec3 cal_c = qc * a;

        std::cout << cal_c.x << "," << cal_c.y << std::endl;
    }

    Orientation qc = Orientation::slerp( qo, qm, 0.5f );

    // Near except
    {
        const glm::vec4 c( 3.5f, 0.5f, 0.0f, 1.0f );
        glm::vec3 cal_c = qc * a;
        EXPECT_VEC3_NEAR( c, cal_c, RM::Maths::precision );
    }
}