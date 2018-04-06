#ifndef CTOKENIZER_HPP
#define CTOKENIZER_HPP

#include <istream>
#include <list>

struct SToken {
    enum TokenType {
        NONE,  // Uninitialized token
        ID, 
        CONST,
        SBLOCK,
        EBLOCK
    };
    SToken(TokenType tt = NONE) : type(tt) {}
    TokenType type;
};

struct SIdToken : public SToken {
    SIdToken(std::string value) : SToken(SToken::ID),  str(value) {};
    std::string str;
};

struct SStartBlockToken : public SToken {
    SStartBlockToken() : SToken(SToken::SBLOCK) {};
};

struct SEndBlockToken : public SToken {
    SEndBlockToken() : SToken(SToken::EBLOCK) {};
};

struct SConstToken : public SToken {
    SConstToken(double v) : SToken(SToken::CONST) , val(v) {}
    double val;
};

class CTokenizer {
public:
    CTokenizer(std::string const &tokens);
    CTokenizer(std::istream& tokstream);
    virtual ~CTokenizer();
    bool getNextToken(SToken** tok);
private:
    std::list<std::string> m_words;
};

#endif /* CTOKENIZER_HPP */

