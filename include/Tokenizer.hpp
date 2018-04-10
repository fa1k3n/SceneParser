#ifndef CTOKENIZER_HPP
#define CTOKENIZER_HPP

#include <istream>
#include <list>
#include <memory>
#include <sstream>
#include <iterator>

#include "Types.hpp"

struct SToken {
    enum TokenType {
        NONE,  // Uninitialized token
        KEYWORD,
        ID,
        CONST,
        SYM
    };
    SToken(TokenType tt = NONE) : type(tt) {}
    TokenType type;
};

struct SKeywordToken : public SToken {
    SKeywordToken(std::string value) : SToken(SToken::KEYWORD),  str(value) {};
    std::string str;
};

struct SIdToken : public SToken {
    SIdToken(std::string value) : SToken(SToken::ID),  str(value) {};
    std::string str;
};

struct SSymToken : public SToken {
    SSymToken(std::string value) : SToken(SToken::SYM), str(value) {};
    std::string str;
};

struct SConstToken : public SToken {
    SConstToken(double v) : SToken(SToken::CONST) , val(v) {}
    double val;
};

class CTokenizer {
public:
    CTokenizer(std::string const &tokstr);
    CTokenizer(std::istream& tokstream);
    virtual ~CTokenizer();
    bool getNextToken(std::unique_ptr<SToken>* tok);
    SToken::TokenType peekNextToken();
private:
    void getLexemesFromStream(std::istream& tokstream);
    bool skipComment(std::istream& tokstream);
    bool tokenizeLexeme(std::string lexeme, std::unique_ptr<SToken>* tok);
    bool isKeyword(std::string lexeme);

    std::list<std::string> m_lexemes;
    
    std::list<std::string> keywords = {
        "camera",
        "material",
        "light"
    };
};

#endif /* CTOKENIZER_HPP */

