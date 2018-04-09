#ifndef ISCENEGENERATOR_HPP
#define ISCENEGENERATOR_HPP

#include <string>
#include <vector>

class CCamera {
public:
    enum CameraType {
        BASIC,
        ADVANCED
    };
    CCamera(CameraType t, std::string n) : type(t), name(n) {}
    CCamera& setEyePoint(std::vector<double> ep) {
        eyePoint = ep;
        return *this;
    }
    
    CCamera& setLookPoint(std::vector<double> ep) {
        lookPoint = ep;
        return *this;
    }
    
    CCamera& setUp(std::vector<double> ep) {
        up = ep;
        return *this;
    }
    
    CCamera& setDistanceImagePlane(double dist) {
        distanceImagePlane = dist;
        return *this;
    }
    CameraType type;
    std::string name;
    std::vector<double> eyePoint    = {0, 0, -1};
    std::vector<double> lookPoint   = {0, 0, 0};
    std::vector<double> up              = {0, 1, 0};
    double distanceImagePlane         = 3;
};

class CBasicCamera : public CCamera {
public:
    CBasicCamera(std::string n) : CCamera(CameraType::BASIC, n) {}
   
     CCamera& setFov(double f) {
        fov = f;
        return *this;
    }
      
     CCamera& setAspectRatio(double ar) {
        aspectRatio = ar;
        return *this;
    }
      
    double fov = 90;
    double aspectRatio = 4/3;
};

class CMaterial {
public:
    enum MaterialType {
        BASIC
    };
    CMaterial(MaterialType t, std::string n) : type(t), name(n) {}
    
    MaterialType type;
    std::string name;
};

class ISceneGenerator {
public:
    virtual ~ISceneGenerator() {}
    virtual bool Camera(CCamera& cam) = 0;
    virtual bool Material(CMaterial& mat) = 0;
private:

};

#endif /* ISCENEGENERATOR_HPP */

