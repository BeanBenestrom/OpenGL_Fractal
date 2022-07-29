#include "VBO.h"


VBO::VBO(const GLfloat* vertices, GLsizeiptr size)
{
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    this->_size = size;
}

VBO::~VBO()
{
    glDeleteBuffers(1, &_id);
}

void VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VBO::un_bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::destroy()
{
    glDeleteBuffers(1, &_id);
}