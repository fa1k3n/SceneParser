#ifndef ISCENEGENERATOR_HPP
#define ISCENEGENERATOR_HPP

#include <string>
#include <vector>
#include <array>

#include <Types.hpp>
#include <Property.hpp>

struct SCamera {
    enum CameraType {
        BASIC,
        ADVANCED
    };
    SCamera(CameraType t, std::string n) : type(t), name(n) {}
    SCamera(const SCamera& c) : type(c.type), name(c.name), eyePoint(c.eyePoint), lookPoint(c.lookPoint), up(c.up), distanceImagePlane(c.distanceImagePlane) {}

    CProperty<CameraType> type;
    CProperty<std::string> name;
    CProperty<double, 3> eyePoint = {{0, 0, -1}};
    CProperty<double, 3> lookPoint = {{0, 0, 0}};
    CProperty<double, 3> up             = {{0, 1, 0}};
    CProperty<double>     distanceImagePlane = 3;
};

struct SBasicCamera : public SCamera {
    SBasicCamera(std::string n) : SCamera(CameraType::BASIC, n) { }
    SBasicCamera(const SBasicCamera& c) : SCamera(c), fov(c.fov), aspectRatio(c.aspectRatio) {}
    CProperty<double> fov = 90;
    CProperty<double> aspectRatio = 4.0/3.0;
};

struct SAdvancedCamera : public SCamera {
    SAdvancedCamera(std::string n) : SCamera(CameraType::ADVANCED, n) {}
    SAdvancedCamera(const SAdvancedCamera& c): SCamera(c), left(c.left), right(c.right), top(c.top), bottom(c.bottom) {}
    CProperty<double> left = 1;
    CProperty<double> right = 1;
    CProperty<double> top = 1;
    CProperty<double> bottom = 1;
};

struct SMaterial {
    enum MaterialType {
        BASIC
    };
    SMaterial(MaterialType t, std::string n) : type(t), name(n) {}
    SMaterial(const SMaterial& mat) :
        type(mat.type), 
        name(mat.name), 
        emission(mat.emission), 
        ambient(mat.ambient), 
        diffuse(mat.diffuse), 
        specular(mat.specular),
        specularPower(mat.specularPower),
        texture(mat.texture) {}
    CProperty<MaterialType> type;
    CProperty<std::string> name;
    CProperty<double, 3> emission = {{0, 0, 0}};
    CProperty<double, 3> ambient = {{0, 0, 0}};
    CProperty<double, 3> diffuse = {{0, 0, 0}};
    CProperty<double, 3> specular = {{0, 0, 0}};
    CProperty<double> specularPower = 0;
    CProperty<std::string> texture;
};

struct SBasicMaterial : public SMaterial {
    SBasicMaterial(std::string n) : SMaterial(SMaterial::BASIC, n) {}
    SBasicMaterial(const SBasicMaterial& mat) : SMaterial(mat) {}
};

struct SLight {
    enum LightType {
        NONE,
        POINT,
        DIRECTIONAL
    };
    SLight(LightType t = SLight::NONE, std::string n = "") : type(t), name(n) {}
    SLight(const SLight& l) : type(l.type), name(l.name), ambient(l.ambient), diffuse(l.diffuse), specular(l.specular) {}
    CProperty<LightType> type;
    CProperty<std::string> name;
    CProperty<double, 3> ambient = {{0, 0, 0}};
    CProperty<double, 3> diffuse    = {{0, 0, 0}};
    CProperty<double, 3> specular = {{0, 0, 0}};
};

struct SPointLight : public SLight {
    SPointLight(std::string n) : SLight(SLight::POINT, n) {};
    SPointLight(const SPointLight& l) : SLight(l), position(l.position), attenuationCoefs(l.attenuationCoefs) {}
    CProperty<double, 3> position = {{0, 0, 0}};
    CProperty<double, 3> attenuationCoefs = {{1, 0, 0}};
};

struct SDirectionalLight : public SLight {
    SDirectionalLight(std::string n) : SLight(SLight::DIRECTIONAL, n) {};
    SDirectionalLight(const SDirectionalLight& l) : SLight(l), direction(l.direction) {}
    CProperty<double, 3> direction = {{0, 0, 0}};
};

struct SGeometry {
     enum GeometryType {
        MESH,
        SPHERE
    };
    SGeometry(GeometryType t, std::string n) : type(t), name(n) {}
    CProperty<GeometryType> type;
    CProperty<std::string> name;
};

struct SSphere : public SGeometry {
    SSphere(std::string n) : SGeometry(SGeometry::SPHERE, n) {}
};

struct SVertex {
    CProperty<double, 3> p = {{0, 0, 0}};
    CProperty<double, 3> n = {{0, 0, 0}};
    CProperty<double, 3> tc = {{0, 0, 0}};
};

struct SMesh : public SGeometry {
    SMesh(std::string n) : SGeometry(SGeometry::MESH, n) {}
    CProperty<SVertex> vertices;
    CProperty<double, 3> tri;
};

class ISceneGenerator {
public:
    virtual ~ISceneGenerator() {}
    virtual bool Camera(SCamera& cam) = 0;
    virtual bool Material(SMaterial& mat) = 0;
    virtual bool Light(SLight& light) = 0;
private:

};

#endif /* ISCENEGENERATOR_HPP */

