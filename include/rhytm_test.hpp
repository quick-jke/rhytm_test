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
    ./rhytm_test --xml_path=../xml/scheme.xml --cpp_out=./index.hpp

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

/* 
    17 = 16 - 18
    25 = 17 * (2)
    26 = 25 + (0.01)
    19 = 17 * (3)
    22 = 26 + 21
    23 = 19 + 22
    21 = 22
*/

/*
System{blocks=[
  Block{blockType=Inport, Name="setpoint", SID=16, p=[P{Name="Position", value=[-20, 403, 10, 417]}], ports=[Port{p=[P{Name="PortNumber", value=1}, P{Name="Name", value=setpoint}]}]},
  Block{blockType=Inport, Name="feedback", SID=18, p=[P{Name="Position", value=[-20, 453, 10, 467]}, P{Name="Port", value=2}], ports=[Port{p=[P{Name="PortNumber", value=1}, P{Name="Name", value=feedback}]}]},
  Block{blockType=Sum, Name="Add1", SID=17, p=[P{Name="Ports", value=[2, 1]}, P{Name="Position", value=[105, 402, 135, 433]}, P{Name="IconShape", value=rectangular}, P{Name="Inputs", value=SignPair:+-}], ports=[]},
  Block{blockType=Sum, Name="Add2", SID=22, p=[P{Name="Ports", value=[2, 1]}, P{Name="Position", value=[340, 507, 370, 538]}, P{Name="IconShape", value=rectangular}], ports=[]},
  Block{blockType=Sum, Name="Add3", SID=23, p=[P{Name="Ports", value=[2, 1]}, P{Name="Position", value=[430, 412, 460, 443]}, P{Name="IconShape", value=rectangular}], ports=[Port{p=[P{Name="PortNumber", value=1}, P{Name="Name", value=command}]}]},
  Block{blockType=Gain, Name="I_gain", SID=25, p=[P{Name="Position", value=[210, 500, 240, 530]}, P{Name="PGain", value=2}], ports=[]},
  Block{blockType=Gain, Name="P_gain", SID=19, p=[P{Name="Position", value=[305, 405, 335, 435]}, P{Name="PGain", value=3}], ports=[]},
  Block{blockType=Gain, Name="Ts", SID=26, p=[P{Name="Position", value=[270, 500, 310, 530]}, P{Name="PGain", value=0.01}], ports=[]},
  Block{blockType=UnitDelay, Name="Unit Delay1", SID=21, p=[P{Name="Position", value=[350, 573, 385, 607]}, P{Name="SampleTime", value=-1}], ports=[]},
  Block{blockType=Outport, Name="command", SID=20, p=[P{Name="Position", value=[565, 423, 595, 437]}], ports=[]}
], 
lines=[
  Line{p=[P{Name="Name", value=setpoint}, P{Name="Src", value=PDirectionValue{SID=16, dir=Out, port=1}}, P{Name="Dst", value=PDirectionValue{SID=17, dir=In, port=1}}], branches=[]},
  Line{p=[P{Name="Name", value=feedback}, P{Name="Src", value=PDirectionValue{SID=18, dir=Out, port=1}}, P{Name="Points", value=[43,0;0,-35]}, P{Name="Dst", value=PDirectionValue{SID=17, dir=In, port=2}}], branches=[]},
  Line{p=[P{Name="Src", value=PDirectionValue{SID=17, dir=Out, port=1}}, P{Name="Points", value=[51, 0]}], branches=[Branch{p=[P{Name="Points", value=[0, 95]}, P{Name="Dst", value=PDirectionValue{SID=25, dir=In, port=1}}]}, Branch{p=[P{Name="Dst", value=PDirectionValue{SID=19, dir=In, port=1}}]}]},
  Line{p=[P{Name="Src", value=PDirectionValue{SID=21, dir=Out, port=1}}, P{Name="Points", value=[-31,0;0,-60]}, P{Name="Dst", value=PDirectionValue{SID=22, dir=In, port=2}}], branches=[]},
  Line{p=[P{Name="Src", value=PDirectionValue{SID=22, dir=Out, port=1}}, P{Name="Points", value=[34, 0]}], branches=[Branch{p=[P{Name="Points", value=[0, 65]}, P{Name="Dst", value=PDirectionValue{SID=21, dir=In, port=1}}]}, Branch{p=[P{Name="Points", value=[0, -90]}, P{Name="Dst", value=PDirectionValue{SID=23, dir=In, port=2}}]}]},
  Line{p=[P{Name="Src", value=PDirectionValue{SID=19, dir=Out, port=1}}, P{Name="Dst", value=PDirectionValue{SID=23, dir=In, port=1}}], branches=[]},
  Line{p=[P{Name="Name", value=command}, P{Name="Src", value=PDirectionValue{SID=23, dir=Out, port=1}}, P{Name="Dst", value=PDirectionValue{SID=20, dir=In, port=1}}], branches=[]},
  Line{p=[P{Name="Src", value=PDirectionValue{SID=25, dir=Out, port=1}}, P{Name="Dst", value=PDirectionValue{SID=26, dir=In, port=1}}], branches=[]},
  Line{p=[P{Name="Src", value=PDirectionValue{SID=26, dir=Out, port=1}}, P{Name="Dst", value=PDirectionValue{SID=22, dir=In, port=1}}], branches=[]}
]}
*/
