#ifndef PROPERTY_HPP
#define PROPERTY_HPP

#include <array>

struct PropertyException : public std::exception {
    explicit PropertyException(std::string msg) : exception(), m_msg(msg) {}
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
    
    explicit CProperty(T val) {
        m_vals[0] = val;
    }
    
    explicit CProperty(const std::vector<T>& val) {
        for(unsigned int i = 0; i < size; i ++)
            m_vals[i] = val[i];
    }

    T operator()(int index = 0) {
        return get(index);
    }

    T get(int index = 0) {
        return m_vals[index];
    }
private:
    std::array<T, size> m_vals;
};

#endif /* PROPERTY_HPP */

