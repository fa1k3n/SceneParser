#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>

#include <SceneParser.hpp>
#include <ISceneGenerator.hpp>
using namespace std;

using namespace ::testing;

class TestSceneGenerator : public ISceneGenerator {
public:
    MOCK_METHOD2(Camera, bool(SCamera&, Matrix4d&));   
    MOCK_METHOD2(Material, bool(SMaterial&, Matrix4d&));
    MOCK_METHOD2(Light, bool(SLight&, Matrix4d&));
    MOCK_METHOD2(Geometry, bool(SGeometry&, Matrix4d&));
    MOCK_METHOD2(Object, bool(SObject&, Matrix4d&));
    MOCK_METHOD2(Misc, bool(SMisc&, Matrix4d&));

    TestSceneGenerator() {
        ON_CALL(*this, Light(::testing::_, ::testing::_)).WillByDefault(::testing::Invoke(this, &TestSceneGenerator::SaveLight));
        ON_CALL(*this, Camera(::testing::_, ::testing::_)).WillByDefault(::testing::Invoke(this, &TestSceneGenerator::SaveCamera));
        ON_CALL(*this, Material(::testing::_, ::testing::_)).WillByDefault(::testing::Invoke(this, &TestSceneGenerator::SaveMaterial));
        ON_CALL(*this, Geometry(::testing::_, ::testing::_)).WillByDefault(::testing::Invoke(this, &TestSceneGenerator::SaveGeometry));
        ON_CALL(*this, Object(::testing::_, ::testing::_)).WillByDefault(::testing::Invoke(this, &TestSceneGenerator::SaveObject));
        ON_CALL(*this, Misc(::testing::_, ::testing::_)).WillByDefault(Invoke(this, &TestSceneGenerator::SaveMisc));
    }
     ~TestSceneGenerator() {
         if(light) delete light;
         if(material) delete material;
         if(camera) delete camera;
         if(geometry) delete geometry;
         if(object) delete object;
     }
     
    bool SaveLight(SLight& l,  Matrix4d& t) { 
        if(l.type == SLight::DIRECTIONAL) light = new SDirectionalLight(*static_cast<SDirectionalLight*>(&l));
        else if(l.type == SLight::POINT) light = new SPointLight(*static_cast<SPointLight*>(&l));
        return true;
    }
    
    bool SaveMaterial(SMaterial &mat, Matrix4d& t) {
        if(mat.type == SMaterial::BASIC) material = new SBasicMaterial(*static_cast<SBasicMaterial*>(&mat));
        return true;
    }
    
    bool SaveObject(SObject& obj, Matrix4d& t) {
        object = new SObject(obj);
        return true;
    }
    
    bool SaveCamera(SCamera& cam, Matrix4d& t) {
         if(cam.type == SCamera::BASIC) camera = new SBasicCamera(*static_cast<SBasicCamera*>(&cam));
         else if(cam.type == SCamera::ADVANCED) camera = new SAdvancedCamera(*static_cast<SAdvancedCamera*>(&cam));
         return true;
    }
    
    bool SaveGeometry(SGeometry& geom, Matrix4d& t) {
       transform = t;
        if(geom.type == SGeometry::SPHERE) geometry = new SSphere(*static_cast<SSphere*>(&geom));
        else if(geom.type == SGeometry::MESH) geometry = new SMesh(*static_cast<SMesh*>(&geom));
        return true;
    }
    
    bool SaveMisc(SMisc& misc, Matrix4d& m) {
        transform = m;
        return true;
    }
    
    SDirectionalLight* GetDirectionalLight() { return static_cast<SDirectionalLight*>(light); }
    SPointLight* GetPointLight() { return static_cast<SPointLight*>(light);}
    
    SBasicCamera* GetBasicCamera() {  return static_cast<SBasicCamera*>(camera); }
    SAdvancedCamera* GetAdvancedCamera() {  return static_cast<SAdvancedCamera*>(camera); }

    SBasicMaterial* GetBasicMaterial() { return static_cast<SBasicMaterial*>(material); }

    SSphere* GetSphere() { return static_cast<SSphere*>(geometry); }
    SMesh* GetMesh() { return static_cast<SMesh*>(geometry); }
    
    SObject* GetObject() { return object; }
    
