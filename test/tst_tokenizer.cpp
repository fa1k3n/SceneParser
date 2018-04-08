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
        ASSERT_TRUE(tok->type == SToken::KEYWORD);
        auto idt = static_cast<SKeywordToken*>(tok.get());
        ASSERT_STREQ(exp[i++], idt->str.c_str());
    }
}

TEST(Tokenizer, basicBlock) {
    CTokenizer tokenizer("   { }");
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    ASSERT_TRUE(tok->type == SToken::SEPARATOR);
    tokenizer.getNextToken(&tok);
    ASSERT_TRUE(tok->type == SToken::SEPARATOR);
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

TEST(Tokenizer, basicStream) {
    std::string str = "baz";
    std::istringstream tokstream(str);
    CTokenizer t(tokstream);
    std::unique_ptr<SToken> tok;
    t.getNextToken(&tok);
    ASSERT_TRUE(tok->type == SToken::KEYWORD);
    auto idt = static_cast<SKeywordToken*>(tok.get());
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
         auto idt = static_cast<SKeywordToken*>(tok.get());
         ASSERT_STREQ("foo", idt->str.c_str());
    }  
}

TEST(Tokenizer, allValuesAreSmall) { 
    CTokenizer tokenizer("FoO BAR");
    const char* exp[] = {"foo", "bar"};
    std::unique_ptr<SToken> tok;
    int i = 0;
    while(tokenizer.getNextToken(&tok)) {
        ASSERT_TRUE(tok->type == SToken::KEYWORD);
        auto idt = static_cast<SKeywordToken*>(tok.get());
        ASSERT_STREQ(exp[i++], idt->str.c_str());
    }
}

TEST(Tokenizer, tabsAreHandeled) { 
    CTokenizer tokenizer("foo           bar");
    const char* exp[] = {"foo", "bar"};
    std::unique_ptr<SToken> tok;
    int i = 0;
    while(tokenizer.getNextToken(&tok)) {
        ASSERT_TRUE(tok->type == SToken::KEYWORD);
        auto idt = static_cast<SKeywordToken*>(tok.get());
        ASSERT_STREQ(exp[i++], idt->str.c_str());
    }
}

TEST(Tokenizer, testPeek) {
    CTokenizer tokenizer("foo");
    ASSERT_EQ(SToken::KEYWORD, tokenizer.peekNextToken());
    ASSERT_EQ(SToken::KEYWORD, tokenizer.peekNextToken());
}
