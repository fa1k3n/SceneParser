#ifndef SCENEPARSER_HPP
#define SCENEPARSER_HPP

#include <ISceneGenerator.hpp>
#include <istream>
#include <Tokenizer.hpp>
#include <vector>

class CSceneParser {
public:
    CSceneParser(ISceneGenerator& generator);
    bool ParseScene(std::istream& scene);
private:
    bool parseCamera(CTokenizer& tokenizer);
    
    SKeywordToken* getKeywordToken(CTokenizer& tokenizer);
    SSymToken* getSymToken(CTokenizer& tokenizer);
    bool readBlock(CTokenizer& tokenizer, CPropertyMap& pairs);
    bool readPropertyValue(CTokenizer& tokenizer, SPropertyValue& prop);
    bool readConstArray(CTokenizer& tokenizer, std::vector<double>& vals);
    ISceneGenerator& m_generator;
};

#endif /* SCENEPARSER_HPP */

