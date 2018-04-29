#include "SceneModel.hpp"
#include <SceneParser.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;


SceneModel::SceneModel(std::string modelFile, QObject* parent) : QObject(parent) {
    CSceneParser parser(*this);

    std::ifstream stream;
    stream.open (modelFile, std::ifstream::in);

    try {
        parser.ParseScene(stream);
    } catch (ParserException e) {
        std::cout << "Something when wrong: " << e.what() << std::endl;
    }
    stream.close();
}

