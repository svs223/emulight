#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class AsmFile {
    public:
        AsmFile(std::string);
        AsmFile();
        bool load(std::string);
        void assemble();
        void write(std::string);
        ~AsmFile();

    private:
        std::string filename;
        std::stringstream assembly;
        std::ofstream executable;
        std::vector<uint16_t> binary;
        uint8_t *v16_to_a8();
        uint16_t decode(std::string);
};