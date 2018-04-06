#include <gtest/gtest.h>
#include <string>

#include <Tokenizer.hpp>

using namespace std;

TEST(Tokenizer, basicId) { 
    CTokenizer tokenizer("foo bar");
    const char* exp[] = {"foo", "bar"};
    SToken* tok;
    int i = 0;
    while(tokenizer.getNextToken(&tok)) {
        ASSERT_TRUE(tok->type == SToken::ID);
        auto idt = static_cast<SIdToken*>(tok);
        ASSERT_STREQ(exp[i++], idt->str.c_str());
        delete tok;
    }
}

TEST(Tokenizer, basicBlock) {
    CTokenizer tokenizer("{ }");
    SToken* tok;
    tokenizer.getNextToken(&tok);
    ASSERT_TRUE(tok->type == SToken::SBLOCK);
    delete tok;
     tokenizer.getNextToken(&tok);
    ASSERT_TRUE(tok->type == SToken::EBLOCK);
    delete tok;
}

TEST(Tokenizer, basicConst) {
    CTokenizer tokenizer("2 .5 0.7");
    double exp[] = {2.0, 0.5, 0.7};
    SToken* tok;
    SConstToken* idt;
    int i = 0;
    while(tokenizer.getNextToken(&tok)) {
        ASSERT_TRUE(tok->type == SToken::CONST);
        idt = static_cast<SConstToken*>(tok);
        ASSERT_EQ(exp[i++], idt->val);
        delete tok;
    }
}

