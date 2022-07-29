#pragma once

#include <glad/glad.h>

#include "VBO.h"
#include "EBO.h"

class VAO
{
private:
    GLuint _id = 0;

public:
    VAO();
    ~VAO();

    void link_atribute(
        VBO &VBO, 
        GLuint layout, 
        GLuint componentNumber, 
        GLenum type, 
        GLboolean normalized, 
        GLsizei stride, 
        const GLvoid* offset
    );
    void link_EBO(EBO &EBO);

    void bind();
    void un_bind();
    void destroy();

    GLuint inline get_id() { return _id; }
};