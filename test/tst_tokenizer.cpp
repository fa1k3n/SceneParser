#include <gtest/gtest.h>
#include <string>

#include <Tokenizer.hpp>

using namespace std;

TEST(Tokenizer, basicId) { 
    CTokenizer tokenizer("foo bar");
    const char* exp[] = {"foo", "bar"};
    std::unique_ptr<SToken> tok;
    int i = 0;
    while(tokenizer.getNextToken(&tok)) {
        ASSERT_TRUE(tok->type == SToken::ID);
        auto idt = static_cast<SIdToken*>(tok.get());
        ASSERT_STREQ(exp[i++], idt->str.c_str());
    }
}

TEST(Tokenizer, basicBlock) {
    CTokenizer tokenizer("   { }");
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    ASSERT_TRUE(tok->type == SToken::SYM);
    tokenizer.getNextToken(&tok);
    ASSERT_TRUE(tok->type == SToken::SYM);
}

TEST(Tokenizer, basicConst) {
    CTokenizer tokenizer("2 .5 0.7");
    double exp[] = {2.0, 0.5, 0.7};
    std::unique_ptr<SToken> tok;
    int i = 0;
    while(tokenizer.getNextToken(&tok)) {
        ASSERT_TRUE(tok->type == SToken::CONST);
        auto idt = static_cast<SConstToken*>(tok.get());
        ASSERT_EQ(exp[i++], idt->val);
    }
}

TEST(Tokenizer, basicTransform) {
    CTokenizer tokenizer("push_transform");
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    ASSERT_TRUE(tok->type == SToken::TRANSF);        
    auto tt = static_cast<STransformToken*>(tok.get());
    ASSERT_STREQ("push", tt->str.c_str());
}

TEST(Tokenizer, basicStream) {
    std::string str = "baz";
    std::istringstream tokstream(str);
    CTokenizer t(tokstream);
    std::unique_ptr<SToken> tok;
    t.getNextToken(&tok);
    ASSERT_TRUE(tok->type == SToken::ID);
    auto idt = static_cast<SIdToken*>(tok.get());
    ASSERT_STREQ("baz", idt->str.c_str());
}

TEST(Tokenizer, commentsAreSkipped) {
    {
        CTokenizer toks("/* everything in here is a comment */  ");
        std::unique_ptr<SToken> tok;
        ASSERT_FALSE(toks.getNextToken(&tok));
    }
    
    {
        CTokenizer toks("// This is a row comment\nfoo");
        std::unique_ptr<SToken> tok;
        ASSERT_TRUE(toks.getNextToken(&tok));
         auto idt = static_cast<SIdToken*>(tok.get());
         ASSERT_STREQ("foo", idt->str.c_str());
    }  
}

TEST(Tokenizer, allValuesAreSmall) { 
    CTokenizer tokenizer("FoO BAR");
    const char* exp[] = {"foo", "bar"};
    std::unique_ptr<SToken> tok;
    int i = 0;
    while(tokenizer.getNextToken(&tok)) {
        ASSERT_TRUE(tok->type == SToken::ID);
        auto idt = static_cast<SIdToken*>(tok.get());
        ASSERT_STREQ(exp[i++], idt->str.c_str());
    }
}

TEST(Tokenizer, tabsAreHandeled) { 
    CTokenizer tokenizer("foo           bar");
    const char* exp[] = {"foo", "bar"};
    std::unique_ptr<SToken> tok;
    int i = 0;
    while(tokenizer.getNextToken(&tok)) {
        ASSERT_TRUE(tok->type == SToken::ID);
        auto idt = static_cast<SIdToken*>(tok.get());
        ASSERT_STREQ(exp[i++], idt->str.c_str());
    }
}

TEST(Tokenizer, testPeek) {
    CTokenizer tokenizer("foo");
    ASSERT_EQ(SToken::ID, tokenizer.peekNextToken());
    ASSERT_EQ(SToken::ID, tokenizer.peekNextToken());
}
