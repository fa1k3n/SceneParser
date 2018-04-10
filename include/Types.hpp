#ifndef TYPES_HPP
#define TYPES_HPP

#include <map>
#include <vector>
#include <exception>

struct ImplicitTypeConversion : public std::exception {
    ImplicitTypeConversion(std::string msg) : exception() , m_msg(msg) {}
    const char* what() const throw() {
        return m_msg.c_str();
    }
    
    private:
        std::string m_msg;
};

struct NonExistingProperty : public std::exception {
    NonExistingProperty(std::string msg) : exception() , m_msg(msg) {}
    const char* what() const throw() {
        return m_msg.c_str();
    }
    
    private:
        std::string m_msg;
};

struct ParserException : public std::exception {
    ParserException(std::string msg) : exception(), m_msg(msg) {}
    const char* what() const throw() {
        return m_msg.c_str();
    }
    
    private:
        std::string m_msg;
};



class CPropertyMap;

struct SPropertyValue {
    enum ValueType {
        NONE,
        DOUBLE,
        DOUBLE_LIST, 
        STRING,
        STRING_LIST, 
        MAP,
        MAP_LIST
    };
    
    SPropertyValue() : type(NONE) {}
    SPropertyValue(double val) : type(DOUBLE) {m_double.push_back(val);}
    SPropertyValue(std::string str) : type(STRING) {m_str.push_back(str);}
    SPropertyValue(CPropertyMap& map) : type(MAP) {m_map.push_back(&map);}

    void operator=(double d) { 
        if(type != NONE && type != DOUBLE) 
            throw ImplicitTypeConversion("Trying to assign non-double to double property");
        type = DOUBLE;
        m_double.push_back(d); 
    }
    
    void operator=(std::string s) { 
        if(type != NONE && type != STRING) 
            throw ImplicitTypeConversion("Trying to assign non-string to string property");
        type = STRING; 
        m_str.push_back(s); 
    }
    
     void operator=(CPropertyMap& map) { 
        if(type != NONE && type != MAP) 
            throw ImplicitTypeConversion("Trying to assign non-string to string property");
        type = MAP; 
        m_map.push_back(&map); 
    }
     
     SPropertyValue& operator<<(double v) {
        if(type == NONE) type = DOUBLE;
       else if (type == DOUBLE) type = DOUBLE_LIST;
       else if(type != DOUBLE_LIST) throw ImplicitTypeConversion("Trying to assign double to non-double property");
       m_double.push_back(v);
       return *this;
     }
     
      SPropertyValue& operator<<(std::string s) {
        if(type == NONE) type = STRING;
       else if (type == STRING) type = STRING_LIST;
       else if (type != STRING_LIST) throw ImplicitTypeConversion("Trying to assign string to non-string property");
       m_str.push_back(s);
       return *this;
    }
      
    SPropertyValue& operator<<(CPropertyMap& s) {
        if(type == NONE) type = MAP;
       else if (type == MAP) type = MAP_LIST;
       else if(type != MAP_LIST) throw ImplicitTypeConversion("Trying to assign property map to non-property map property");
       m_map.push_back(&s);
       return *this;
    }
     
    double toDouble(int index = 0) { 
        if(type != DOUBLE && type != DOUBLE_LIST)
            throw ImplicitTypeConversion("Trying to read a double from non-double property" );
        return m_double[index]; 
    }
    
    std::vector<double> toDoubleList() {
        if(type != DOUBLE_LIST && type != DOUBLE)
            throw ImplicitTypeConversion("Trying to read a double list from non-double list property" );
        return m_double;
    }
    
    std::string& toStr(int index = 0) { 
          if(type != STRING && type != STRING_LIST)
            throw ImplicitTypeConversion("Trying to read a string from non-string property" );
        return m_str[index]; 
    }
    
    std::vector<std::string> toStrList() {
        if(type != STRING_LIST && type != STRING)
            throw ImplicitTypeConversion("Trying to read a string list from non-string list property" );
        return m_str;
    }
    
    CPropertyMap& toMap(int index = 0) {
        if(type != MAP && type != MAP_LIST)
            throw ImplicitTypeConversion("Trying to read a map from non-map property" );
        return *m_map[index];
    }
    ValueType type;
    
private:
    std::vector<double> m_double;
    std::vector<std::string> m_str;
    std::vector<CPropertyMap*> m_map;
};

class CPropertyMap {
public:    
    std::string first() { return m_firstKey; }
    
    SPropertyValue& operator[](std::string name) {
         if(m_props.empty()) {
            m_firstKey = name;
        }
        return m_props[name]; 
    }
    
    bool hasProperty(std::string name) { return m_props.find(name) != m_props.end(); }
    
    std::map<std::string, SPropertyValue>::iterator begin() { return m_props.begin(); }
    std::map<std::string, SPropertyValue>::iterator end() { return m_props.end(); }
    
private:
        std::map<std::string, SPropertyValue> m_props;
        std::string m_firstKey;
};


#endif /* TYPES_HPP */

