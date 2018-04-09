#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>

#include <SceneParser.hpp>
#include <ISceneGenerator.hpp>

using namespace std;

class TestSceneGenerator : public ISceneGenerator {
public:
    MOCK_METHOD1(Camera, bool(CCamera&));
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
    CCamera cam(CCamera::BASIC,"");
    EXPECT_CALL(generator, Camera(::testing::_)).WillOnce(::testing::DoAll(::testing::SaveArg<0>(&cam), ::testing::Return(true)));
   istringstream scene( "Camera {\n"
    "    Type Basic\n"
    "    Name first_camera\n"
    "}");
    bool success = parser.ParseScene(scene);
    ASSERT_TRUE(success);
    
    // Check defaults
    ASSERT_TRUE(equal(cam.eyePoint, {0, 0, -1}));
    ASSERT_TRUE(equal(cam.lookPoint, {0, 0, 0}));
    ASSERT_TRUE(equal(cam.up, {0, 1, 0}));
    ASSERT_EQ(3, cam.distanceImagePlane);
}
TEST(SceneParserCamera, testCameraParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    CCamera cam(CCamera::BASIC,"");
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
    ASSERT_EQ(CCamera::BASIC, cam.type);
    ASSERT_STREQ("first_camera", cam.name.c_str());
    
    ASSERT_TRUE(equal(cam.eyePoint, {0.2, 0.3, 0.6}));
    ASSERT_TRUE(equal(cam.lookPoint, {0.3, 0.1, -0.7}));
    ASSERT_TRUE(equal(cam.up, {0.2, -0.1, 0}));
    ASSERT_EQ(7, cam.distanceImagePlane);
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
    istringstream scene("Camera { Name foo Type Basic }");
    EXPECT_THROW(parser.ParseScene(scene), ParserException);
}

TEST(SceneParserCamera, testArrayMissmatch) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    istringstream scene("Camera { Type Basic Name foo  Eye_point 0.8 }");
    EXPECT_THROW(parser.ParseScene(scene), ParserException);
}