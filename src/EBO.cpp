#include "EBO.h"


EBO::EBO(const GLuint* indices, GLsizeiptr size)
{
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);

    this->_size = size;
}

EBO::~EBO()
{
    glDeleteBuffers(1, &_id);
}

void EBO::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void EBO::un_bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::destroy()
{
    glDeleteBuffers(1, &_id);
}