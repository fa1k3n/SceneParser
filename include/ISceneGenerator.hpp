#ifndef ISCENEGENERATOR_HPP
#define ISCENEGENERATOR_HPP

#include <string>
#include <vector>

#include <Types.hpp>

struct CameraException : public std::exception {
    CameraException(std::string msg) : exception(), m_msg(msg) {}
    const char* what() const throw() {
        return m_msg.c_str();
    }
    
    private:
        std::string m_msg;
};

class CCamera {
public:
    enum CameraType {
        BASIC,
        ADVANCED
    };
    CCamera(CameraType t, std::string n) : type(t), name(n) {}
    CCamera& setEyePoint(std::vector<double> ep) {
        if(ep.size() != 3) throw CameraException("Camera: eye point array dimension mismatch");
        eyePoint = ep;
        return *this;
    }
    
    CCamera& setLookPoint(std::vector<double> ep) {
        if(ep.size() != 3) throw CameraException("Camera: look point array dimension mismatch");
        lookPoint = ep;
        return *this;
    }
    
    CCamera& setUp(std::vector<double> ep) {
        if(ep.size() != 3) throw CameraException("Camera: up array dimension mismatch");
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
    std::vector<double> emission = {0, 0, 0};
    std::vector<double> ambient = {0, 0, 0};
    std::vector<double> diffuse = {0, 0, 0};
    std::vector<double> specular = {0, 0, 0};
    double specularPower = 0;
    std::string texture = "";;
};

class ISceneGenerator {
public:
    virtual ~ISceneGenerator() {}
    virtual bool Camera(CCamera& cam) = 0;
    virtual bool Material(CMaterial& mat) = 0;
private:

};

#endif /* ISCENEGENERATOR_HPP */

