#include <SceneParser.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;
class ExampleGenerator : public ISceneGenerator {
    bool Camera(SCamera& cam, Matrix4d& transf) {
        cout << "=> CAMERA #" << m_cameras.size() << std::endl;
        m_cameras.push_back(cam);
        string type = cam.type == SCamera::BASIC ? "basic" : "advanced";
        cout << setw(20) << std::right << "Type: " << type << std::endl;
        cout << setw(20) << std::right << "Name: " << cam.name << std::endl;
        cout << setw(20) << std::right <<  "Eyepoint: "  << cam.eyePoint.transpose()   << std::endl;
        cout << setw(20) << std::right << "Lookpoint: " << cam.lookPoint.transpose() << std::endl;
        cout << setw(20) << std::right << "Up: " << cam.up.transpose() << std::endl;
        if(cam.type == SCamera::BASIC) {
            SBasicCamera* c = static_cast<SBasicCamera*>(&cam);
            cout << setw(20) << std::right << "Field of view: " << c->fov << std::endl;
            cout << setw(20) << std::right << "Aspect ratio: " << c->aspectRatio << std::endl;
        } else {
            SAdvancedCamera* c = static_cast<SAdvancedCamera*>(&cam);
            cout << setw(20) << std::right << "Left: " << c->left << std::endl;
            cout << setw(20) << std::right << "Right: " << c->right << std::endl;
            cout << setw(20) << std::right << "Top: " << c->top << std::endl;
            cout << setw(20) << std::right << "Bottom: " << c->bottom << std::endl;
        }
        return true;
    }

    bool Material(SMaterial& mat, Matrix4d& transf) {
        return true;
    }

    bool Light(SLight& light, Matrix4d& transf) {
        return true;
    }

    bool Geometry(SGeometry& geom, Matrix4d& transf) {
        return true;
    }

     bool Object(SObject& obj, Matrix4d& transf) {
        return true;
    }

     bool Misc(SMisc& misc, Matrix4d& transf) {
        return true;
    }
     
     std::vector<SCamera> m_cameras;
};

int main(void) {
    ExampleGenerator generator;
    CSceneParser parser(generator);
    parser.ParseScene("camera { type basic name foo } camera { type advanced name bar }");
    return 0;
}