    Matrix4d GetTransform() { return transform; }

    SLight* light = nullptr; 
    SMaterial* material = nullptr;
    SCamera* camera = nullptr;
    SGeometry* geometry = nullptr;
    SObject* object = nullptr;
    Matrix4d transform;
};

TEST(SceneParser, testConstructor) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    ASSERT_TRUE(true);
}

TEST(SceneParserCamera, defaultCameraParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
   EXPECT_CALL(generator, Camera(_, _));
   istringstream scene( "Camera {\n"
    "    Type Basic\n"
    "    Name first_camera\n"
    "}");
    bool success = parser.ParseScene(scene);
    ASSERT_TRUE(success);
    SBasicCamera* cam = generator.GetBasicCamera();
    EXPECT_TRUE(cam != nullptr);
    
    // Check defaults
    ASSERT_EQ(cam->eyePoint, Vector3d(0, 0, -1));
    ASSERT_EQ(cam->lookPoint, Vector3d(0, 0, 0));
    ASSERT_EQ(cam->up, Vector3d(0, 1, 0));
    ASSERT_EQ(3, cam->distanceImagePlane);
    ASSERT_EQ(90, cam->fov);
    ASSERT_EQ(4.0/3.0, cam->aspectRatio);
}

TEST(SceneParserCamera, testCameraParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Camera(_, _));
  
    bool success = parser.ParseScene("Camera {\n"
    "    Type Basic\n"
    "    Name first_camera\n"
    "    Eye_point 0.2 0.3 0.6 \n"
    "    Look_point .3 0.1 -0.7 \n"
    "    Up 0.2 -0.1 0 \n"
    "    Distance_image_plane 7 \n"
    "    fov 45  \n"
    "    aspect_ratio 0.3 \n"
    " }");
    ASSERT_TRUE(success);    
    SBasicCamera* cam = generator.GetBasicCamera();
    EXPECT_TRUE(cam != nullptr);
    
    ASSERT_EQ(SCamera::BASIC, cam->type);
    ASSERT_STREQ("first_camera", cam->name.c_str());
    ASSERT_EQ(cam->eyePoint, Vector3d(0.2, 0.3, 0.6));
    ASSERT_EQ(cam->lookPoint, Vector3d(0.3, 0.1, -0.7));
    ASSERT_EQ(cam->up, Vector3d(0.2, -0.1, 0));
    ASSERT_EQ(7, cam->distanceImagePlane);
    ASSERT_EQ(45, cam->fov);
    ASSERT_EQ(0.3, cam->aspectRatio);
}

TEST(SceneParserCamera, advancedCameraDefaultParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
   EXPECT_CALL(generator, Camera(_, _));
   istringstream scene( "Camera {\n"
    "    Type Advanced\n"
    "    Name first_camera\n"
    "}");
    bool success = parser.ParseScene(scene);
    ASSERT_TRUE(success);
    SAdvancedCamera* cam = generator.GetAdvancedCamera();
    EXPECT_TRUE(cam != nullptr);
    
    // Check defaults
    ASSERT_EQ(1, cam->left);
    ASSERT_EQ(1, cam->right);
    ASSERT_EQ(1, cam->top);
    ASSERT_EQ(1, cam->bottom);
}

TEST(SceneParserCamera, advancedCameraParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
   EXPECT_CALL(generator, Camera(_, _));
   istringstream scene( "Camera {\n"
    "    Type Advanced\n"
    "    Name first_camera\n"
    "    left 2\n"
    "    right 3\n"
    "    top 4\n"
    "    bottom 5\n"
   "}");
    bool success = parser.ParseScene(scene);
    ASSERT_TRUE(success);
    SAdvancedCamera* cam = generator.GetAdvancedCamera();
    EXPECT_TRUE(cam != nullptr);
    
    // Check defaults
    ASSERT_EQ(2, cam->left);
    ASSERT_EQ(3, cam->right);
    ASSERT_EQ(4, cam->top);
    ASSERT_EQ(5, cam->bottom);
}

TEST(SceneParserCamera, testMissingRequiredKey) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    istringstream scene("Camera { Type Basic }");
    EXPECT_THROW(parser.ParseScene(scene), ParserException);
    
    istringstream scene2("Camera { }");
    EXPECT_THROW(parser.ParseScene(scene2), ParserException);
}

