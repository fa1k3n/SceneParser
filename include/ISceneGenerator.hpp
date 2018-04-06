#ifndef ISCENEGENERATOR_HPP
#define ISCENEGENERATOR_HPP

#include <string>

class CCamera {
public:
    enum CameraType {
        BASIC,
        ADVANCED
    };
    CCamera(CameraType t, std::string n) : type(t), name(n) {}
    CameraType type;
    std::string name;
};

class ISceneGenerator {
public:
    virtual ~ISceneGenerator() {}
    virtual bool Camera(CCamera& cam) = 0;
private:

};

#endif /* ISCENEGENERATOR_HPP */

