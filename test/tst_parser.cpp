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

TEST(SceneParserCamera, testBasicCameraParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    CCamera cam(CCamera::BASIC,"");
    EXPECT_CALL(generator, Camera(::testing::_)).WillOnce(::testing::SaveArg<0>(&cam)).WillRepeatedly(::testing::Return(true));
    istringstream scene( "Camera {"
    "    Type Basic"
    "    Eye_point 0.5 0.5 0.5"
    "    Name first_camera"
    " }");
  
    bool success = parser.ParseScene(scene);
    ASSERT_TRUE(success);
    ASSERT_STREQ("first_camera", cam.name.c_str());
}

/*
TEST(SceneParserCamera, testBasicCameraParsingMissingRequiredKey) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Camera()).WillRepeatedly(::testing::Return(true));
    istringstream scene("Camera { Type Basic }");
    bool success = parser.ParseScene(scene);
    ASSERT_FALSE(success);
}
*/