TEST(SceneParserCamera, testTypeNotFirst) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_THROW(parser.ParseScene("Camera { Name foo Type Basic }"), ParserException);
}

TEST(SceneParserCamera, testArrayMissmatch) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_THROW(parser.ParseScene("Camera { Type Basic Name foo  Eye_point 0.8 }"), ParserException);
    EXPECT_THROW(parser.ParseScene("Camera { Type Basic Name foo  Look_point 0.8 }"), ParserException);
    EXPECT_THROW(parser.ParseScene("Camera { Type Basic Name foo  up 0.8 }"), ParserException);
}

TEST(SceneParserCamera, testUnknownKeyword) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_THROW(parser.ParseScene("Camera { Type Basic Name foo  unknown_key foo }"), ParserException);
}

TEST(SceneParserMaterial, defaultMaterialParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    
    EXPECT_CALL(generator, Material(_, _));
    bool success = parser.ParseScene("Material { Type basic Name foo }");
    ASSERT_TRUE(success);
    SBasicMaterial* mat = generator.GetBasicMaterial();
    EXPECT_TRUE(mat != nullptr);
    ASSERT_EQ(SMaterial::BASIC, mat->type);
    ASSERT_STREQ("foo", mat->name.c_str());
    
    // Check defaults
    ASSERT_EQ(mat->emission, Vector3d(0, 0, 0));
    ASSERT_EQ(mat->ambient, Vector3d(0, 0, 0));
    ASSERT_EQ(mat->diffuse, Vector3d(0, 0, 0));
    ASSERT_EQ(mat->specular, Vector3d(0, 0, 0));
    ASSERT_EQ(0, mat->specularPower);
}

TEST(SceneParserCamera, testMaterialParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Material(_, _));
    istringstream scene( "Material {\n"
    "    Type Basic\n"
    "    Name first_material\n"
    "    Emission 0.2 0.3 0.6 \n"
    "    Ambient .3 0.1 -0.7 \n"
    "    Diffuse 0.2 -0.1 0 \n"
    "    specular 0.2 -0.1 0 \n"
    "    specular_power 7 \n"
    "    texture foo \n"
    " }");
  
    bool success = parser.ParseScene(scene);
    ASSERT_TRUE(success);    
    SBasicMaterial* mat = generator.GetBasicMaterial();
    EXPECT_TRUE(mat != nullptr);
    ASSERT_EQ(SMaterial::BASIC, mat->type);
    ASSERT_STREQ("first_material", mat->name.c_str());
    
    ASSERT_EQ(mat->emission, Vector3d(0.2, 0.3, 0.6));
    ASSERT_EQ(mat->ambient, Vector3d(0.3, 0.1, -0.7));
    ASSERT_EQ(mat->diffuse, Vector3d(0.2, -0.1, 0));
    ASSERT_EQ(mat->specular, Vector3d(0.2, -0.1, 0));
    ASSERT_EQ(7, mat->specularPower);
    ASSERT_STREQ("foo", mat->texture.c_str());
}

TEST(SceneParserLight, defaultPointLightParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Light(_, _));
    bool success = parser.ParseScene("Light { Type POINT Name foo }");
    ASSERT_TRUE(success);
    SPointLight* light = generator.GetPointLight();
    EXPECT_TRUE(light != nullptr);
    ASSERT_EQ(SLight::POINT, light->type);
    ASSERT_STREQ("foo", light->name.c_str());
    
    // Check defaults
    ASSERT_EQ(light->ambient, Vector3d(0, 0, 0));
    ASSERT_EQ(light->diffuse, Vector3d(0, 0, 0));
    ASSERT_EQ(light->specular, Vector3d(0, 0, 0));
    ASSERT_EQ(light->position, Vector3d(0, 0, 0));
    ASSERT_EQ(light->attenuationCoefs, Vector3d(1, 0, 0));
}

