#pragma once

struct Orientation
{
    glm::quat _rotation;
    glm::vec3 _position;
    glm::vec3 _homothetie;

    Orientation(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& homothetie = glm::vec3(1.0f)):
    _position(position), _homothetie(homothetie)
    {}

    Orientation(const Orientation& copy) :
    _rotation( copy._rotation ),
    _position( copy._position ),
    _homothetie( copy._homothetie )
    {}

    static Orientation inverse(const Orientation& orientation)
    {
        Orientation newOrientation(orientation);
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
    Orientation operator * ( const Orientation& localSpace ) const
    {
        Orientation worldSpace;
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

    static Orientation lerp(const Orientation& a, const Orientation& b, const float factor)
    {
        Orientation interpole;
        interpole._homothetie = a._homothetie + (b._homothetie - a._homothetie) * factor;
        interpole._rotation   = glm::lerp( a._rotation, b._rotation, factor );
        interpole._position   = a._position + (b._position - a._position) * factor;
        return interpole;
    }

    static Orientation slerp( const Orientation& a, const Orientation& b, const float factor )
    {
        Orientation interpole;
        interpole._homothetie = a._homothetie + (b._homothetie - a._homothetie) * factor;
        interpole._rotation   = glm::slerp( a._rotation, b._rotation, factor );
        interpole._position   = a._position + (b._position - a._position) * factor;
        return interpole;
    }
};