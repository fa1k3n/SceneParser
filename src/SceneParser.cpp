#include <SceneParser.hpp>
#include <Tokenizer.hpp>

CSceneParser::CSceneParser(ISceneGenerator& generator) : 
    m_generator(generator) 
{}

    
bool CSceneParser::ParseScene(std::istream& scene) {
    CTokenizer tokenizer(scene);
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    if(tok->type == SToken::KEYWORD) {
        auto idt = static_cast<SKeywordToken*>(tok.get());
        if(idt->str == "camera") {
            parseCamera(tokenizer);
        }
    }

    return true;
}

bool CSceneParser::readBlock(CTokenizer& tokenizer, CPropertyMap& map) {
        getSeparatorToken(tokenizer);  // First block separator
        
        std::unique_ptr<SToken> tok;
        while(tokenizer.getNextToken(&tok)) {
            switch(tok->type) {
                case SToken::SEPARATOR:
                    return true;     // Block closed 
                    break;
                    
                case SToken::KEYWORD:
                {
                    auto keyw = static_cast<SKeywordToken*>(tok.get());
                    auto value = getKeywordToken(tokenizer);
                    map.add(SKVPair{keyw->str, value->str});
                }
                 break;
                    
                default:
                    return false; // malformed code block
            }         
        }
 
        return false; // Block never closed
}

bool CSceneParser::parseCamera(CTokenizer& tokenizer) {
    
    CPropertyMap properties;
    readBlock(tokenizer, properties); 
    if(properties.first().key != "type") return false; // First property must by type
    auto type = properties.get("type") == "basic" ? CCamera::BASIC : CCamera::ADVANCED;
    CCamera cam(type,  properties.get("name"));
    
    m_generator.Camera(cam);
    return true;
}

SKeywordToken* CSceneParser::getKeywordToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    return static_cast<SKeywordToken*>(tok.get());
}

SSeparatorToken* CSceneParser::getSeparatorToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    if(tok->type != SToken::SEPARATOR) return nullptr;
    return static_cast<SSeparatorToken*>(tok.get());
}
