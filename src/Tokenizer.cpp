#include "Tokenizer.hpp"
#include <iterator>
#include <sstream>

CTokenizer::CTokenizer(std::string const &tokens) {
    std::istringstream buffer(tokens);
    std::copy(std::istream_iterator<std::string>(buffer), std::istream_iterator<std::string>(),std::back_inserter(m_words));
}

CTokenizer::CTokenizer(std::istream& tokstream) {
    
}

bool CTokenizer::getNextToken(SToken** tok) {
    if(m_words.size() == 0)
        return false;
    
    auto value = m_words.front(); m_words.pop_front();
    
    // Try to convert to double
    try {
        double val = std::stod(value);
        *tok = new SConstToken(val);
        return true;
    } catch (std::invalid_argument& ) {}
    
    if(value == "{")
        *tok = new SStartBlockToken();
    else if (value == "}")
        *tok = new SEndBlockToken();
    else
        *tok = new SIdToken(value);
    return true;
}

    
CTokenizer::~CTokenizer() {
}