TEST(SceneParserLight, testPointLightParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Light(_, _));
    bool success = parser.ParseScene("Light { "
    "Type POINT Name foo "
    "ambient 1 0 0 diffuse 0 1 0 specular 0 0 1 "
    "position 1 0 0 attenuation_coefs 1 0 1 }");
    ASSERT_TRUE(success);
    SPointLight* light = generator.GetPointLight();
    EXPECT_TRUE(light != nullptr);
    
    ASSERT_EQ(SLight::POINT, light->type);
    ASSERT_STREQ("foo", light->name.c_str());
    
    ASSERT_EQ(light->ambient, Vector3d(1, 0, 0));
    ASSERT_EQ(light->diffuse, Vector3d(0, 1, 0));
    ASSERT_EQ(light->specular, Vector3d(0, 0, 1));
    ASSERT_EQ(light->position, Vector3d(1, 0, 0));
    ASSERT_EQ(light->attenuationCoefs, Vector3d(1, 0, 1));
 }

TEST(SceneParserLight, defaultDirectionalLightParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Light(_, _));

    bool success = parser.ParseScene("Light { Type DIRECTIONAL Name foo }");
    ASSERT_TRUE(success);

    SDirectionalLight *light = generator.GetDirectionalLight();
    EXPECT_TRUE(light != nullptr);
    
    ASSERT_EQ(SLight::DIRECTIONAL, light->type);
    ASSERT_STREQ("foo", light->name.c_str());
    
    // Check defaults, same
   ASSERT_EQ(light->direction, Vector3d(0, 0, 0));
}

TEST(SceneParserLight, testDirectionalLightParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Light(_, _));

    bool success = parser.ParseScene("Light { Type DIRECTIONAL Name foo direction -1 1 0 }");
    ASSERT_TRUE(success);

    SDirectionalLight *light = generator.GetDirectionalLight();
    EXPECT_TRUE(light != nullptr);
    ASSERT_EQ(light->direction, Vector3d(-1, 1, 0));
}

TEST(SceneParserLight, unknownLightTypeWillThrow) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    SDirectionalLight light("");
    EXPECT_THROW(parser.ParseScene("Light { Type unknown Name foo }"), ParserException);
}

TEST(SceneParserGeometry, sphereGeometry) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Geometry(_, _));
    bool success = parser.ParseScene("Geometry { type sphere name foo }");
    ASSERT_TRUE(success);
    SSphere* sphere = generator.GetSphere();
    EXPECT_TRUE(sphere != nullptr);
}

TEST(SceneParserGeometry, meshGeometry) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Geometry(_, _));
    bool success = parser.ParseScene("Geometry { type mesh name foo "
    "vertices { p -1 1 0 N -1 0 1 tc 0 0 0 } { p -1 1 0 N 0 0 1 tc 0 0 0 } { p -1 1 0 N 0 -1 1 tc 0 0 0 } \n"
    "tri 0 1 2 }");
    ASSERT_TRUE(success);
    SMesh* mesh = generator.GetMesh();
    EXPECT_TRUE(mesh != nullptr);
    ASSERT_EQ(mesh->vertices[0].p, Vector3d(-1, 1, 0));
    ASSERT_EQ(mesh->vertices[0].n,  Vector3d(-1, 0, 1));
    ASSERT_EQ(mesh->vertices[0].tc,  Vector3d(0, 0, 0));
    
    ASSERT_EQ(mesh->vertices[1].p,  Vector3d(-1, 1, 0));
    ASSERT_EQ(mesh->vertices[1].n,  Vector3d(0, 0, 1));
    ASSERT_EQ(mesh->vertices[1].tc,  Vector3d(0, 0, 0));
    
    ASSERT_EQ(mesh->vertices[2].p,  Vector3d(-1, 1, 0));
    ASSERT_EQ(mesh->vertices[2].n,  Vector3d(0, -1, 1));
    ASSERT_EQ(mesh->vertices[2].tc,  Vector3d(0, 0, 0));
    
    ASSERT_THAT(mesh->tri[0], ElementsAre(0, 1, 2));
}

TEST(SceneParserGeometry, objectInstance) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Object(_,_));
    bool success = parser.ParseScene("object { name obj geometry foo material bar }");
    ASSERT_TRUE(success);
    SObject* obj = generator.GetObject();
    EXPECT_TRUE(obj != nullptr);
    ASSERT_STREQ("obj", obj->name.c_str());
    ASSERT_STREQ("foo", obj->geometry.c_str());
    ASSERT_STREQ("bar", obj->material.c_str());

}

