#ifndef CTOKENIZER_HPP
#define CTOKENIZER_HPP

#include <istream>
#include <list>
#include <memory>
#include <sstream>
#include <iterator>


struct SToken {
    enum TokenType {
        NONE,  // Uninitialized token
        ID, 
        CONST,
        BLOCK
    };
    SToken(TokenType tt = NONE) : type(tt) {}
    TokenType type;
};

struct SIdToken : public SToken {
    SIdToken(std::string value) : SToken(SToken::ID),  str(value) {};
    std::string str;
};

struct SBlockToken : public SToken {
    enum BlockType {
        START,
        END
    };
    SBlockToken(BlockType bt) : SToken(SToken::BLOCK), type(bt) {};
    BlockType type;
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
private:
    void getWordsFromStream(std::istream& tokstream);
    bool skipComment(std::istream& tokstream);
    std::list<std::string> m_words;
};

#endif /* CTOKENIZER_HPP */

