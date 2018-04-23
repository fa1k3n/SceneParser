#ifndef PROPERTY_HPP
#define PROPERTY_HPP

#include <array>

struct PropertyException : public std::exception {
    PropertyException(std::string msg) : exception(), m_msg(msg) {}
    const char* what() const throw() {
        return m_msg.c_str();
    }
    
    private:
        std::string m_msg;
};

template <typename T, size_t size = 1>
class CProperty {
public:
    CProperty() {}
    
    CProperty(T val) {
        m_vals[0] = val;
    }
    
    CProperty(const std::vector<T>& val) {
        for(unsigned int i = 0; i < size; i ++)
            m_vals[i] = val[i];
    }

    T operator()(int index = 0) {
        return get(index);
    }
    
    void set(T val) {
        m_vals[0] = val;
    }
    
    void set(const std::vector<T>& val) {
        if(val.size() != size) throw PropertyException("Property: array dimension mismatch");
        for(unsigned int i = 0; i < size; i ++)
            m_vals[i] = val[i];
    }
    
    T get(int index = 0) {
        return m_vals[index];
    }
    
    std::vector<T> toVector() {
        return std::vector<T>(m_vals.begin(), m_vals.end());
    }
    
private:
    std::array<T, size> m_vals;
};

#endif /* PROPERTY_HPP */

