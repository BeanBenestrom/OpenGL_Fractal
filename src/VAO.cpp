#include "VAO.h"


VAO::VAO()
{
    glGenVertexArrays(1, &_id);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &_id);
}

void VAO::link_atribute(
    VBO &VBO, 
    GLuint layout, 
    GLuint componentNumber, 
    GLenum type, 
    GLboolean normalized, 
    GLsizei stride, 
    const GLvoid* offset
)
{
    glBindVertexArray(_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO.get_id());
    glVertexAttribPointer(layout, componentNumber, type, normalized, stride, offset);
    glEnableVertexAttribArray(layout);
}

void VAO::link_EBO(EBO &EBO)
{
    glBindVertexArray(_id);
    EBO.bind();
}

void VAO::bind() 
{ 
    glBindVertexArray(_id); 
}

void VAO::un_bind() 
{
    glBindVertexArray(0); 
}

void VAO::destroy()
{
    glDeleteVertexArrays(1, &_id);
}