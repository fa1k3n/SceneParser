#ifndef SCENEPARSER_HPP
#define SCENEPARSER_HPP

#include <ISceneGenerator.hpp>
#include <istream>
#include <Tokenizer.hpp>
#include <vector>

struct SKVPair {
    std::string key;
    std::string value;
};

class CPropertyMap {
public:
    void add(SKVPair p) { m_pairs.push_back(p); }
    std::string get(std::string name) {
        for(auto prop : m_pairs) {
            if(prop.key == name) return prop.value;
        }
        return "";
    }
    
    SKVPair first() { return m_pairs.front(); }
            
    private:
        std::vector<SKVPair> m_pairs;
};

class CSceneParser {
public:
    CSceneParser(ISceneGenerator& generator);
    bool ParseScene(std::istream& scene);
private:
    bool parseCamera(CTokenizer& tokenizer);
    
    SKeywordToken* getKeywordToken(CTokenizer& tokenizer);
    SSeparatorToken* getSeparatorToken(CTokenizer& tokenizer);
    bool readBlock(CTokenizer& tokenizer, CPropertyMap& pairs);
    ISceneGenerator& m_generator;
};

#endif /* SCENEPARSER_HPP */

