#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>

#include <SceneParser.hpp>
#include <ISceneGenerator.hpp>
using namespace std;

class TestSceneGenerator : public ISceneGenerator {
public:
    MOCK_METHOD1(Camera, bool(SCamera&));   
    MOCK_METHOD1(Material, bool(SMaterial&));
    MOCK_METHOD1(Light, bool(SLight&));
    MOCK_METHOD1(Geometry, bool(SGeometry&));
    MOCK_METHOD1(Object, bool(SObject&));
    MOCK_METHOD1(Misc, bool(SMisc&));

    TestSceneGenerator() {
        ON_CALL(*this, Light(::testing::_)).WillByDefault(::testing::Invoke(this, &TestSceneGenerator::SaveLight));
        ON_CALL(*this, Camera(::testing::_)).WillByDefault(::testing::Invoke(this, &TestSceneGenerator::SaveCamera));
        ON_CALL(*this, Material(::testing::_)).WillByDefault(::testing::Invoke(this, &TestSceneGenerator::SaveMaterial));
        ON_CALL(*this, Geometry(::testing::_)).WillByDefault(::testing::Invoke(this, &TestSceneGenerator::SaveGeometry));
        ON_CALL(*this, Object(::testing::_)).WillByDefault(::testing::Invoke(this, &TestSceneGenerator::SaveObject));
        ON_CALL(*this, Misc(::testing::_)).WillByDefault(::testing::Return(true));
    }
     ~TestSceneGenerator() {
         if(light) delete light;
         if(material) delete material;
         if(camera) delete camera;
         if(geometry) delete geometry;
         if(object) delete object;
     }
     
    bool SaveLight(SLight& l) { 
        if(l.type() == SLight::DIRECTIONAL) light = new SDirectionalLight(*static_cast<SDirectionalLight*>(&l));
        else if(l.type() == SLight::POINT) light = new SPointLight(*static_cast<SPointLight*>(&l));
        return true;
    }
    
    bool SaveMaterial(SMaterial &mat) {
        if(mat.type() == SMaterial::BASIC) material = new SBasicMaterial(*static_cast<SBasicMaterial*>(&mat));
        return true;
    }
    
    bool SaveObject(SObject& obj) {
        object = new SObject(obj);
        return true;
    }
    
    bool SaveCamera(SCamera& cam) {
         if(cam.type() == SCamera::BASIC) camera = new SBasicCamera(*static_cast<SBasicCamera*>(&cam));
         else if(cam.type() == SCamera::ADVANCED) camera = new SAdvancedCamera(*static_cast<SAdvancedCamera*>(&cam));
         return true;
    }
    
    bool SaveGeometry(SGeometry& geom) {
        if(geom.type() == SGeometry::SPHERE) geometry = new SSphere(*static_cast<SSphere*>(&geom));
        else if(geom.type() == SGeometry::MESH) geometry = new SMesh(*static_cast<SMesh*>(&geom));
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

    SLight* light = nullptr; 
    SMaterial* material = nullptr;
    SCamera* camera = nullptr;
    SGeometry* geometry = nullptr;
    SObject* object = nullptr;
};

TEST(SceneParser, testConstructor) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    ASSERT_TRUE(true);
}

bool equal(const std::vector<double>& a, const std::vector<double>& b) {
    if(a.size() != b.size()) return false;
    for(unsigned int i = 0; i <a.size(); i++)
        if(a[i] != b[i]) return false;
    return true;
}

TEST(SceneParserCamera, defaultCameraParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
   EXPECT_CALL(generator, Camera(::testing::_));
   istringstream scene( "Camera {\n"
    "    Type Basic\n"
    "    Name first_camera\n"
    "}");
    bool success = parser.ParseScene(scene);
    ASSERT_TRUE(success);
    SBasicCamera* cam = generator.GetBasicCamera();
    EXPECT_TRUE(cam != nullptr);
    
    // Check defaults
    ASSERT_TRUE(equal(cam->eyePoint.toVector(), {0, 0, -1}));
    ASSERT_TRUE(equal(cam->lookPoint.toVector(), {0, 0, 0}));
    ASSERT_TRUE(equal(cam->up.toVector(), {0, 1, 0}));
    ASSERT_EQ(3, cam->distanceImagePlane.get());
    ASSERT_EQ(90, cam->fov.get());
    ASSERT_EQ(4.0/3.0, cam->aspectRatio.get());
}

