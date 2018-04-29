/*
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
*/
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "SceneModel.hpp"
#include <QQmlContext>


int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    SceneModel model(argv[1]);
    engine.rootContext()->setContextProperty("sceneModel", QVariant::fromValue(&model));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}

