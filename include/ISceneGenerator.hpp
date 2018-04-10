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
    CProperty<CameraType> type;
    CProperty<std::string> name;
    CProperty<double, 3> eyePoint = {{0, 0, -1}};
    CProperty<double, 3> lookPoint = {{0, 0, 0}};
    CProperty<double, 3> up             = {{0, 1, 0}};
    CProperty<double>     distanceImagePlane = 3;
};

struct SBasicCamera : public SCamera {
    SBasicCamera(std::string n) : SCamera(CameraType::BASIC, n) {}
    CProperty<double> fov = 90;
    CProperty<double> aspectRatio = 4/3;
};

struct SMaterial {
    enum MaterialType {
        BASIC
    };
    SMaterial(MaterialType t, std::string n) : type(t), name(n) {}
    CProperty<MaterialType> type;
    CProperty<std::string> name;
    CProperty<double, 3> emission = {{0, 0, 0}};
    CProperty<double, 3> ambient = {{0, 0, 0}};
    CProperty<double, 3> diffuse = {{0, 0, 0}};
    CProperty<double, 3> specular = {{0, 0, 0}};
    CProperty<double> specularPower = 0;
    CProperty<std::string> texture;
};

class ISceneGenerator {
public:
    virtual ~ISceneGenerator() {}
    virtual bool Camera(SCamera& cam) = 0;
    virtual bool Material(SMaterial& mat) = 0;
private:

};

#endif /* ISCENEGENERATOR_HPP */

