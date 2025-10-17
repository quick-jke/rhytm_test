#include "scheme.hpp"


std::string P::to_string() const {
    std::ostringstream oss;
    oss << "P{Name=\"" << PNameToString(Name) << "\", value=";
    if (std::holds_alternative<std::string>(value)) {
        oss << std::get<std::string>(value);
    } else if(std::holds_alternative<std::vector<int>>(value)){
        const auto& vec = std::get<std::vector<int>>(value);
        oss << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << vec[i];
        }
        oss << "]";
    } 
    else if(std::holds_alternative<PDirectionValue>(value)){
        oss << std::get<PDirectionValue>(value).to_string();
    }
    else if(std::holds_alternative<SignPair>(value)){
        oss << std::get<SignPair>(value).to_string();
    }
    oss << "}";
    return oss.str();
}

std::string Port::to_string() const {
    std::ostringstream oss;
    oss << "Port{p=[";
    for (size_t i = 0; i < p.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << p[i].to_string();
    }
    oss << "]}";
    return oss.str();
}


std::string Branch::to_string() const {
    std::ostringstream oss;
    oss << "Branch{p=[";
    for (size_t i = 0; i < p.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << p[i].to_string();
    }
    oss << "]}";
    return oss.str();
}

std::string Block::to_string() const {
    std::ostringstream oss;
    oss << "Block{blockType=" << blockTypeToString(blockType)
        << ", Name=\"" << Name << "\", SID=" << SID
        << ", p=[";
    for (size_t i = 0; i < p.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << p[i].to_string();
    }
    oss << "], ports=[";
    for (size_t i = 0; i < ports.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << ports[i].to_string();
    }
    oss << "]}";
    return oss.str();
}

std::string Line::to_string() const {
    std::ostringstream oss;
    oss << "Line{p=[";
    for (size_t i = 0; i < p.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << p[i].to_string();
    }
    oss << "], branches=[";
    for (size_t i = 0; i < branches.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << branches[i].to_string();
    }
    oss << "]}";
    return oss.str();
}

std::string System::to_string() const {
    std::ostringstream oss;
    oss << "System{blocks=[\n";
    for (size_t i = 0; i < blocks.size(); ++i) {
        if (i > 0) oss << ",\n";
        oss << "  " << blocks[i].to_string();
    }
    oss << "\n], \nlines=[\n";
    for (size_t i = 0; i < lines.size(); ++i) {
        if (i > 0) oss << ",\n";
        oss << "  " << lines[i].to_string();
    }
    oss << "\n]}";
    return oss.str();
}

bool SignPair::isValid() const {
    bool first_ok = !first.has_value() || 
                    (*first == '+' || *first == '-');
    
    bool second_ok = !second.has_value() || 
                        (*second == '+' || *second == '-');

    return first_ok && second_ok;
}

std::string SignPair::to_string() const {
    std::string result;
    if (first)  result += *first;
    if (second) result += *second;
    return "SignPair:" + result;
}

std::string PDirectionValue::to_string() const{
    std::ostringstream oss;
    oss << "PDirectionValue{SID=" << SID << ", dir=" << PDirectionToString(dir) << ", port=" << port << "}";
    return oss.str();
}


std::string System::content(){
    std::stringstream oss;
    oss << "#include \"nwocg_run.h\"\n#include <math.h>\n";

    oss << "static struct\n{\n";

    for(auto const& block : blocks){
        oss << "\tdouble " << removeSpaces(block.Name) << ";\n";
    }

    oss << "} nwocg;\n";

    oss << "void nwocg_generated_init()\n{\n";

    oss << "}\nvoid nwocg_generated_step()\n{\n";

    oss << "}\nstatic const nwocg_ExtPort ext_ports[] =\n{\n";

    oss << "};\nconst nwocg_ExtPort * const nwocg_generated_ext_ports = ext_ports;\nconst size_t nwocg_generated_ext_ports_size = sizeof(ext_ports);";

    return oss.str();
}


