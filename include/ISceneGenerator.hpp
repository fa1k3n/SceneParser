#ifndef ISCENEGENERATOR_HPP
#define ISCENEGENERATOR_HPP

#include <string>
#include <vector>
#include <array>

#include <Types.hpp>
//#include <Property.hpp>

struct SCamera {
    enum CameraType {
        BASIC,
        ADVANCED
    };
    SCamera(CameraType t, std::string n) : type(t), name(n) {}
    SCamera(const SCamera& c) : type(c.type), name(c.name), eyePoint(c.eyePoint), lookPoint(c.lookPoint), up(c.up), distanceImagePlane(c.distanceImagePlane) {}

    CameraType type;
    std::string name;
    Vector3d eyePoint = {0, 0, -1};
    Vector3d lookPoint = {0, 0, 0};
    Vector3d up            = {0.0, 1.0, 0.0};
    double distanceImagePlane = 3;
};

struct SBasicCamera : public SCamera {
    SBasicCamera(std::string n) : SCamera(CameraType::BASIC, n), fov(90), aspectRatio(4.0/3.0) { }
    SBasicCamera(const SBasicCamera& c) : SCamera(c), fov(c.fov), aspectRatio(c.aspectRatio) {}
    double fov;
    double aspectRatio;
};

struct SAdvancedCamera : public SCamera {
    SAdvancedCamera(std::string n) : SCamera(CameraType::ADVANCED, n) {}
    SAdvancedCamera(const SAdvancedCamera& c): SCamera(c), left(c.left), right(c.right), top(c.top), bottom(c.bottom) {}
    double left = 1;
    double right = 1;
    double top = 1;
    double bottom = 1;
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
    MaterialType type;
    std::string name;
    Vector3d emission = {0, 0, 0};
    Vector3d ambient = {0, 0, 0};
    Vector3d diffuse = {0, 0, 0};
    Vector3d specular = {0, 0, 0};
    double specularPower = 0;
    std::string texture;
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
    LightType type;
    std::string name;
    Vector3d ambient = {0, 0, 0};
    Vector3d diffuse    = {0, 0, 0};
    Vector3d specular = {0, 0, 0};
};

struct SPointLight : public SLight {
    SPointLight(std::string n) : SLight(SLight::POINT, n) {};
    SPointLight(const SPointLight& l) : SLight(l), position(l.position), attenuationCoefs(l.attenuationCoefs) {}
    Vector3d position = {0, 0, 0};
    Vector3d attenuationCoefs = {1, 0, 0};
};

struct SDirectionalLight : public SLight {
    SDirectionalLight(std::string n) : SLight(SLight::DIRECTIONAL, n) {};
    SDirectionalLight(const SDirectionalLight& l) : SLight(l), direction(l.direction) {}
    Vector3d direction = {0, 0, 0};
};

struct SGeometry {
     enum GeometryType {
         NONE, 
        MESH,
        SPHERE
    };
    SGeometry(GeometryType t, std::string n) : type(t), name(n) {}
    SGeometry(const SGeometry& g): type(g.type), name(g.name) {}
    GeometryType type;
    std::string name;
};

struct SSphere : public SGeometry {
    SSphere(std::string n) : SGeometry(SGeometry::SPHERE, n) {}
};

struct SVertex {
    Vector3d p = {0, 0, 0};
    Vector3d n = {0, 0, 0};
    Vector3d tc = {0, 0, 0};
};

struct SMesh : public SGeometry {
    SMesh(std::string n) : SGeometry(SGeometry::MESH, n) {}
    SMesh(const SMesh& mesh) : SGeometry(mesh), vertices(mesh.vertices), tri(mesh.tri) {}
    std::vector<SVertex> vertices;
    std::vector<Vector3d> tri;
};

struct SObject {
    SObject(std::string geom, std::string mat) : geometry(geom), material(mat) {}
    std::string name;
    std::string geometry;
    std::string material;
};

struct SMisc {};

class ISceneGenerator {
public:
    virtual ~ISceneGenerator() {}
    virtual bool Camera(SCamera& cam, Matrix4d& transform) = 0;
    virtual bool Material(SMaterial& mat, Matrix4d& transform) = 0;
    virtual bool Light(SLight& light, Matrix4d& transform) = 0;
    virtual bool Geometry(SGeometry& geometry, Matrix4d& transform) = 0;
    virtual bool Object(SObject& obj, Matrix4d& transform) = 0;
    virtual bool Misc(SMisc& misc, Matrix4d& transform) = 0;

private:

};

#endif /* ISCENEGENERATOR_HPP */

