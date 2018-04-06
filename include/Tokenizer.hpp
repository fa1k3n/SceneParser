#ifndef CTOKENIZER_HPP
#define CTOKENIZER_HPP

#include <istream>

class CTokenizer {
public:
    CTokenizer();
    CTokenizer(std::istream& tokstream);
    virtual ~CTokenizer();
private:

};

#endif /* CTOKENIZER_HPP */

