#ifndef SCENEPARSER_HPP
#define SCENEPARSER_HPP

#include <ISceneGenerator.hpp>
#include <istream>
#include <Tokenizer.hpp>

class CSceneParser {
public:
    CSceneParser(ISceneGenerator& generator);
    bool ParseScene(std::istream& scene);
private:
    bool parseCamera(CTokenizer& tokenizer);
    
    SKeywordToken* getKeywordToken(CTokenizer& tokenizer);
    SSeparatorToken* getSeparatorToken(CTokenizer& tokenizer);
    ISceneGenerator& m_generator;
};

#endif /* SCENEPARSER_HPP */

