#ifndef INCLUDE_CODEGEN_HPP
#define INCLUDE_CODEGEN_HPP
#include <string>
#include <fstream>
#include "scheme.hpp"
class CodeGenerator{
public:
    CodeGenerator(System system);
    void write(const std::string& out_path);
private:
    System system_;
};
#endif


