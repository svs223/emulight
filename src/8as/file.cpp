#include <cstdint>

#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <bit>

#include "file.hpp"

using std::string;
using std::endl;

AsmFile::AsmFile()
{
}

bool AsmFile::load(std::string filename)
{
    std::ifstream file(filename, std::ios::in);
    if (!file.is_open())
        return false;
    assembly << file.rdbuf();

    return true;
}

void AsmFile::assemble()
{
    string line;
    while(std::getline(assembly, line))
    {
        if (line[0] == ';')
            continue;
    
        binary.push_back(decode(line));
    }

    for(uint16_t b : binary)
    {
        printf("binary: %04X\n", b);
    }
}

uint16_t AsmFile::decode(string pneum)
{
    std::stringstream ss(pneum);
    string str;
    uint16_t work;
    getline(ss, str, ' ');
    if (str == "cls"){
        work = 0x00E0;
        std::cerr << "cls" << '\n';
    }else if (str == "rt"){
        work = 0x00EE;
        std::cerr << "rt" << '\n';
    }else if (str == "jp"){
        work = 1 << 12;
        std::cerr << "jp" << '\n';
        getline(ss, str, ' '); // jp NNN <- here
        int i = std::atoi(str.c_str());
        if (i > 0x0FFF){
            std::cout << "Too Large of a jump" << endl;
            return 0;
        }
        work |= i;
        // printf("%04X\n", work);
    } else if (str == "call"){
        std::cerr << "call" << '\n';
        work = 0x2000;
        getline(ss, str, ' '); // call NNN <- here
        int i = std::atoi(str.c_str());
        if (i > 0x0FFF){
            std::cerr << "Too Large of a jump" << endl;
            return 0;
        }
    }else {
        std::cerr << "ukn" << endl;
        //work = 0;
    }

    printf("work: %04X\n", work);

    return work;
}

void AsmFile::write(string filename)
{
    std::ofstream file(filename, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "File could not be opened" << endl;
        return;
    }

    const uint8_t *a8 = v16_to_a8();

    file.write(reinterpret_cast<const char*>(a8), binary.size() * 2);
    file.close();


    return;
}

uint8_t *AsmFile::v16_to_a8()
{
    uint8_t *a8 = new uint8_t[binary.size() * 2];
    uint32_t i = 0;
    for(uint16_t w : binary)
    {
        a8[++i] = (w >> 8u) & 0xFF;
        a8[++i] = w & 0xFF;
    }

    return a8;
}


AsmFile::AsmFile(std::string filename)
{
    load(filename);
}

AsmFile::~AsmFile()
{
}