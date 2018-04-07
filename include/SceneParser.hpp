#ifndef SCENEPARSER_HPP
#define SCENEPARSER_HPP

#include <ISceneGenerator.hpp>
#include <istream>
#include <Tokenizer.hpp>
#include <vector>

struct SProperty {
    std::string key;
    std::string value;
};

class CPropertyMap {
public:
    void add(SProperty p) { m_pairs.push_back(p); }
    std::string& get(std::string name) {
        for(int i = 0; i < m_pairs.size(); i++) {
            if(m_pairs[i].key == name) return m_pairs[i].value;
        }
    }
    
    std::string first() { return m_pairs.front().key; }
    
    std::string& operator[](std::string name) { return get(name); }
    private:
        std::vector<SProperty> m_pairs;
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

