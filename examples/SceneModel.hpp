#ifndef SCENEMODEL_HPP
#define SCENEMODEL_HPP

#include <ISceneGenerator.hpp>
#include <QAbstractListModel>
#include <QList>
#include <QObject>
#include <sstream>


static QString vecToStr(Vector3d vec) {
    std::ostringstream oss;
    oss << "(" << vec.transpose()[0] << "," << vec.transpose()[1] << "," << vec.transpose()[2] << ")";
    return QString(oss.str().c_str());
}

class MaterialsModel : public QAbstractListModel {
    Q_OBJECT
 public:
    MaterialsModel(QObject* parent = 0) : QAbstractListModel(parent) {}
    virtual ~MaterialsModel() {}

    enum MaterialsRoles {
        TypeRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        EmissionRole = Qt::UserRole + 3,
        AmbientRole = Qt::UserRole + 4,
        DiffuseRole = Qt::UserRole + 5,
        SpecularRole = Qt::UserRole + 6,
        SpecularPowerRole = Qt::UserRole + 7,
        TextureRole = Qt::UserRole + 8,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const { return m_materials.count(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        SMaterial* mat = m_materials[index.row()];
        if (role == TypeRole) return mat->type == SMaterial::BASIC ? QString("basic") : QString("unknown");
        else if (role == NameRole) return QVariant::fromValue(QString(mat->name.c_str()));
        else if(role == EmissionRole) return QVariant(vecToStr(mat->emission));
        else if(role == AmbientRole) return QVariant(vecToStr(mat->ambient));
        else if(role == DiffuseRole) return QVariant(vecToStr(mat->diffuse));
        else if(role == SpecularRole) return QVariant(vecToStr(mat->specular));
        else if(role == SpecularPowerRole) return mat->specularPower;
        else if(role == TextureRole) return QVariant(mat->texture.c_str());

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
        roles[TypeRole] = "type";
        roles[NameRole] = "name";
        roles[EmissionRole] = "emission";
        roles[AmbientRole] = "ambient";
        roles[DiffuseRole] = "diffuse";
        roles[SpecularRole] = "specular";
        roles[SpecularPowerRole] = "specularPower";
        roles[TextureRole] = "texture";
        return roles;
    }
private:
    QList<SMaterial*> m_materials;
};

class GeometryModel : public QAbstractListModel {
    Q_OBJECT
 public:
    GeometryModel(QObject* parent = 0) : QAbstractListModel(parent) {}
    virtual ~GeometryModel() {}

    enum GeometryRoles {
        NameRole = Qt::UserRole + 1,
        TypeRole = Qt::UserRole + 2,
        VerticesRole = Qt::UserRole + 3,
        TriRole = Qt::UserRole + 4
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const { return m_geometries.count(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        SGeometry* geom = m_geometries[index.row()];
        if (role == NameRole) return QVariant::fromValue(QString(geom->name.c_str()));
        else if (role == TypeRole) return geom->type == SGeometry::MESH ? QVariant("mesh") : QVariant("sphere");
        else if (role == VerticesRole) {
            //QString ret;
            QStringList ret;
            //SMesh* mesh = static_cast<SMesh*>(geom);
            for (auto vert : static_cast<SMesh*>(geom)->vertices) {
                //ret.push_back(QString(vecToStr(vert.n)) + QString(vecToStr(vert.p)) + QString(vecToStr(vert.tc)));
                ret.append(QString(vecToStr(vert.n)) + QString(vecToStr(vert.p)) + QString(vecToStr(vert.tc)));
            }
            return ret;
        }
        else if (role == TriRole) {
            QString ret;
            //for (auto tri : static_cast<SMesh*>(geom)->tri) {
            //    ret.push_back(QString(std::string(std::begin(tri), std::end(tri)).c_str()));
            //}
            ret.append("World");
            return ret;
        }
        return QVariant();
    }

    void AddGeometry(SGeometry& g) {
        emit beginInsertRows(createIndex(m_geometries.count(), 0), m_geometries.count(), m_geometries.count());
        if (g.type == SGeometry::SPHERE) m_geometries.append(new SSphere(*static_cast<SSphere*>(&g)));
        else if (g.type == SGeometry::MESH) m_geometries.append(new SMesh(*static_cast<SMesh*>(&g)));

        emit endInsertRows();
    }
protected:
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        roles[VerticesRole] = "vertices";
        roles[TriRole] = "tri";

         return roles;
    }
private:
    QList<SGeometry*> m_geometries;
};

class LightsModel : public QAbstractListModel {
   Q_OBJECT
public:
    LightsModel(QObject* parent = 0) : QAbstractListModel(parent) {}
    virtual ~LightsModel() {}