TEST(SceneParserGeometry, miscBlock) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Misc(_, _));
    bool success = parser.ParseScene("Misc { }");    
    ASSERT_TRUE(success);
}

TEST(SceneTransform, basicTransf) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    bool success = parser.ParseScene("push_transform");    
    ASSERT_TRUE(success);
}

TEST(SceneTransform, basicTranslateTransf) {
   NiceMock< TestSceneGenerator> generator;
    CSceneParser parser(generator);

    Matrix4d tmp;
    tmp << 1, 0, 0, 2,
                   0, 1, 0, 3,
                   0, 0, 1, 4,
                   0, 0, 0, 1;

    bool success = parser.ParseScene("translate 2 3 4 misc { } ");    
    ASSERT_TRUE(success);
    auto mat = generator.GetTransform();
    ASSERT_TRUE(mat.isApprox(tmp));
}

TEST(SceneTransform, basicScaleTransf) {
   NiceMock< TestSceneGenerator> generator;
    CSceneParser parser(generator);

    Matrix4d tmp;
    tmp << 2, 0, 0, 0,
                   0, 3, 0, 0,
                   0, 0, 4, 0,
                   0, 0, 0, 1;

    bool success = parser.ParseScene("scale 2 3 4 misc { } ");    
    ASSERT_TRUE(success);
    auto mat = generator.GetTransform();
    ASSERT_TRUE(mat.isApprox(tmp));
}

TEST(SceneTransform, basicRotateTransf) {
   NiceMock< TestSceneGenerator> generator;
    CSceneParser parser(generator);

    Matrix4d tmp;
    tmp << 0, -1, 0, 0,
                   1, 0, 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1;

    bool success = parser.ParseScene("rotate 0 0 1 90  misc { } ");    
    ASSERT_TRUE(success);
    auto mat = generator.GetTransform();
    ASSERT_TRUE(mat.isApprox(tmp));
}

TEST(SceneTransform, basicTransformTransf) {
   NiceMock< TestSceneGenerator> generator;
    CSceneParser parser(generator);

    Matrix4d tmp;
    tmp <<  1, -2, -3, 0,
                   2,  2, -3, 0,
                   3, 3, 4, 0,
                    0,   0, 1, 1;

    bool success = parser.ParseScene("transform 1 2 3 0 -2 2 3 0 -3 -3 4 1 misc { } ");    
    ASSERT_TRUE(success);
    auto mat = generator.GetTransform();
    ASSERT_TRUE(mat.isApprox(tmp));
}

TEST(SceneTransform, basicLoadIdentityTransf) {
   NiceMock< TestSceneGenerator> generator;
    CSceneParser parser(generator);

    bool success = parser.ParseScene("transform 1 2 3 0 -2 2 3 0 -3 -3 4 1 load_identity misc { } ");    
    ASSERT_TRUE(success);
    auto mat = generator.GetTransform();
    ASSERT_TRUE(mat.isApprox(Matrix4d::Identity()));
}

TEST(SceneTransform, basicPushTransf) {
   NiceMock< TestSceneGenerator> generator;
    CSceneParser parser(generator);

    bool success = parser.ParseScene("scale 2 2 2 push_transform misc { } ");    
    ASSERT_TRUE(success);
    auto mat = generator.GetTransform();
    ASSERT_TRUE(mat.isApprox(Matrix4d::Identity()));
}

TEST(SceneTransform, basicPopTransf) {
   NiceMock< TestSceneGenerator> generator;
    CSceneParser parser(generator);

    bool success = parser.ParseScene("push_transform scale 2 2 2 pop_transform misc { } ");    
    ASSERT_TRUE(success);
    auto mat = generator.GetTransform();
    ASSERT_TRUE(mat.isApprox(Matrix4d::Identity()));
}

TEST(SceneGeometry, multiTri) {
   NiceMock< TestSceneGenerator> generator;
    CSceneParser parser(generator);

    bool success = parser.ParseScene(" Geometry { type mesh name foo tri 0 1 2 tri 1 2 3 } ");    
    ASSERT_TRUE(success);
    auto mesh = generator.GetMesh();
    ASSERT_THAT(mesh->tri[0], ElementsAre(0, 1, 2));
    ASSERT_THAT(mesh->tri[1], ElementsAre(1, 2, 3));
}