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

    #include "nwocg_run.h"
    #include <math.h>
    static struct
    {
        double setpoint;
        double feedback;
        double Add1;
        double Add2;
        double Add3;
        double I_gain;
        double P_gain;
        double Ts;
        double UnitDelay1;
    } nwocg;

    void nwocg_generated_init()
    {
        nwocg.UnitDelay1 = 0;
    }
    void nwocg_generated_step()
    {
        nwocg.Add1 = nwocg.setpoint - nwocg.feedback;
        nwocg.I_gain = nwocg.Add1 * 2;
        nwocg.Ts = nwocg.I_gain * 0.01;
        nwocg.P_gain = nwocg.Add1 * 3;
        nwocg.Add2 = nwocg.Ts + nwocg.UnitDelay1;
        nwocg.Add3 = nwocg.P_gain + nwocg.Add2;
        nwocg.UnitDelay1 = nwocg.Add2;
    }
    static const nwocg_ExtPort ext_ports[] =
    {
        { "command", &nwocg.Add3, 0 },
        { "feedback", &nwocg.feedback, 1 },
        { "setpoint", &nwocg.setpoint, 1 },
        { 0, 0, 0 },
    };
    const nwocg_ExtPort * const nwocg_generated_ext_ports = ext_ports;
    const size_t nwocg_generated_ext_ports_size = sizeof(ext_ports);
*/