    enum LightRoles {
        NameRole = Qt::UserRole + 1,
        TypeRole = Qt::UserRole + 2,
        AmbientRole = Qt::UserRole + 3,
        DiffuseRole = Qt::UserRole + 4,
        SpecularRole = Qt::UserRole + 5,
        PositionRole = Qt::UserRole + 6,
        AttenuationCoeffsRole = Qt::UserRole + 7,
        DirectionRole = Qt::UserRole + 8,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const { return m_lights.count(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        SLight* light = m_lights[index.row()];
        if (role == NameRole) return QVariant::fromValue(QString(light->name.c_str()));
        else if (role == TypeRole) return light->type == SLight::DIRECTIONAL ? QVariant("directional") : QVariant("point");
        else if (role == AmbientRole) return QVariant(vecToStr(light->ambient));
        else if (role == DiffuseRole) return QVariant(vecToStr(light->diffuse));
        else if (role == SpecularRole) return QVariant(vecToStr(light->specular));
        else if (role == PositionRole) return QVariant(vecToStr(static_cast<SPointLight*>(light)->position));
        else if (role == AttenuationCoeffsRole) return QVariant(vecToStr(static_cast<SPointLight*>(light)->attenuationCoefs));
        else if (role == DirectionRole) return QVariant(vecToStr(static_cast<SDirectionalLight*>(light)->direction));
        return QVariant();
    }

    void AddLight(SLight& l) {
        emit beginInsertRows(createIndex(m_lights.count(), 0), m_lights.count(), m_lights.count());
        if(l.type == SLight::POINT) m_lights.append(new SPointLight(*static_cast<SPointLight*>(&l)));
        else if(l.type == SLight::DIRECTIONAL) m_lights.append(new SDirectionalLight(*static_cast<SDirectionalLight*>(&l)));
        emit endInsertRows();
    }
protected:
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        roles[AmbientRole] = "ambient";
        roles[DiffuseRole] = "diffuse";
        roles[SpecularRole] = "specular";
        roles[PositionRole] = "position";
        roles[AttenuationCoeffsRole] = "attenuationCoeffs";
        roles[DirectionRole] = "direction";
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
        UpRole = Qt::UserRole + 4,
        TypeRole = Qt::UserRole + 5,
        DistImgPlaneRole = Qt::UserRole + 6,
        FovRole = Qt::UserRole + 7,
        AspectRatioRole = Qt::UserRole + 8,
        LeftRole = Qt::UserRole + 9,
        RightRole = Qt::UserRole + 10,
        TopRole = Qt::UserRole + 11,
        BottomRole = Qt::UserRole + 12,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const { return m_cameras.count(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        // Sanity checks
        if(!index.isValid()) return QVariant();
        if(index.row() >= m_cameras.count()) return QVariant();

        SCamera* cam = m_cameras[index.row()];
        double ar = (double)static_cast<SBasicCamera*>(cam)->aspectRatio;
        if (role == NameRole) return QVariant(QString(cam->name.c_str()));
        else if(role == EyePointRole) return QVariant(vecToStr(cam->eyePoint));
        else if(role == LookPointRole) return QVariant(vecToStr(cam->lookPoint));
        else if(role == UpRole) return QVariant(vecToStr(cam->up));
        else if(role == TypeRole) return QVariant(cam->type == 0 ? "basic" : "advanced");
        else if(role == DistImgPlaneRole) return QVariant(cam->distanceImagePlane);
        else if(role == FovRole) return QVariant(static_cast<SBasicCamera*>(cam)->fov);
        else if(role == AspectRatioRole) return QVariant::fromValue(ar);
        else if(role == LeftRole) return QVariant(static_cast<SAdvancedCamera*>(cam)->left);
        else if(role == RightRole) return static_cast<SAdvancedCamera*>(cam)->right;
        else if(role == TopRole) return static_cast<SAdvancedCamera*>(cam)->top;
        else if(role == BottomRole) return static_cast<SAdvancedCamera*>(cam)->bottom;

        // Something went wrong
        return QVariant();
    }

    void AddCamera(SCamera& c) {
        emit beginInsertRows(createIndex(m_cameras.count(), 0), m_cameras.count(), m_cameras.count());
        if(c.type == SCamera::BASIC) m_cameras.append(new SBasicCamera(*static_cast<SBasicCamera*>(&c)));
        else if(c.type == SCamera::ADVANCED) m_cameras.append(new SAdvancedCamera(*static_cast<SAdvancedCamera*>(&c)));
        emit endInsertRows();
    }
protected:
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[EyePointRole] = "eyePoint";
        roles[LookPointRole] = "lookPoint";
        roles[UpRole] = "up";
        roles[TypeRole] = "type";
        roles[DistImgPlaneRole] = "distanceImagePlane";
        roles[FovRole] = "fov";
        roles[AspectRatioRole] = "aspectRatio";
        roles[LeftRole] = "left";
        roles[RightRole] = "right";
        roles[TopRole] = "top";
        roles[BottomRole] = "bottom";

         return roles;
    }
private:
    QList<SCamera*> m_cameras;
};

class SceneModel : public QObject, public ISceneGenerator {
   Q_OBJECT
   Q_PROPERTY(LightsModel* lights READ lights NOTIFY lightsChanged)
   Q_PROPERTY(CamerasModel* cameras READ cameras NOTIFY camerasChanged)
   Q_PROPERTY(MaterialsModel* materials READ materials NOTIFY materialsChanged)
   Q_PROPERTY(GeometryModel* geometries READ geometries NOTIFY geometriesChanged)

public:
    SceneModel(std::string modelFile, QObject* parent = 0);
    virtual ~SceneModel() {}

    LightsModel* lights() { return &m_lights; }
    CamerasModel* cameras() { return &m_cameras; }
    MaterialsModel* materials() { return &m_materials; }
    GeometryModel* geometries() { return &m_geometries; }

signals:
    void lightsChanged();
    void camerasChanged();
    void materialsChanged();
    void geometriesChanged();

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
        m_geometries.AddGeometry(geom);
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
     GeometryModel m_geometries;
};
#endif /* SCENEMODEL_HPP */
