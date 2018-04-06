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

bool CSceneParser::parseCamera(CTokenizer& tokenizer) {
    getSeparatorToken(tokenizer);
    getSeparatorToken(tokenizer);
    std::string type = getKeywordToken(tokenizer)->str; // Type always first
    getKeywordToken(tokenizer);
    std::string name = getKeywordToken(tokenizer)->str;
    CCamera cam(CCamera::BASIC, name);
    
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
    return static_cast<SSeparatorToken*>(tok.get());
}
