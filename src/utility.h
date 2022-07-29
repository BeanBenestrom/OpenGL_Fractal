#pragma once

#include <string>

#include <glad/glad.h>

namespace utility 
{
    int load_text_from_file(std::string &container, const char* path);
}