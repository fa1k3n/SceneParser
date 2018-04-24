#ifndef CTOKENIZER_HPP
#define CTOKENIZER_HPP

#include <istream>
#include <list>
#include <memory>
#include <sstream>
#include <iterator>
#include <sstream>
#include <string>

#include "Types.hpp"

struct SToken {
    enum TokenType {
        NONE,  // Uninitialized token
        KEYWORD,
        ID,
        CONST,
        SYM,
        TRANSF
    };
    
    std::string TokenStrings[TRANSF + 1] = {
        "NONE",
        "KEYWORD",
         "ID",
        "CONST",
        "SYM",
        "TRANSF"
    };
   
    SToken(TokenType tt = NONE) : type(tt) {}
    
    virtual std::string toStr() {
        return TokenStrings[type];
    }
    
    TokenType type;
};

struct SKeywordToken : public SToken {
    SKeywordToken(std::string value) : SToken(SToken::KEYWORD),  str(value) {};
    std::string str;
    
    std::string toStr() override {
        std::ostringstream oss;
        oss << "<" << TokenStrings[type] << ", " << str << ">";
        return oss.str();
    }
};

struct SIdToken : public SToken {
    SIdToken(std::string value) : SToken(SToken::ID),  str(value) {};
    std::string str;
    
    std::string toStr() override {
        std::ostringstream oss;
        oss << "<" << TokenStrings[type] << ", " << str << ">";
        return oss.str();
    }
};

struct SSymToken : public SToken {
    SSymToken(std::string value) : SToken(SToken::SYM), str(value) {};
    std::string str;
    
    std::string toStr() override {
        std::ostringstream oss;
        oss << "<" << TokenStrings[type] << ", " << str << ">";
        return oss.str();
    }
};

struct SConstToken : public SToken {
    SConstToken(double v) : SToken(SToken::CONST) , val(v) {}
    double val;
    
    std::string toStr() override {
        std::ostringstream oss;
        oss << "<" << TokenStrings[type] << ", " << val << ">";
        return oss.str();
    }
};

struct STransfToken: public SToken {
    enum TransfTypeID {
        PUSH,
        POP,
        LOAD,
        TRANSLATE,
        ROTATE,
        SCALE,
        TRANSFORM
    };
    
    STransfToken(TransfTypeID ttid) : SToken(SToken::TRANSF), id(ttid) {};
    TransfTypeID id;
    
    std::string toStr() override {
        std::ostringstream oss;
        oss << "<" << TokenStrings[type] << ", " << id << ">";
        return oss.str();
    }
};

class CTokenizer {
public:
    CTokenizer(std::string const &tokstr);
    CTokenizer(std::istream& tokstream);
    virtual ~CTokenizer();
    bool getNextToken(std::unique_ptr<SToken>* tok);
    SToken::TokenType peekNextToken();
    bool hasMoreTokens() { return m_lexemes.size(); }
    int getCurrentPos() { return numHandledTokens; }
private:
    void getLexemesFromStream(std::istream& tokstream);
    bool skipComment(std::istream& tokstream);
    bool tokenizeLexeme(std::string lexeme, std::unique_ptr<SToken>* tok);
    bool isKeyword(std::string lexeme);
    bool isTransform(std::string lexeme);

    std::list<std::string> m_lexemes;
    int numHandledTokens = 0;
    
    std::list<std::string> keywords = {
        "camera",
        "material",
        "light",
        "geometry",
        "object",
        "misc"
    };   
    
    std::list<std::string> transforms = {
        "push_transform",
        "pop_transform",
        "load_identity",
        "translate",
        "rotate",
        "scale",
        "transform"
    };

};

#endif /* CTOKENIZER_HPP */

