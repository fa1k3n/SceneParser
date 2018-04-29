#include <SceneParser.hpp>
#include <Tokenizer.hpp>
#include <sstream>
#include <set>
#include <Eigen/Geometry>
#include <math.h>

CSceneParser::CSceneParser(ISceneGenerator& generator) : 
    m_generator(generator) 
{
    //Create unit transform
    m_transformStack.push(Matrix4d::Identity());
}

bool CSceneParser::ParseScene(std::string scene) {
     std::istringstream s(scene);
     return ParseScene(s);
}

bool CSceneParser::ParseScene(std::istream& scene) {
    CTokenizer tokenizer(scene);
    while(tokenizer.hasMoreTokens()) {
        SToken::TokenType type = tokenizer.peekNextToken();

        if(type == SToken::KEYWORD) {
            CPropertyMap properties;
            auto keyw = getKeywordToken(tokenizer).str;
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
            handleTransf(tokenizer);
        } else {
            std::unique_ptr<SToken> tok;
            tokenizer.getNextToken(&tok);
            std::ostringstream oss;
            oss << "Unexpected token of type " << tok->toStr() << " at pos " << tokenizer.getCurrentPos();
            throw ParserException(oss.str().c_str());
        }
   }
    return true;
}

bool CSceneParser::handleTransf(CTokenizer& tokenizer) {
    STransfToken::TransfTypeID id = getTransfToken(tokenizer).id;
    switch(id) {
        case STransfToken::PUSH:
            m_transformStack.push(Matrix4d::Identity());
        break;
        case STransfToken::POP:
            m_transformStack.push(Matrix4d::Identity());
        break;
        case STransfToken::TRANSLATE:
        {
            std::vector<double> vals;
            readConstVect<3>(tokenizer, vals);
            m_transformStack.top() *=  Eigen::Affine3d(Eigen::Translation3d(vals[0], vals[1], vals[2])).matrix();
        }
        break;
        case STransfToken::SCALE:
        {
            std::vector<double> vals;
            readConstVect<3>(tokenizer, vals);
            m_transformStack.top() *=  Eigen::Affine3d(Eigen::Scaling(vals[0], vals[1], vals[2])).matrix();
        }
        break;
        case STransfToken::ROTATE:
        {
            std::vector<double> vals;
            readConstVect<4>(tokenizer, vals);
            m_transformStack.top() *=  Eigen::Affine3d(Eigen::AngleAxis<double>( vals[3] * M_PI / 180.0, 
                    Eigen::Vector3d(vals[0], vals[1], vals[2]))).matrix();
        }
        break;
        case STransfToken::TRANSFORM:
        {
            std::vector<double> vals;
            readConstVect<12>(tokenizer, vals);
            std::vector<double> tmp = {0.0, 0.0, 0.0, 1.0};
            vals.insert(std::end(vals), std::begin(tmp), std::end(tmp));
            m_transformStack.top() *=  Eigen::Matrix4d(vals.data());
        }
        break;
        case STransfToken::LOAD:
           m_transformStack.top() =  Eigen::Matrix4d::Identity();
        break;

        default:
            throw ParserException("Unknown transform %d", id);
    }
    return true;
}

