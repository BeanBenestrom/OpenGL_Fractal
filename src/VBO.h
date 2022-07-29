#pragma once

#include <glad/glad.h>

class VBO
{
private:
    GLuint _id = 0;
    GLsizeiptr _size;

public:
    VBO(const GLfloat* vertices, GLsizeiptr size);
    ~VBO();

    void bind();
    void un_bind();
    void destroy();

    GLsizeiptr inline get_size() { return _size; }
    GLuint inline get_id() { return _id; }
};