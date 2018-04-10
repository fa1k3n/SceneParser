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
    SCamera cam(SCamera::BASIC,"");
    EXPECT_CALL(generator, Camera(::testing::_)).WillOnce(::testing::DoAll(::testing::SaveArg<0>(&cam), ::testing::Return(true)));
   istringstream scene( "Camera {\n"
    "    Type Basic\n"
    "    Name first_camera\n"
    "}");
    bool success = parser.ParseScene(scene);
    ASSERT_TRUE(success);
    
    // Check defaults
    ASSERT_TRUE(equal(cam.eyePoint.toVector(), {0, 0, -1}));
    ASSERT_TRUE(equal(cam.lookPoint.toVector(), {0, 0, 0}));
    ASSERT_TRUE(equal(cam.up.toVector(), {0, 1, 0}));
    ASSERT_EQ(3, cam.distanceImagePlane.get());
}

TEST(SceneParserCamera, testCameraParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    SCamera cam(SCamera::BASIC,"");
    EXPECT_CALL(generator, Camera(::testing::_)).WillOnce(::testing::DoAll(::testing::SaveArg<0>(&cam), ::testing::Return(true)));
    istringstream scene( "Camera {\n"
    "    Type Basic\n"
    "    Name first_camera\n"
    "    Eye_point 0.2 0.3 0.6 \n"
    "    Look_point .3 0.1 -0.7 \n"
    "    Up 0.2 -0.1 0 \n"
    "    Distance_image_plane 7 \n"
    " }\n");
  
    bool success = parser.ParseScene(scene);
    ASSERT_TRUE(success);    
    ASSERT_EQ(SCamera::BASIC, cam.type.get());
    ASSERT_STREQ("first_camera", cam.name.get().c_str());
    
    ASSERT_TRUE(equal(cam.eyePoint.toVector(), {0.2, 0.3, 0.6}));
    ASSERT_TRUE(equal(cam.lookPoint.toVector(), {0.3, 0.1, -0.7}));
    ASSERT_TRUE(equal(cam.up.toVector(), {0.2, -0.1, 0}));
    ASSERT_EQ(7, cam.distanceImagePlane.get());
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

TEST(SceneParserMaterial, defaultMaterialParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    SMaterial mat(SMaterial::BASIC,"");
    EXPECT_CALL(generator, Material(::testing::_)).WillOnce(::testing::DoAll(::testing::SaveArg<0>(&mat), ::testing::Return(true)));
    bool success = parser.ParseScene("Material { Type basic Name foo }");
    ASSERT_TRUE(success);
    
    ASSERT_EQ(SMaterial::BASIC, mat.type());
    ASSERT_STREQ("foo", mat.name().c_str());
    
    // Check defaults
    ASSERT_TRUE(equal(mat.emission.toVector(), {0, 0, 0}));
    ASSERT_TRUE(equal(mat.ambient.toVector(), {0, 0, 0}));
    ASSERT_TRUE(equal(mat.diffuse.toVector(), {0, 0, 0}));
    ASSERT_TRUE(equal(mat.specular.toVector(), {0, 0, 0}));
    ASSERT_EQ(0, mat.specularPower());
    ASSERT_STREQ("", mat.texture().c_str());

}