TEST(SceneParserCamera, testCameraParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Camera(::testing::_));
    istringstream scene( "Camera {\n"
    "    Type Basic\n"
    "    Name first_camera\n"
    "    Eye_point 0.2 0.3 0.6 \n"
    "    Look_point .3 0.1 -0.7 \n"
    "    Up 0.2 -0.1 0 \n"
    "    Distance_image_plane 7 \n"
    "    fov 45  \n"
    "    aspect_ratio 0.3 \n"
    " }");
  
    bool success = parser.ParseScene(scene);
    ASSERT_TRUE(success);    
    SBasicCamera* cam = generator.GetBasicCamera();
    EXPECT_TRUE(cam != nullptr);
    
    ASSERT_EQ(SCamera::BASIC, cam->type.get());
    ASSERT_STREQ("first_camera", cam->name.get().c_str());
    
    ASSERT_TRUE(equal(cam->eyePoint.toVector(), {0.2, 0.3, 0.6}));
    ASSERT_TRUE(equal(cam->lookPoint.toVector(), {0.3, 0.1, -0.7}));
    ASSERT_TRUE(equal(cam->up.toVector(), {0.2, -0.1, 0}));
    ASSERT_EQ(7, cam->distanceImagePlane.get());
    ASSERT_EQ(45, cam->fov.get());
    ASSERT_EQ(0.3, cam->aspectRatio.get());
}

TEST(SceneParserCamera, advancedCameraDefaultParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
   EXPECT_CALL(generator, Camera(::testing::_));
   istringstream scene( "Camera {\n"
    "    Type Advanced\n"
    "    Name first_camera\n"
    "}");
    bool success = parser.ParseScene(scene);
    ASSERT_TRUE(success);
    SAdvancedCamera* cam = generator.GetAdvancedCamera();
    EXPECT_TRUE(cam != nullptr);
    
    // Check defaults
    ASSERT_EQ(1, cam->left.get());
    ASSERT_EQ(1, cam->right.get());
    ASSERT_EQ(1, cam->top.get());
    ASSERT_EQ(1, cam->bottom.get());
}

TEST(SceneParserCamera, advancedCameraParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
   EXPECT_CALL(generator, Camera(::testing::_));
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
    ASSERT_EQ(2, cam->left.get());
    ASSERT_EQ(3, cam->right.get());
    ASSERT_EQ(4, cam->top.get());
    ASSERT_EQ(5, cam->bottom.get());
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
    EXPECT_THROW(parser.ParseScene("Camera { Type Basic Name foo  Eye_point 0.8 }"), PropertyException);
    EXPECT_THROW(parser.ParseScene("Camera { Type Basic Name foo  Look_point 0.8 }"), PropertyException);
    EXPECT_THROW(parser.ParseScene("Camera { Type Basic Name foo  up 0.8 }"), PropertyException);
}

TEST(SceneParserCamera, testUnknownKeyword) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_THROW(parser.ParseScene("Camera { Type Basic Name foo  unknown_key foo }"), ParserException);
}

TEST(SceneParserMaterial, defaultMaterialParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    
    EXPECT_CALL(generator, Material(::testing::_));
    bool success = parser.ParseScene("Material { Type basic Name foo }");
    ASSERT_TRUE(success);
    SBasicMaterial* mat = generator.GetBasicMaterial();
    EXPECT_TRUE(mat != nullptr);
    ASSERT_EQ(SMaterial::BASIC, mat->type());
    ASSERT_STREQ("foo", mat->name().c_str());
    
    // Check defaults
    ASSERT_TRUE(equal(mat->emission.toVector(), {0, 0, 0}));
    ASSERT_TRUE(equal(mat->ambient.toVector(), {0, 0, 0}));
    ASSERT_TRUE(equal(mat->diffuse.toVector(), {0, 0, 0}));
    ASSERT_TRUE(equal(mat->specular.toVector(), {0, 0, 0}));
    ASSERT_EQ(0, mat->specularPower());
}

TEST(SceneParserCamera, testMaterialParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Material(::testing::_));
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
    ASSERT_EQ(SMaterial::BASIC, mat->type.get());
    ASSERT_STREQ("first_material", mat->name.get().c_str());
    
    ASSERT_TRUE(equal(mat->emission.toVector(), {0.2, 0.3, 0.6}));
    ASSERT_TRUE(equal(mat->ambient.toVector(), {0.3, 0.1, -0.7}));
    ASSERT_TRUE(equal(mat->diffuse.toVector(), {0.2, -0.1, 0}));
    ASSERT_TRUE(equal(mat->specular.toVector(), {0.2, -0.1, 0}));
    ASSERT_EQ(7, mat->specularPower.get());
    ASSERT_STREQ("foo", mat->texture().c_str());
}

