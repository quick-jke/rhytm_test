#ifndef INCLUDE_RHYTM_TEST_HPP
#define INCLUDE_RHYTM_TEST_HPP
#include <string>
#include <fstream>
#include <sstream>
struct config{
    std::string xml_path;
    std::string out_path;
};

config get_cmd_path(int argc, char* argv[]){
    config cfg;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg.find("--xml_path=") == 0) {
            cfg.xml_path = arg.substr(11); 
        } else if (arg.find("--cpp_out=") == 0) {
            cfg.out_path = arg.substr(10); 
        }
    }
    return cfg;
}

std::string content(const std::string& xml_path){
    std::ifstream file(xml_path);
    
    if (!file.is_open()) {
        throw std::runtime_error("Can not open file: " + xml_path);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();  

    return buffer.str();
}

#endif
/*
    ./rhytm_test --xml_path=../xml/scheme.xml --cpp_out=./out/index.hpp
*/
