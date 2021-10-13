/*
Copyright (c) 2020-2021 Bruno Moretto M.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// <<<< EXAMPLE 1 >>>>
/* 
// Be sure to include shdiopp.h after a gl declaration header, such as glew!
#include <GL/glew.h>

#define SHDIOPP_IMPLEMENTATION
#include "shdiopp.h"

int main()
{
    ...

    const char* vertexShader = "..."
    const char* fragmentShader = "..."

    shdiopp::Program p = shdiopp::Program::loadRaw(vertexShader, fragmentShader);

    glUseProgram(p.getId());

    ...
}
*/

// <<<< EXAMPLE 2 >>>>
/* 
// Be sure to include shdiopp.h after a gl declaration header, such as glew!
#include <GL/glew.h>

#define SHDIOPP_IMPLEMENTATION
#include "shdiopp.h"

int main()
{
    ...

    Program p = Program::loadFile("res/shader/vertex.vert", "res/shader/fragment.frag");

    glUseProgram(p.getId());

    ...
}
*/

// ====================================================================================
// ================================== DECLARATION =====================================
// ====================================================================================

#ifndef SHDIOPP_H
#define SHDIOPP_H

namespace shdiopp
{
    class Program
    {
    private:
        GLuint id;
        // TODO: Isso aqui seria util pra debug: 
        // const char const* vertexPath, fragmentPath; // TODO: Pra inicializar esses membros const precisa de initilizer list 
    
    public:
        static Program loadRaw(const char* vertexSrc, const char* fragmentSrc);
        // TODO: static Program loadRaw(const char* vertexSrc, const char* geometrySrc, const char* fragmentSrc);
        static Program loadFile(const char* vertexPath, const char* fragmentPath);
        // TODO: static Program loadFile(const char* vertexSrc, const char* geometryPath, const char* fragmentSrc);
        GLuint getId();

    private:
        Program();
        static void compileShader(GLuint id, size_t count, const char** src);
        static void fileCat(const char* path, size_t size, char* buffer);
    };
}

#endif

// ====================================================================================
// ================================ IMPLEMENTATION ====================================
// ====================================================================================

#ifdef SHDIOPP_IMPLEMENTATION

#include <stdio.h>
#include <string.h>

namespace shdiopp
{
    Program Program::loadRaw(const char* vertexSrc, const char* fragmentSrc)
    {
        // Shader program will be used to bound the shaders:
        Program program;
        program.id = glCreateProgram();

        GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
        compileShader(vertexId, 1, &vertexSrc);
        glAttachShader(program.id, vertexId);

        GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
        compileShader(fragmentId, 1, &fragmentSrc);
        glAttachShader(program.id, fragmentId);

        // Shaders compiled code linking:
        glLinkProgram(program.id);

        // Checking for shader linking errors:
        int success;
        glGetProgramiv(program.id, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(program.id, sizeof(infoLog), NULL, infoLog);

            printf("%s\n", infoLog);
        }

        // We can delete compiled shader objects after linking:
        glDeleteShader(vertexId);
        glDeleteShader(fragmentId);

        return program;
    }

    Program Program::loadFile(const char* vertexPath, const char* fragmentPath)
    {
        size_t maxSize = 512 * 100;

        char* vertexSrc = new char[maxSize];
        memset(vertexSrc, 0, maxSize);
        fileCat(vertexPath, maxSize, vertexSrc);

        char* fragmentSrc = new char[maxSize];
        memset(fragmentSrc, 0, maxSize);
        fileCat(fragmentPath, maxSize, fragmentSrc);

        Program program = loadRaw(vertexSrc, fragmentSrc);

        delete[] vertexSrc;
        delete[] fragmentSrc;

        return program;
    }

    GLuint Program::getId()
    {
        return id;
    }

    Program::Program() // : id(0)
    {
        id = 0;
    }

    void Program::compileShader(GLuint id, size_t count, const char** src)
    {
        glShaderSource(id, count, src, NULL);
        glCompileShader(id);

        // Checking for shader compiling errors:
        int success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);

            printf("%s\n", infoLog);
        }
    }
    
    // Buffer needs to be zero-initialized!
    void Program::fileCat(const char* path, size_t size, char* buffer)
    {
        FILE* file = fopen(path, "r");

        // Check if file != NULL to please the compiler:
        if (file)
        {
            char lineBuffer[512];
            while (fgets(lineBuffer, sizeof(lineBuffer), file))
                strncat(buffer, lineBuffer, size - strlen(buffer));

            fclose(file);
        }
        else
            printf("Could not open %s\n", path);
    }
}

#endif