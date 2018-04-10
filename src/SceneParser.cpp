#include <SceneParser.hpp>
#include <Tokenizer.hpp>
#include <sstream>

CSceneParser::CSceneParser(ISceneGenerator& generator) : 
    m_generator(generator) 
{}

bool CSceneParser::ParseScene(std::string scene) {
     std::istringstream s(scene);
     return ParseScene(s);
}

bool CSceneParser::ParseScene(std::istream& scene) {
    CTokenizer tokenizer(scene);
    SToken::TokenType type = tokenizer.peekNextToken();
    if(type == SToken::KEYWORD) {
        CPropertyMap properties;
        auto keyw = getKeywordToken(tokenizer)->str;
        if(keyw == "camera") {
            readBlock(tokenizer, properties); 
            if(!parseCamera(tokenizer, properties)) return false;
        } else if(keyw == "material") {
            readBlock(tokenizer, properties); 
            if(!parseMaterial(tokenizer, properties)) return false;
        }
    }

    return true;
}

bool CSceneParser::readBlock(CTokenizer& tokenizer, CPropertyMap& map) {
        getSymToken(tokenizer);  // First block separator
        SToken::TokenType type;

        while((type = tokenizer.peekNextToken()) != SToken::NONE)  {
            switch(type) {
                case SToken::SYM:
                    if(getSymToken(tokenizer)->str != "}")
                        throw ParserException("GENERAL: Unclosed block");
                    return true;     // Block closed hopefully
                    break;
                    
                case SToken::ID:
                {
                    auto name = getIdToken(tokenizer)->str;
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
    auto nextTok = tokenizer.peekNextToken() ;
    if(nextTok == SToken::ID) {
        val = getIdToken(tokenizer)->str;
    } else if (nextTok == SToken::CONST) {
        // Glob all consts
        while( tokenizer.peekNextToken() == SToken::CONST) 
            val << getConstToken(tokenizer)->val;
    } 
    return true;
}

bool CSceneParser::parseMaterial(CTokenizer& tokenizer, CPropertyMap& properties) {
    if(properties.first() != "type") throw ParserException("Camera: TYPE field missing or not first in block");
    if(!properties.hasProperty("name")) throw ParserException("Camera: Missing required field NAME");
  
    SMaterial mat(SMaterial::BASIC,  properties["name"].toStr());
    
    return m_generator.Material(mat);
}

bool CSceneParser::parseCamera(CTokenizer& tokenizer, CPropertyMap& properties) { 
    SCamera* cam;
   
    if(properties.first() != "type") throw ParserException("Camera: TYPE field missing or not first in block");
    if(!properties.hasProperty("name")) throw ParserException("Camera: Missing required field NAME");

    auto type = properties["type"].toStr() == "basic" ? SCamera::BASIC : SCamera::ADVANCED;
    if(type == SCamera::BASIC) cam = new SBasicCamera( properties["name"].toStr()) ;
    
    if(properties.hasProperty("eye_point")) cam->eyePoint.set(properties["eye_point"].toDoubleList());
    if(properties.hasProperty("look_point")) cam->lookPoint.set(properties["look_point"].toDoubleList());
    if(properties.hasProperty("up"))  cam->up.set(properties["up"].toDoubleList());
    if(properties.hasProperty("distance_image_plane")) cam->distanceImagePlane.set(properties["distance_image_plane"].toDouble());

    bool ret = m_generator.Camera(*cam);
    delete cam;
    return ret;
}

SKeywordToken* CSceneParser::getKeywordToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
     if(tok->type != SToken::KEYWORD) return nullptr;
    return static_cast<SKeywordToken*>(tok.get());
}

SSymToken* CSceneParser::getSymToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    if(tok->type != SToken::SYM) return nullptr;
    return static_cast<SSymToken*>(tok.get());
}

SConstToken* CSceneParser::getConstToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    if(tok->type != SToken::CONST) return nullptr;
    return static_cast<SConstToken*>(tok.get());
}

SIdToken* CSceneParser::getIdToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    if(tok->type != SToken::ID) return nullptr;
    return static_cast<SIdToken*>(tok.get());
}
