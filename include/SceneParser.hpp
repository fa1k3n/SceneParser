#ifndef SCENEPARSER_HPP
#define SCENEPARSER_HPP

#include <ISceneGenerator.hpp>
#include <../src/PrivTypes.hpp>
#include <istream>
#include <../src/Tokenizer.hpp>
#include <vector>
#include <stack>
#include <set>

class CSceneParser {
public:
    explicit CSceneParser(ISceneGenerator& generator);
    bool ParseScene(std::istream& scene);
    bool ParseScene(std::string scene);
private:
    bool parseCamera(CTokenizer& tokenizer, CPropertyMap& properties);
    bool parseMaterial(CTokenizer& tokenizer, CPropertyMap& properties);
    bool parseLight(CTokenizer& tokenizer, CPropertyMap& properties);
    bool parseGeometry(CTokenizer& tokenizer, CPropertyMap& properties);
    bool parseObject(CTokenizer& tokenizer, CPropertyMap& properties);
    bool parseMisc(CTokenizer& tokenizer, CPropertyMap& properties);

    bool checkKeywords(CPropertyMap& properties, std::set<std::string> validKeywords);
    
    SKeywordToken getKeywordToken(CTokenizer& tokenizer);
    SSymToken getSymToken(CTokenizer& tokenizer);
    SConstToken getConstToken(CTokenizer& tokenizer);
    SIdToken getIdToken(CTokenizer& tokenizer);
    STransfToken getTransfToken(CTokenizer& tokenizer);

    bool handleTransf(CTokenizer& tokenizer);

    bool readBlock(CTokenizer& tokenizer, CPropertyMap& pairs);
    bool readPropertyValue(CTokenizer& tokenizer, SPropertyValue& prop);
    
    Vector3d toVector3d(std::vector<double> dblList);
    
    template<size_t size>
    bool readConstVect(CTokenizer& tokenizer, std::vector<double>& vect) {
        for(unsigned int i = 0; i < size; i ++)
            vect.push_back(getConstToken(tokenizer).val);
        return true;
    }
    
    std::stack<Matrix4d> m_transformStack;
    ISceneGenerator& m_generator;
};

#endif /* SCENEPARSER_HPP */

