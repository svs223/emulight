#include <iostream>
#include <string>
#include <vector>

#include "file.hpp"

using std::vector;
using std::string;
using std::endl;

int main(int argc, char **argv)
{
    if (argc < 2){
        printf("Too few arguments\n");
        return 1;
    }
    string inf = string(argv[1]);
    AsmFile f(inf);
    f.assemble();
    f.write("out.ch8");
}