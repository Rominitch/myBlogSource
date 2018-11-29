///------------------------------------------------------------------------------------------------
/// Transformation structure: make similar thing than matrix 4x4 but using quaternion rotation system.
///
/// Author: Rominitch
///------------------------------------------------------------------------------------------------

#pragma once

struct Transformation
{
    glm::quat _rotation;
    glm::vec3 _position;
    glm::vec3 _homothetie;

    Transformation(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& homothetie = glm::vec3(1.0f)):
    _position(position), _homothetie(homothetie)
    {}

    Transformation(const Transformation& copy) :
    _rotation( copy._rotation ),
    _position( copy._position ),
    _homothetie( copy._homothetie )
    {}

    static Transformation inverse(const Transformation& orientation)
    {
        Transformation newOrientation(orientation);
        newOrientation.inverse();
        return newOrientation;
    }

    void inverse()
    {
        _homothetie  = 1.0f / _homothetie;
        _rotation = glm::inverse( _rotation );
        _position = (_rotation * (_homothetie * -_position));
    }

    // World = This * Local
    Transformation operator * ( const Transformation& localSpace ) const
    {
        Transformation worldSpace;
        worldSpace._position = _position + _rotation * (localSpace._position * _homothetie);
        worldSpace._rotation = _rotation * localSpace._rotation;
        worldSpace._homothetie  = _homothetie * localSpace._homothetie;
        return worldSpace;
    }

    glm::vec3 operator * (const glm::vec3& localPoint) const
    {
#ifdef _DEBUG
        glm::vec3 s = (localPoint * _homothetie);
        glm::vec3 r = _rotation * s;

        return _position + r;
#else
        return _position + _rotation * (localPoint * _homothetie);
#endif
    }

    static Transformation lerp(const Transformation& a, const Transformation& b, const float factor)
    {
        Transformation interpole;
        interpole._homothetie = a._homothetie + (b._homothetie - a._homothetie) * factor;
        interpole._rotation   = glm::lerp( a._rotation, b._rotation, factor );
        interpole._position   = a._position + (b._position - a._position) * factor;
        return interpole;
    }

    static Transformation slerp( const Transformation& a, const Transformation& b, const float factor )
    {
        Transformation interpole;
        interpole._homothetie = a._homothetie + (b._homothetie - a._homothetie) * factor;
        interpole._rotation   = glm::slerp( a._rotation, b._rotation, factor );
        interpole._position   = a._position + (b._position - a._position) * factor;
        return interpole;
    }
};