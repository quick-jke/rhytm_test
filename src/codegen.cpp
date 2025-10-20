#include "codegen.hpp"
#include <iostream>
#include "topological_sort.hpp"
CodeGenerator::CodeGenerator(System system) : system_(system){}

std::string writeGain(Block src, std::vector<Block> deps){
    std::stringstream oss;
    oss << "\tnwocg." << removeSpaces(src.Name) << " = ";
    for(auto const& dep : deps){
        oss << "nwocg." << removeSpaces(dep.Name) << ", ";
    }
    oss << std::endl;

    return oss.str();
}


std::string writeAdd(Block src, std::vector<Block> deps){
    std::stringstream oss;
    oss << "\tnwocg." << removeSpaces(src.Name) << " = ";
    for(auto const& dep : deps){
        oss << "nwocg." << removeSpaces(dep.Name) << ", ";
    }

    for(auto const& p : src.p){
        if(p.Name == enums::PName::Inputs){
            oss << std::get<SignPair>(p.value).to_string() << std::endl;
        }
    }
    
    oss << std::endl;


    return oss.str();
}



void CodeGenerator::write(const std::string& out_path){
    std::ofstream file(out_path);
    auto blocks = system_.blocks;
    auto lines = system_.lines;
    std::stringstream oss;

    oss << "#include \"nwocg_run.h\"\n#include <math.h>\n\n";

    // 1. struct nwocg
    oss << "static struct\n{\n";
    for (const auto& block : blocks) {
        oss << "\tdouble " << removeSpaces(block.Name) << ";\n";
    }
    oss << "} nwocg;\n\n";

    // 2. init
    oss << "void nwocg_generated_init()\n{\n";
    for (const auto& block : blocks) {
        if (block.blockType == enums::BlockType::UnitDelay) {
            oss << "\tnwocg." << removeSpaces(block.Name) << " = 0;\n";
        }
    }
    oss << "}\n\n";

    // 3. step 

    TopologicalSort sorter(blocks, lines);
    auto sorted = sorter.sort();


    oss << "void nwocg_generated_step()\n{\n";
    for(auto const& [src, deps] : sorted){
        switch (src.blockType)
        {
            case enums::BlockType::Gain: {
                oss << writeGain(src, deps);
                break;
            }
            case enums::BlockType::Sum: {
                oss << writeAdd(src, deps);
                break;
            }
            case enums::BlockType::UnitDelay: {
                oss << "\tnwocg." << removeSpaces(src.Name) << " = ";
                for(auto const& dep : deps){
                    oss << "nwocg." << removeSpaces(dep.Name) << ", ";
                }
                oss << std::endl;
                break;

            }
            default:{
                break;
            }
        }
        
    }
    oss << "}\n";


    // void nwocg_generated_step()
    // {
    //     nwocg.Add1 = nwocg.setpoint - nwocg.feedback;
    //     nwocg.I_gain = nwocg.Add1 * 2;
    //     nwocg.Ts = nwocg.I_gain * 0.01;
    //     nwocg.P_gain = nwocg.Add1 * 3;
    //     nwocg.Add2 = nwocg.Ts + nwocg.UnitDelay1;
    //     nwocg.Add3 = nwocg.P_gain + nwocg.Add2;
    //     nwocg.UnitDelay1 = nwocg.Add2;
    // }

    // 4. ext_ports
    oss << "static const nwocg_ExtPort ext_ports[] =\n{\n";
    for (const auto& block : blocks) {
        if (block.blockType == enums::BlockType::Outport) {
            oss << "\t{ \"" << block.Name << "\", &nwocg." << removeSpaces(block.Name) << ", 0 },\n";
        }
    }
    for (const auto& block : blocks) {
        if (block.blockType == enums::BlockType::Inport) {
            oss << "\t{ \"" << block.Name << "\", &nwocg." << removeSpaces(block.Name) << ", 1 },\n";
        }
    }
    oss << "\t{ 0, 0, 0 },\n};\n";

    oss << "const nwocg_ExtPort * const nwocg_generated_ext_ports = ext_ports;\n";
    oss << "const size_t nwocg_generated_ext_ports_size = sizeof(ext_ports);\n";    

    file << oss.str();
}
