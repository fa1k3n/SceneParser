#include <SceneParser.hpp>
#include <iostream>
#include <sstream>

using namespace std;
class ExampleGenerator : public ISceneGenerator {
    bool Camera(SCamera& cam, Matrix4d& transf) {
        cout << "=> CAMERA\n";
        string type = cam.type == SCamera::BASIC ? "basic" : "advanced";
        cout << "\tType: " << type << std::endl;
        cout << "\tName: " << cam.name << std::endl;
        cout << "\tEyepoint: " << Vector3dToStr(cam.eyePoint) << std::endl;
        cout << "\tLookpoint: " << Vector3dToStr(cam.lookPoint) << std::endl;
        cout << "\tUp: " << Vector3dToStr(cam.up) << std::endl;

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
     
     string Vector3dToStr(Vector3d vect) {
         std::stringstream buffer;
        buffer << "(" << vect[0] << "," << vect[1] << "," << vect[2] <<")";
        return buffer.str();
     }
};

int main(void) {
    ExampleGenerator generator;
    CSceneParser parser(generator);
    parser.ParseScene("camera { type basic name foo } ");
    return 0;
}

