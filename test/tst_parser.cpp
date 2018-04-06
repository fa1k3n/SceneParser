#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>

#include <SceneParser.hpp>
#include <ISceneGenerator.hpp>

using namespace std;

class TestSceneGenerator : public ISceneGenerator {
public:
    MOCK_METHOD0(Camera, bool());
};

TEST(SceneParser, testConstructor) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    ASSERT_TRUE(true);
}

TEST(SceneParserCamera, testBasicCameraParsing) {
    TestSceneGenerator generator;
    CSceneParser parser(generator);
    EXPECT_CALL(generator, Camera()).WillRepeatedly(::testing::Return(true));
    istringstream scene("Camera { Type Basic\nName first_camera}");
    bool success = parser.ParseScene(scene);
    ASSERT_TRUE(success);
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