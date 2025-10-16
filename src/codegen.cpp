#include "codegen.hpp"
#include <iostream>
CodeGenerator::CodeGenerator(System system) : system_(system){}

void CodeGenerator::write(const std::string& out_path){
    std::cout << out_path << std::endl;
    std::ofstream file(out_path);
    file << system_.content();
}