bool CSceneParser::readBlock(CTokenizer& tokenizer, CPropertyMap& map) {
        getSymToken(tokenizer);  // First block separator
        SToken::TokenType type;

        while((type = tokenizer.peekNextToken()) != SToken::NONE)  {
            switch(type) {
                case SToken::SYM:
                    if((getSymToken(tokenizer).str != "}")) {
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
                    auto name = getKeywordToken(tokenizer).str;
                    SPropertyValue prop;
                    readPropertyValue(tokenizer, prop);
                    map[name] =  prop;
                }
                 break;
                case SToken::ID:
                {
                    auto name = getIdToken(tokenizer).str;
                    SPropertyValue prop;
                    readPropertyValue(tokenizer, prop);
                    // TRI is a special case as it might occure several times in a
                    // given block
                    if(name == "tri") {
                        auto tmp = prop.toDoubleList();
                        map[name] <<  tmp[0] << tmp[1] << tmp[2];
                    } else
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
        val = getIdToken(tokenizer).str;
    } else if (nextTok == SToken::CONST) {
        // Glob all consts
        while( tokenizer.peekNextToken() == SToken::CONST) 
            val << getConstToken(tokenizer).val;
    } else if (nextTok == SToken::SYM) {
         while( tokenizer.peekNextToken() == SToken::SYM) {
             CPropertyMap pm;;
             readBlock(tokenizer, pm);
             val <<  pm;
         } 
    }
    return true;
}

bool CSceneParser::parseMisc(CTokenizer& tokenizer, CPropertyMap& properties) {
    SMisc misc;
    return m_generator.Misc(misc, m_transformStack.top());
}

bool CSceneParser::parseObject(CTokenizer& tokenizer, CPropertyMap& properties) {
    if(!properties.hasProperty("geometry")) throw ParserException("Object: Missing required field GEOMETRY");
    if(!properties.hasProperty("material")) throw ParserException("Object: Missing required field MATERIAL");
    SObject obj(properties["geometry"].toStr(), properties["material"].toStr());
    if(properties.hasProperty("name")) obj.name = properties["name"].toStr();
    
    return m_generator.Object(obj, m_transformStack.top());
}

bool CSceneParser::parseLight(CTokenizer& tokenizer, CPropertyMap& properties) {
    SLight* light;
    
    std::set<std::string> validKeywords = {"name", "type", "ambient", "diffuse", "specular", "position", "attenuation_coefs", "direction"};
    //if(!checkKeywords(properties, validKeywords)) throw ParserException("Light: unknown keyword");
    for(auto keyw : properties) {
        if(validKeywords.find(keyw.first) == validKeywords.end()) {
            std::ostringstream oss;
            oss << "Light, unknown property " << keyw.first;
            throw ParserException(oss.str().c_str());
        }
    }

    if(properties.first() != "type") throw ParserException("Light: TYPE field missing or not first in block");
    if(!properties.hasProperty("name")) throw ParserException("Light: Missing required field NAME");
    
    SLight::LightType type = SLight::NONE;
    if(properties["type"].toStr() == "point") type = SLight::POINT;
    else if(properties["type"].toStr() == "directional") type =SLight::DIRECTIONAL;
    
    if(type == SLight::POINT) light = new SPointLight(properties["name"].toStr());
    else if(type == SLight::DIRECTIONAL) light = new SDirectionalLight(properties["name"].toStr());
    else throw ParserException("Light: unknown light type");

    if(properties.hasProperty("ambient")) light->ambient = toVector3d(properties["ambient"].toDoubleList());
    if(properties.hasProperty("diffuse")) light->diffuse = toVector3d(properties["diffuse"].toDoubleList());
    if(properties.hasProperty("specular")) light->specular = toVector3d(properties["specular"].toDoubleList());
    
    bool ret;
    if(type == SLight::POINT) {
        SPointLight* pl = static_cast<SPointLight*>(light);
        if(properties.hasProperty("position")) pl->position = toVector3d(properties["position"].toDoubleList());
        if(properties.hasProperty("attenuation_coefs")) pl->attenuationCoefs = toVector3d(properties["attenuation_coefs"].toDoubleList());
        ret = m_generator.Light(*pl, m_transformStack.top());
    } else if (type == SLight::DIRECTIONAL) {
        SDirectionalLight* dl = static_cast<SDirectionalLight*>(light) ;
        if(properties.hasProperty("direction")) dl->direction = toVector3d(properties["direction"].toDoubleList());
        ret = m_generator.Light(*dl, m_transformStack.top());
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
    if(properties.hasProperty("emission")) mat.emission = toVector3d(properties["emission"].toDoubleList());
    if(properties.hasProperty("ambient")) mat.ambient = toVector3d(properties["ambient"].toDoubleList());
    if(properties.hasProperty("diffuse")) mat.diffuse = toVector3d(properties["diffuse"].toDoubleList());
    if(properties.hasProperty("specular")) mat.specular = toVector3d(properties["specular"].toDoubleList());
    if(properties.hasProperty("specular_power")) mat.specularPower = properties["specular_power"].toDouble();
    if(properties.hasProperty("texture")) mat.texture = properties["texture"].toStr();

    return m_generator.Material(mat, m_transformStack.top());
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

    if(properties.hasProperty("eye_point")) cam->eyePoint = toVector3d(properties["eye_point"].toDoubleList());
    if(properties.hasProperty("look_point")) cam->lookPoint = toVector3d(properties["look_point"].toDoubleList());
    if(properties.hasProperty("up"))  cam->up = toVector3d(properties["up"].toDoubleList());
    if(properties.hasProperty("distance_image_plane")) cam->distanceImagePlane = properties["distance_image_plane"].toDouble();

    bool ret = false;
    if(type == SCamera::BASIC) {
        SBasicCamera* bc = static_cast<SBasicCamera*>(cam);
        if(properties.hasProperty("fov")) bc->fov = properties["fov"].toDouble();
        if(properties.hasProperty("aspect_ratio")) bc->aspectRatio = properties["aspect_ratio"].toDouble();
        ret = m_generator.Camera(*bc, m_transformStack.top());
    } else if (type == SCamera::ADVANCED) {
        SAdvancedCamera* ac = static_cast<SAdvancedCamera*>(cam);
        if(properties.hasProperty("left")) ac->left = properties["left"].toDouble();
        if(properties.hasProperty("right")) ac->right = properties["right"].toDouble();
        if(properties.hasProperty("top")) ac->top = properties["top"].toDouble();        
        if(properties.hasProperty("bottom")) ac->bottom = properties["bottom"].toDouble();
        ret = m_generator.Camera(*ac, m_transformStack.top());
    }

    delete cam;
    return ret;
}

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
        if(properties.hasProperty("tri")) {
            auto tmp = properties["tri"].toDoubleList();
            for(unsigned int i = 0; i < tmp.size(); ) {
                int v1 = tmp[i++];
                int v2 = tmp[i++];
                int v3 = tmp[i++];
                mesh->tri.push_back({v1, v2, v3});
            }
        }
        if(properties.hasProperty("vertices")) {
            auto verts = properties["vertices"].toMapList();
            for(unsigned int i = 0; i < verts.size(); i++ ) {
               SVertex v;
               if(verts[i].hasProperty("p")) v.p = toVector3d(verts[i]["p"].toDoubleList());
               if(verts[i].hasProperty("n")) v.n = toVector3d(verts[i]["n"].toDoubleList());
               if(verts[i].hasProperty("tc")) v.tc = toVector3d(verts[i]["tc"].toDoubleList());
                mesh->vertices.push_back(v);
            }

        }
        ret = m_generator.Geometry(*mesh, m_transformStack.top());
        
    } else
        ret = m_generator.Geometry(*geom, m_transformStack.top());
    delete geom;
    return ret;
}

SKeywordToken CSceneParser::getKeywordToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    return *static_cast<SKeywordToken*>(tok.get());
}

bool CSceneParser::checkKeywords(CPropertyMap& properties, std::set<std::string> validKeywords) {
    for(auto keyw : properties) 
        if(validKeywords.find(keyw.first) == validKeywords.end()) return false;
    return true;
}

SSymToken CSceneParser::getSymToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    return *static_cast<SSymToken*>(tok.get());
}

SConstToken CSceneParser::getConstToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
//    if(tok->type != SToken::CONST) return nullptr;
    return *static_cast<SConstToken*>(tok.get());
}

SIdToken CSceneParser::getIdToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    return *static_cast<SIdToken*>(tok.get());
}
    
STransfToken CSceneParser::getTransfToken(CTokenizer& tokenizer) {
    std::unique_ptr<SToken> tok;
    tokenizer.getNextToken(&tok);
    return *static_cast<STransfToken*>(tok.get());
}

Vector3d CSceneParser::toVector3d(std::vector<double> dblList) {
    if(dblList.size() != 3) throw ParserException("Not enough values in list");
    Vector3d tmp = Vector3d::Zero();
    for(unsigned int i = 0; i < dblList.size(); i ++) {
        tmp(i) = dblList[i];
    }
    return tmp;
}
