#ifndef SCENEMODEL_HPP
#define SCENEMODEL_HPP

#include <ISceneGenerator.hpp>
#include <QAbstractListModel>
#include <QList>
#include <QObject>
#include <sstream>


static QString vecToStr(Vector3d vec) {
    std::ostringstream oss;
    oss << vec.transpose();
    return QString(oss.str().c_str());
}

class MaterialsModel : public QAbstractListModel {
    Q_OBJECT
 public:
    MaterialsModel(QObject* parent = 0) : QAbstractListModel(parent) {}
    virtual ~MaterialsModel() {}

    enum MaterialsRoles {
        NameRole = Qt::UserRole + 1
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const { return m_materials.count(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        SMaterial* mat = m_materials[index.row()];
        if (role == NameRole) return QVariant::fromValue(QString(mat->name.c_str()));
        return QVariant();
    }

    void AddMaterial(SMaterial& m) {
        emit beginInsertRows(createIndex(m_materials.count(), 0), m_materials.count(), m_materials.count());
        m_materials.append(new SMaterial(m));
        emit endInsertRows();
    }
protected:
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
         return roles;
    }
private:
    QList<SMaterial*> m_materials;
};

class LightsModel : public QAbstractListModel {
   Q_OBJECT
public:
    LightsModel(QObject* parent = 0) : QAbstractListModel(parent) {}
    virtual ~LightsModel() {}

    enum LightRoles {
        NameRole = Qt::UserRole + 1
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const { return m_lights.count(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        SLight* light = m_lights[index.row()];
        if (role == NameRole) return QVariant::fromValue(QString(light->name.c_str()));
        return QVariant();
    }

    void AddLight(SLight& l) {
        emit beginInsertRows(createIndex(m_lights.count(), 0), m_lights.count(), m_lights.count());
        m_lights.append(new SLight(l));
        emit endInsertRows();
    }
protected:
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
         return roles;
    }
private:
    QList<SLight*> m_lights;
};

class CamerasModel : public QAbstractListModel {
   Q_OBJECT
public:
    CamerasModel(QObject* parent = 0) : QAbstractListModel(parent) {}
    virtual ~CamerasModel() {}
    CamerasModel(const CamerasModel &other) { m_cameras = other.m_cameras; }

    enum CamerasRoles {
        NameRole = Qt::UserRole + 1,
        EyePointRole = Qt::UserRole + 2,
        LookPointRole = Qt::UserRole + 3,
        UpRole = Qt::UserRole + 4
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const { return m_cameras.count(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        SCamera* cam = m_cameras[index.row()];
        if (role == NameRole) return QVariant(QString(cam->name.c_str()));
        else if(role == EyePointRole) return QVariant(vecToStr(cam->eyePoint));
        else if(role == LookPointRole) return QVariant(vecToStr(cam->lookPoint));
        else if(role == UpRole) return QVariant(vecToStr(cam->up));

        return QVariant();
    }

    void AddCamera(SCamera& c) {
        emit beginInsertRows(createIndex(m_cameras.count(), 0), m_cameras.count(), m_cameras.count());
        m_cameras.append(new SCamera(c));
        emit endInsertRows();
    }
protected:
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[EyePointRole] = "eyePoint";
        roles[LookPointRole] = "lookPoint";
        roles[UpRole] = "up";

         return roles;
    }
private:
    QList<SCamera*> m_cameras;
};

class SceneModel : public QObject, public ISceneGenerator {
   Q_OBJECT
   Q_PROPERTY(LightsModel* lights READ lights NOTIFY lightsChanged);
   Q_PROPERTY(CamerasModel* cameras READ cameras NOTIFY camerasChanged);
   Q_PROPERTY(MaterialsModel* materials READ materials NOTIFY materialsChanged);

public:
    SceneModel(std::string modelFile, QObject* parent = 0);
    virtual ~SceneModel() {}

    LightsModel* lights() { return &m_lights; }
    CamerasModel* cameras() { return &m_cameras; }
    MaterialsModel* materials() { return &m_materials; }

signals:
    void lightsChanged();
    void camerasChanged();
    void materialsChanged();

protected:

    bool Camera(SCamera& cam, Matrix4d& transf) {
        m_cameras.AddCamera(cam);
        return true;
    }

    bool Material(SMaterial& mat, Matrix4d& transf) {
        m_materials.AddMaterial(mat);
        return true;
    }

    bool Light(SLight& light, Matrix4d& transf) {
        m_lights.AddLight(light);
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

private:
     CamerasModel m_cameras;
     LightsModel m_lights;
     MaterialsModel m_materials;
};
#endif /* SCENEMODEL_HPP */
