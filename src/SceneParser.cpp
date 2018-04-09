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
            if(!parseCamera(tokenizer)) return false;
        }
    }

    return true;
}

bool CSceneParser::readBlock(CTokenizer& tokenizer, CPropertyMap& map) {
        getSymToken(tokenizer);  // First block separator
        SToken::TokenType type;
        std::unique_ptr<SToken> tok;

        while((type = tokenizer.peekNextToken()) != SToken::NONE)  {

            switch(type) {
                case SToken::SYM:
                    tokenizer.getNextToken(&tok);
                    if(static_cast<SSymToken*>(tok.get())->str != "}")
                        throw ParserException("GENERAL: Unclosed block");
                    return true;     // Block closed hopefully
                    break;
                    
                case SToken::ID:
                {
                    tokenizer.getNextToken(&tok);  // Get name
                    auto name = static_cast<SIdToken*>(tok.get())->str;
                    SPropertyValue prop;
                    readPropertyValue(tokenizer, prop);
                    map[name] = prop;
                }
                 break;
                      
                default:
                    return false; // malformed code block
            }         
        }
 
        return false; // Block never closed
}

bool CSceneParser::readPropertyValue(CTokenizer& tokenizer, SPropertyValue& val) { 
    std::unique_ptr<SToken> tok;
    auto nextTok = tokenizer.peekNextToken() ;
    if(nextTok == SToken::ID) {
        tokenizer.getNextToken(&tok);
        val = static_cast<SIdToken*>(tok.get())->str;       
    } else if (nextTok == SToken::CONST) {
        // Read all consts
        while( tokenizer.peekNextToken() == SToken::CONST) {
            tokenizer.getNextToken(&tok);
             val << static_cast<SConstToken*>(tok.get())->val;  
        }
    } 
    return true;
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

    if(properties.first() != "type") throw ParserException("Camera: TYPE field missing or not first in block");
    if(!properties.hasProperty("name")) throw ParserException("Camera: Missing required field NAME");

    auto type = properties["type"].toStr() == "basic" ? CCamera::BASIC : CCamera::ADVANCED;
    CCamera cam(type,  properties["name"].toStr());
    
    if(properties.hasProperty("eye_point")) {
        auto ep = properties["eye_point"].toDoubleList();
        if(ep.size() != 3) throw ParserException("Camera: EYE_POINT array dimension mismatch");
        cam.setEyePoint(ep);
    }
    
    if(properties.hasProperty("look_point")) 
        cam.setLookPoint(properties["look_point"].toDoubleList());
    
    if(properties.hasProperty("up")) 
        cam.setUp(properties["up"].toDoubleList());
    
    if(properties.hasProperty("distance_image_plane")) 
        cam.setDistanceImagePlane(properties["distance_image_plane"].toDouble());

    return m_generator.Camera(cam);
}

SKeywordToken* CSceneParser::getKeywordToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    return static_cast<SKeywordToken*>(tok.get());
}

SSymToken* CSceneParser::getSymToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    if(tok->type != SToken::SYM) return nullptr;
    return static_cast<SSymToken*>(tok.get());
}
