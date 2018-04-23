#include <gtest/gtest.h>
#include <PrivTypes.hpp>

TEST(SPropertyValueTest, basicDouble) {
    SPropertyValue var(3);
    ASSERT_EQ(SPropertyValue::DOUBLE, var.type);
    ASSERT_EQ(3, var.toDouble());
}

TEST(SPropertyValueTest, setvalueDouble) {
    SPropertyValue var;
    var = 3;
    ASSERT_EQ(SPropertyValue::DOUBLE, var.type);
    ASSERT_EQ(3, var.toDouble());
}

TEST(SPropertyValueTest, noCastingOfTypes) {
    SPropertyValue strProp("foo"); // String value property
    SPropertyValue doubleProp(4);
    CPropertyMap map;
    SPropertyValue mapProp(map);
    
    // Will try to cast to double which is not allowed
    EXPECT_THROW(strProp = map, ImplicitTypeConversion);
    EXPECT_THROW(doubleProp = "foo", ImplicitTypeConversion);
    EXPECT_THROW(mapProp = 3, ImplicitTypeConversion);
}

TEST(SPropertyValueTest, readWrongTypeWillThrow) {
    SPropertyValue strProp("foo"); 
    SPropertyValue doubleProp(4);
    CPropertyMap map;
    SPropertyValue mapProp(map);

    EXPECT_THROW(strProp.toDouble(), ImplicitTypeConversion);
    EXPECT_THROW(doubleProp.toStr(), ImplicitTypeConversion);
    EXPECT_THROW(mapProp.toStr(), ImplicitTypeConversion);
}

TEST(SPropertyValueTest, basicString) {
    SPropertyValue var("foo");
    ASSERT_EQ(SPropertyValue::STRING, var.type);
    ASSERT_STREQ("foo", var.toStr().c_str());
}

TEST(SPropertyValueTest, setvalueString) {
    SPropertyValue var;
    var = "foo";
    ASSERT_EQ(SPropertyValue::STRING, var.type);
    ASSERT_STREQ("foo", var.toStr().c_str());
}

TEST(SPropertyValueTest, basicPropertyMap) {
    CPropertyMap map;
    map["test"] = SPropertyValue(3.0);
    SPropertyValue var(map);
    ASSERT_EQ(SPropertyValue::MAP, var.type);
    ASSERT_EQ(map["test"].toDouble(), var.toMap()["test"].toDouble());
}

TEST(SPropertyValueTest, doubleList) {
    SPropertyValue var;
    var << 3 << 4 << 0.3;
    ASSERT_EQ(SPropertyValue::DOUBLE_LIST, var.type);
    ASSERT_EQ(3, var.toDouble(0));
    ASSERT_EQ(4, var.toDouble(1));
     ASSERT_EQ(0.3, var.toDouble(2));
}

TEST(SPropertyValueTest, stringList) {
    SPropertyValue var;
    var << "foo" << "bar";
    ASSERT_EQ(SPropertyValue::STRING_LIST, var.type);
    ASSERT_STREQ("foo", var.toStr(0).c_str());
    ASSERT_STREQ("bar", var.toStr(1).c_str());
}

TEST(SPropertyValueTest, propertyMapList) {
    SPropertyValue var;
    CPropertyMap m1, m2;
    m1["test"] = SPropertyValue(3.0);
    m2["test"] = SPropertyValue(-3.0);
    var << m1 << m2;
    ASSERT_EQ(SPropertyValue::MAP_LIST, var.type);
    ASSERT_EQ(m1["test"].toDouble(), var.toMap(0)["test"].toDouble());
    ASSERT_EQ(m2["test"].toDouble(), var.toMap(1)["test"].toDouble());
}

TEST(CPropertyMapTest, hasProperty) {
    CPropertyMap map;
    map["bar"] << 4;
    ASSERT_FALSE(map.hasProperty("foo"));
    ASSERT_TRUE(map.hasProperty("bar"));
}