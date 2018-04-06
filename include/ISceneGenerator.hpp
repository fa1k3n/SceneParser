#ifndef ISCENEGENERATOR_HPP
#define ISCENEGENERATOR_HPP

class ISceneGenerator {
public:
    virtual ~ISceneGenerator() {}
    virtual bool Camera() = 0;
private:

};

#endif /* ISCENEGENERATOR_HPP */

