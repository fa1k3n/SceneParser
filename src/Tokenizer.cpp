#include "Tokenizer.hpp"
#include <algorithm>

CTokenizer::CTokenizer(std::string const &tokstr) {
    std::istringstream buffer(tokstr);
    getLexemesFromStream(buffer);
}

CTokenizer::CTokenizer(std::istream& tokstream) {
    getLexemesFromStream(tokstream);
}

void CTokenizer::getLexemesFromStream(std::istream& tokstream) {
     char c;
     std::string str;
     while(tokstream.get(c)) {
         // Skip whitespaces
         if(c == ' ')
            continue;
         tokstream.unget();

         if(c == '/' && ((char)tokstream.peek() == '/' || (char)tokstream.peek() == '*')) {
              skipComment(tokstream);
             continue;
         }
         
         // Get the word
         std::string word;
         tokstream >> word;
         std::transform(word.begin(), word.end(), word.begin(), ::tolower);
         
         m_lexemes.push_back(word);
     }
 }
 
bool CTokenizer::skipComment(std::istream& tokstream) {
    std::string tmp;
    std::string comment(2, ' ');
    char c;
    tokstream.read(&comment[0], 2);

   
    if(comment == "//") std::getline(tokstream, tmp); // read whole line
    else if (comment == "/*") {
       // Block comment
        tokstream.ignore(1024, '*');
        tokstream.get(c);
        if(c != '/') {
            // Block comment not closed
            return false;
        }
    }
    return true;
}

bool CTokenizer::getNextToken(std::unique_ptr<SToken>* tok) {
    if(m_lexemes.size() == 0)
        return false;
    
    auto value = m_lexemes.front(); m_lexemes.pop_front();
    return tokenizeLexeme(value, tok);
}

 SToken::TokenType CTokenizer::peekNextToken() {
    auto value = m_lexemes.front(); 
    std::unique_ptr<SToken> tok;
    if(!tokenizeLexeme(value, &tok)) return SToken::TokenType::NONE;
    return tok.get()->type;
}

bool CTokenizer::tokenizeLexeme(std::string lexeme, std::unique_ptr<SToken>* tok) {
    // Try to convert to double
    try {
        double val = std::stod(lexeme);
        *tok = std::unique_ptr<SConstToken>(new SConstToken(val));
        return true;
    } catch (std::invalid_argument& ) {}
    
    if(lexeme == "{" || lexeme == "}")
        *tok =  std::unique_ptr<SSymToken>(new SSymToken(lexeme));
    else if(isKeyword(lexeme)) {
        *tok =  std::unique_ptr<SKeywordToken>(new SKeywordToken(lexeme));
    } else
        *tok =  std::unique_ptr<SIdToken>(new SIdToken(lexeme));
    return true;
}

bool CTokenizer::isKeyword(std::string lexeme) {
    return std::find(keywords.begin(), keywords.end(), lexeme) != keywords.end();
    //return lexeme == "camera";
}

CTokenizer::~CTokenizer() {
}


