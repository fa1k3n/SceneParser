#include <SceneParser.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

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
        cout << setw(20) << std::right << "Distance image plane: " << cam.distanceImagePlane << std::endl;
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
       /* cout << "=> GEOMETRY #" << m_geometries.size() << std::endl;
        m_geometries.push_back(geom);
        string type = geom.type == SGeometry::MESH ? "mesh" : "sphere";
        cout << setw(20) << std::right << "Type: " << type << std::endl;
        cout << setw(20) << std::right << "Name: " << geom.name << std::endl;*/
        m_geometries.insert( std::map<std::string, SGeometry>::value_type (geom.name, geom));
        return true;
    }

     bool Object(SObject& obj, Matrix4d& transf) {
        cout << "=> OBJECT #" << m_objects.size() << std::endl;
        m_objects.push_back(obj);
        cout << setw(20) << std::right << "Name: " << obj.name << std::endl;
        cout << setw(20) << std::right << "Geometry: " << std::endl;
        SGeometry& geom = m_geometries.find(obj.geometry)->second;
         string type = geom.type == SGeometry::MESH ? "mesh" : "sphere";
        cout << setw(24) << std::right << "Type: " << type << std::endl;
        cout << setw(24) << std::right << "Name: " << geom.name << std::endl;
        return true;
    }

     bool Misc(SMisc& misc, Matrix4d& transf) {
        return true;
    }
     
     std::vector<SCamera> m_cameras;
     std::vector<SObject> m_objects;
     std::map<std::string, SGeometry> m_geometries;
};

int main(void) {
    ExampleGenerator generator;
    CSceneParser parser(generator);
    std::ifstream stream;
    stream.open ("../examples/ex1.scene", std::ifstream::in);
      
    try {
        parser.ParseScene(stream);
    } catch (ParserException e) {
        std::cout << "Something when wrong: " << e.what() << std::endl;
    }
    
    stream.close();
        return 0;
}

