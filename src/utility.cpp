#include <fstream>
// #include <iostream>

#include "utility.h"


namespace utility 
{
    int load_text_from_file(std::string &container, const char* path)
    {
        std::ifstream file;
        file.open(path, std::ios::in);

        if (!file.is_open()) { return 0; }

        container = "";         // Makes sure that container is empty
        std::string snippet;    // Line of text     
        
        while (std::getline(file, snippet)) 
        { 
            container += snippet + "\n";
        }
        file.close();
        return 1;
    }
}