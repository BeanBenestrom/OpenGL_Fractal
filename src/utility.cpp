#include <fstream>
#include <iostream>
#include<algorithm>

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

    int load_shader_source_from_file(std::string &container, const char* path, bool version_430)
    {
        std::ifstream file;
        std::string fractal_settings;
        int success = load_text_from_file(fractal_settings, "Fractal_Settings.vs");

        file.open(path, std::ios::in);

        if (!file.is_open()) { 
            std::cout << "[*] UTILITY - Could not load shader source file\n" 
                      << "    PATH: " << path << "\n";
            return 0; 
        }
        if (success == 0) { 
            std::cout << "[*] UTILITY - Could not load fractal settings file\n" 
                      << "    PATH: Fractal_Settings.vs" << "\n\n HENCE\n";
            return 0; 
        }

        container = "";         // Makes sure that container is empty
        std::string snippet;    // Line of text
        size_t index; 
        size_t index2;
        size_t index3;
        
        while (std::getline(file, snippet)) 
        { 
            index = snippet.find("<version>");
            if (index != std::string::npos)
            {
                if (version_430) { snippet.replace(snippet.begin() + index, snippet.begin() + index + 9, "430"); } 
                else { snippet.replace(snippet.begin() + index, snippet.begin() + index + 9, "330"); }
            }

            index = snippet.find("<MandelBrot>");
            if (index != std::string::npos)
            {
                index2 = fractal_settings.find("// <MandelBrot>");
                index3 = fractal_settings.find("// <Julia Set>");
                std::string code = fractal_settings.substr(index2, index3 - index2);

                snippet.replace(snippet.begin() + index, snippet.begin() + index + 12, code);
            }

            index = snippet.find("<Julia Set>");
            if (index != std::string::npos)
            {
                index2 = fractal_settings.find("// <Julia Set>");
                index3 = fractal_settings.find("// <Color>");
                std::string code = fractal_settings.substr(index2, index3 - index2);

                snippet.replace(snippet.begin() + index, snippet.begin() + index + 11, code);
            }

            index = snippet.find("<Color>");
            if (index != std::string::npos)
            {
                index2 = fractal_settings.find("// <Color>");
                index3 = fractal_settings.find("// <END>");
                std::string code = fractal_settings.substr(index2, index3 - index2);

                snippet.replace(snippet.begin() + index, snippet.begin() + index + 7, code);
            }

            container += snippet + "\n";
        }
        file.close();
        return 1;
    }
}