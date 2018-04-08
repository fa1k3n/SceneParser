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
        
        SToken::TokenType type;
        std::unique_ptr<SToken> tok;

        while((type = tokenizer.peekNextToken()) != SToken::NONE)  {

            switch(type) {
                case SToken::SEPARATOR:
                    tokenizer.getNextToken(&tok);
                    return true;     // Block closed 
                    break;
                    
                case SToken::KEYWORD:
                {
                    SProperty prop;
                    readProperty(tokenizer, prop);
                    map.add(prop);
                }
                 break;
                      
                default:
                    return false; // malformed code block
            }         
        }
 
        return false; // Block never closed
}

bool CSceneParser::readProperty(CTokenizer& tokenizer, SProperty& val) {
     std::unique_ptr<SToken> tok;

    tokenizer.getNextToken(&tok);
    val.name = static_cast<SKeywordToken*>(tok.get())->str;
    auto nextTok = tokenizer.peekNextToken() ;
    if(nextTok == SToken::KEYWORD) {
        val.value = getKeywordToken(tokenizer)->str;
        //map[keyw->str] = value->str;
       
    } else if (nextTok == SToken::CONST) {
        // Read all consts
        while( tokenizer.peekNextToken() == SToken::CONST)
             tokenizer.getNextToken(&tok);

    }
}


bool CSceneParser::readConstArray(CTokenizer& tokenizer, std::vector<double>& vals) {
    std::unique_ptr<SToken> tok;
    for(int i = 0; i < 3; i ++) {
        tokenizer.getNextToken(&tok);
        vals.push_back(static_cast<SConstToken*>(tok.get())->val);
    }
    return true;
}

bool CSceneParser::parseCamera(CTokenizer& tokenizer) {
    
    CPropertyMap properties;
    readBlock(tokenizer, properties); 
    if(properties.first() != "type") return false; // First property must by type
    auto type = properties["type"] == "basic" ? CCamera::BASIC : CCamera::ADVANCED;
    CCamera cam(type,  properties["name"]);
    
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
