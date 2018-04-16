#include <SceneParser.hpp>
#include <Tokenizer.hpp>
#include <sstream>
#include <set>

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
        }  else if(keyw == "light") {
            readBlock(tokenizer, properties); 
            if(!parseLight(tokenizer, properties)) return false;
        }  else if(keyw == "geometry") {
            readBlock(tokenizer, properties); 
            if(!parseGeometry(tokenizer, properties)) return false;
        }  else if(keyw == "object") {
            readBlock(tokenizer, properties); 
            if(!parseObject(tokenizer, properties)) return false;
        }   else if(keyw == "misc") {
            readBlock(tokenizer, properties); 
            if(!parseMisc(tokenizer, properties)) return false;
        }
    } else if(type == SToken::TRANSF) {
        return true;
    } else throw ParserException("Unknown keyword");

    return true;
}

bool CSceneParser::readBlock(CTokenizer& tokenizer, CPropertyMap& map) {
        getSymToken(tokenizer);  // First block separator
        SToken::TokenType type;

        while((type = tokenizer.peekNextToken()) != SToken::NONE)  {
            switch(type) {
                case SToken::SYM:
                    if((getSymToken(tokenizer)->str != "}")) {
                        //CPropertyMap pm;
                        //readBlock(tokenizer, pm);
                        return false;
                    }
                    
                    return true;     // Block closed hopefully
                    break;
               
                // The Object keyword contains geometry and material keywords as
                // properties, need to read them here
                case SToken::KEYWORD:
                {
                    auto name = getKeywordToken(tokenizer)->str;
                    SPropertyValue prop;
                    readPropertyValue(tokenizer, prop);
                    map[name] =  prop;
                }
                 break;
                case SToken::ID:
                {
                    auto name = getIdToken(tokenizer)->str;
                    SPropertyValue prop;
                    readPropertyValue(tokenizer, prop);
                    map[name] =  prop;
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
    } else if (nextTok == SToken::SYM) {
         while( tokenizer.peekNextToken() == SToken::SYM) {
             CPropertyMap pm;
             readBlock(tokenizer, pm);
             val <<  pm;
         } 
    }
    return true;
}

bool CSceneParser::parseMisc(CTokenizer& tokenizer, CPropertyMap& properties) {
    SMisc misc;
    return m_generator.Misc(misc);
}

bool CSceneParser::parseObject(CTokenizer& tokenizer, CPropertyMap& properties) {
    if(!properties.hasProperty("geometry")) throw ParserException("Object: Missing required field GEOMETRY");
    if(!properties.hasProperty("material")) throw ParserException("Object: Missing required field MATERIAL");
    SObject obj(properties["geometry"].toStr(), properties["material"].toStr());
    if(properties.hasProperty("name")) obj.name.set(properties["name"].toStr());
    
    return m_generator.Object(obj);
}

bool CSceneParser::parseLight(CTokenizer& tokenizer, CPropertyMap& properties) {
    SLight* light;
    
    std::set<std::string> validKeywords = {"name", "type", "ambient", "diffuse", "specular", "position", "attenuation_coefs", "direction"};
    if(!checkKeywords(properties, validKeywords)) throw ParserException("Light: unknown keyword");
    
    if(properties.first() != "type") throw ParserException("Light: TYPE field missing or not first in block");
    if(!properties.hasProperty("name")) throw ParserException("Light: Missing required field NAME");
    
    SLight::LightType type = SLight::NONE;
    if(properties["type"].toStr() == "point") type = SLight::POINT;
    else if(properties["type"].toStr() == "directional") type =SLight::DIRECTIONAL;
    
    if(type == SLight::POINT) light = new SPointLight(properties["name"].toStr());
    else if(type == SLight::DIRECTIONAL) light = new SDirectionalLight(properties["name"].toStr());
    else throw ParserException("Light: unknown light type");

    if(properties.hasProperty("ambient")) light->ambient.set(properties["ambient"].toDoubleList());
    if(properties.hasProperty("diffuse")) light->diffuse.set(properties["diffuse"].toDoubleList());
    if(properties.hasProperty("specular")) light->specular.set(properties["specular"].toDoubleList());
    
    bool ret;
    if(type == SLight::POINT) {
        SPointLight* pl = static_cast<SPointLight*>(light);
        if(properties.hasProperty("position")) pl->position.set(properties["position"].toDoubleList());
        if(properties.hasProperty("attenuation_coefs")) pl->attenuationCoefs.set(properties["attenuation_coefs"].toDouble());
        ret = m_generator.Light(*pl);
    } else if (type == SLight::DIRECTIONAL) {
        SDirectionalLight* dl = static_cast<SDirectionalLight*>(light) ;
        if(properties.hasProperty("direction")) dl->direction.set(properties["direction"].toDoubleList());
        ret = m_generator.Light(*dl);
    } 
   
    delete light;
    return ret;
}

bool CSceneParser::parseMaterial(CTokenizer& tokenizer, CPropertyMap& properties) {
    std::set<std::string> validKeywords = {"name", "type", "emission", "ambient", "diffuse", "specular", "specular_power", "texture"};
    if(!checkKeywords(properties, validKeywords)) throw ParserException("Material: unknown keyword");
    
    if(properties.first() != "type") throw ParserException("Camera: TYPE field missing or not first in block");
    if(!properties.hasProperty("name")) throw ParserException("Camera: Missing required field NAME");
    
    SMaterial mat(SMaterial::BASIC,  properties["name"].toStr());
    if(properties.hasProperty("emission")) mat.emission.set(properties["emission"].toDoubleList());
    if(properties.hasProperty("ambient")) mat.ambient.set(properties["ambient"].toDoubleList());
    if(properties.hasProperty("diffuse")) mat.diffuse.set(properties["diffuse"].toDoubleList());
    if(properties.hasProperty("specular")) mat.specular.set(properties["specular"].toDoubleList());
    if(properties.hasProperty("specular_power")) mat.specularPower.set(properties["specular_power"].toDouble());
    if(properties.hasProperty("texture")) mat.texture.set(properties["texture"].toStr());

    return m_generator.Material(mat);
}

bool CSceneParser::parseCamera(CTokenizer& tokenizer, CPropertyMap& properties) { 
    SCamera* cam = nullptr;
   
    std::set<std::string> validKeywords = {"name", "type", "eye_point", "look_point", "up", "distance_image_plane", "fov", "aspect_ratio", "top", "bottom", "left", "right"};
    if(!checkKeywords(properties, validKeywords)) throw ParserException("Camera: unknown keyword");
    
    if(properties.first() != "type") throw ParserException("Camera: TYPE field missing or not first in block");
    if(!properties.hasProperty("name")) throw ParserException("Camera: Missing required field NAME");

    auto type = properties["type"].toStr() == "basic" ? SCamera::BASIC : SCamera::ADVANCED;
    if(type == SCamera::BASIC) cam = new SBasicCamera( properties["name"].toStr()) ;
    else if(type == SCamera::ADVANCED) cam = new SAdvancedCamera(properties["name"].toStr());
    else throw ParserException("Camera: unknown camera type");

    if(properties.hasProperty("eye_point")) cam->eyePoint.set(properties["eye_point"].toDoubleList());
    if(properties.hasProperty("look_point")) cam->lookPoint.set(properties["look_point"].toDoubleList());
    if(properties.hasProperty("up"))  cam->up.set(properties["up"].toDoubleList());
    if(properties.hasProperty("distance_image_plane")) cam->distanceImagePlane.set(properties["distance_image_plane"].toDouble());

    bool ret = false;
    if(type == SCamera::BASIC) {
        SBasicCamera* bc = static_cast<SBasicCamera*>(cam);
        if(properties.hasProperty("fov")) bc->fov.set(properties["fov"].toDouble());
        if(properties.hasProperty("aspect_ratio")) bc->aspectRatio.set(properties["aspect_ratio"].toDouble());
        ret = m_generator.Camera(*bc);
    } else if (type == SCamera::ADVANCED) {
        SAdvancedCamera* ac = static_cast<SAdvancedCamera*>(cam);
        if(properties.hasProperty("left")) ac->left.set(properties["left"].toDouble());
        if(properties.hasProperty("right")) ac->right.set(properties["right"].toDouble());
        if(properties.hasProperty("top")) ac->top.set(properties["top"].toDouble());        
        if(properties.hasProperty("bottom")) ac->bottom.set(properties["bottom"].toDouble());
        ret = m_generator.Camera(*ac);
    }

    delete cam;
    return ret;
}

#include <iostream>
bool CSceneParser::parseGeometry(CTokenizer& tokenizer, CPropertyMap& properties) {
     SGeometry* geom = nullptr;
   
    std::set<std::string> validKeywords = {"name", "type", "vertices", "tri"};
    if(!checkKeywords(properties, validKeywords)) throw ParserException("Geometry: unknown keyword");
    
    if(properties.first() != "type") throw ParserException("Geometry: TYPE field missing or not first in block");
    if(!properties.hasProperty("name")) throw ParserException("Geometry: Missing required field NAME");

    auto type = SGeometry::NONE;;
    if(properties["type"].toStr() == "sphere") type = SGeometry::SPHERE;
    else if(properties["type"].toStr() == "mesh") type = SGeometry::MESH;
    
    if(type == SGeometry::SPHERE) geom = new SSphere( properties["name"].toStr()) ;
    else if(type == SGeometry::MESH) geom = new SMesh(properties["name"].toStr());
    else throw ParserException("Geometry: unknown geometry");

    bool ret = false;
      if(type == SGeometry::MESH) {
        SMesh* mesh = static_cast<SMesh*>(geom);
        if(properties.hasProperty("tri")) mesh->tri.set(properties["tri"].toDoubleList());
        if(properties.hasProperty("vertices")) {
            // BUGFROM HERE
            std::vector<SVertex> vertices;
            vertices.push_back({-1, 1, 0});     
           mesh->vertices.set(vertices);
        }
        ret = m_generator.Geometry(*mesh);
        
    } else
        ret = m_generator.Geometry(*geom);
    delete geom;
    return ret;
}

SKeywordToken* CSceneParser::getKeywordToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
     if(tok->type != SToken::KEYWORD) return nullptr;
    return static_cast<SKeywordToken*>(tok.get());
}

bool CSceneParser::checkKeywords(CPropertyMap& properties, std::set<std::string> validKeywords) {
    for(auto keyw : properties) 
        if(validKeywords.find(keyw.first) == validKeywords.end()) return false;
    return true;
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
