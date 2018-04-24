#ifndef TYPES_HPP
#define TYPES_HPP

#include <exception>
#include <cstdarg>

#include <Eigen/Dense>
using Eigen::Matrix4d;
using Eigen::Vector3d;

#include <iostream>

struct ParserException : public std::exception {
    ParserException(const char*  msg, ...) : exception(), m_msg(msg) {
        va_list args;
        va_start(args, msg);
        char tmp[500];
        sprintf(tmp, msg, args);
        m_msg = std::string(tmp);
    }
    
    const char* what() const throw() {
        return m_msg.c_str();
    }
    
    private:
        std::string m_msg;
};




#endif /* TYPES_HPP */

