#include <gtest/gtest.h>
#include <string>

#include <Tokenizer.hpp>

using namespace std;

TEST(Tokenizer, testConstructor) {
    CTokenizer tokenizer;
}

TEST(Tokenizer, basicPair) {
    istringstream stream("foo bar");
    CTokenizer tokenizer(stream);
}