TEST(SceneParserLight, defaultPointLightParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Light(::testing::_));
    bool success = parser.ParseScene("Light { Type POINT Name foo }");
    ASSERT_TRUE(success);
    SPointLight* light = generator.GetPointLight();
    EXPECT_TRUE(light != nullptr);
    ASSERT_EQ(SLight::POINT, light->type());
    ASSERT_STREQ("foo", light->name().c_str());
    
    // Check defaults
    ASSERT_TRUE(equal(light->ambient.toVector(), {0, 0, 0}));
    ASSERT_TRUE(equal(light->diffuse.toVector(), {0, 0, 0}));
    ASSERT_TRUE(equal(light->specular.toVector(), {0, 0, 0}));
    ASSERT_TRUE(equal(light->position.toVector(), {0, 0, 0}));
    ASSERT_TRUE(equal(light->attenuationCoefs.toVector(), {1, 0, 0}));
}

TEST(SceneParserLight, testPointLightParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Light(::testing::_));
    bool success = parser.ParseScene("Light { "
    "Type POINT Name foo "
    "ambient 1 0 0 diffuse 0 1 0 specular 0 0 1 "
    "position 1 0 0 attenuation_coefs 1 0 1 }");
    ASSERT_TRUE(success);
    SPointLight* light = generator.GetPointLight();
    EXPECT_TRUE(light != nullptr);
    
    ASSERT_EQ(SLight::POINT, light->type());
    ASSERT_STREQ("foo", light->name().c_str());
    
    ASSERT_TRUE(equal(light->ambient.toVector(), {1, 0, 0}));
    ASSERT_TRUE(equal(light->diffuse.toVector(), {0, 1, 0}));
    ASSERT_TRUE(equal(light->specular.toVector(), {0, 0, 1}));
    ASSERT_TRUE(equal(light->position.toVector(), {1, 0, 0}));
    ASSERT_TRUE(equal(light->attenuationCoefs.toVector(), {1, 0, 0}));
 }

TEST(SceneParserLight, defaultDirectionalLightParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Light(::testing::_));

    bool success = parser.ParseScene("Light { Type DIRECTIONAL Name foo }");
    ASSERT_TRUE(success);

    SDirectionalLight *light = generator.GetDirectionalLight();
    EXPECT_TRUE(light != nullptr);
    
    ASSERT_EQ(SLight::DIRECTIONAL, light->type());
    ASSERT_STREQ("foo", light->name().c_str());
    
    // Check defaults, same
   ASSERT_TRUE(equal(light->direction.toVector(), {0, 0, 0}));
}

TEST(SceneParserLight, testDirectionalLightParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Light(::testing::_));

    bool success = parser.ParseScene("Light { Type DIRECTIONAL Name foo direction -1 1 0 }");
    ASSERT_TRUE(success);

    SDirectionalLight *light = generator.GetDirectionalLight();
    EXPECT_TRUE(light != nullptr);
    ASSERT_TRUE(equal(light->direction.toVector(), {-1, 1, 0}));
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
    EXPECT_CALL(generator, Geometry(::testing::_));
    bool success = parser.ParseScene("Geometry { type sphere name foo }");
    ASSERT_TRUE(success);
    SSphere* sphere = generator.GetSphere();
    EXPECT_TRUE(sphere != nullptr);
}

TEST(SceneParserGeometry, meshGeometry) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Geometry(::testing::_));
    bool success = parser.ParseScene("Geometry { type mesh name foo "
    "vertices { p -1 1 0 N 0 0 1 tc 0 0 0 } { p -1 1 0 N 0 0 1 tc 0 0 0 } { p -1 1 0 N 0 0 1 tc 0 0 0 } \n"
    "tri 0 1 2 }");
    ASSERT_TRUE(success);
    SMesh* mesh = generator.GetMesh();
    EXPECT_TRUE(mesh != nullptr);
    ASSERT_TRUE(equal(mesh->tri.toVector(), {0, 1, 2}));
}

TEST(SceneParserGeometry, objectInstance) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Object(::testing::_));
    bool success = parser.ParseScene("object { name obj geometry foo material bar }");
    ASSERT_TRUE(success);
    SObject* obj = generator.GetObject();
    EXPECT_TRUE(obj != nullptr);
    ASSERT_STREQ("obj", obj->name().c_str());
    ASSERT_STREQ("foo", obj->geometry().c_str());
    ASSERT_STREQ("bar", obj->material().c_str());

}

TEST(SceneParserGeometry, miscBlock) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Misc(::testing::_));
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
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Object(::testing::_));

    bool success = parser.ParseScene("translate 2 2 2 object { name obj geometry foo material bar } ");    
    ASSERT_TRUE(success);
    SObject* obj = generator.GetObject();
    EXPECT_TRUE(obj != nullptr);
}