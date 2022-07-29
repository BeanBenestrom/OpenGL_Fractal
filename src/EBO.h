#pragma once

#include <glad/glad.h>

class EBO
{
private:
    GLuint _id = 0;
    GLsizeiptr _size;

public:
    EBO(const GLuint* indices, GLsizeiptr size);
    ~EBO();

    void bind();
    void un_bind();
    void destroy();

    GLsizeiptr inline get_size() { return _size; }
    GLuint inline get_id() { return _id; }
};