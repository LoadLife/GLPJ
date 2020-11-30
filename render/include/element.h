#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <map>
#include <string>

struct vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 tPos;
};

enum Direction {
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

struct Character {
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

extern std::map<GLchar, Character> Characters;

inline GLenum glErrorChecker(const char* file_name, int line_num) {
    GLenum errorCode = GL_NO_ERROR;
    while ((errorCode = glGetError() != GL_NO_ERROR))
    {
        std::string error;
        switch(errorCode) {
            case GL_INVALID_ENUM:                    error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                   error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:               error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                  error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:                 error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                   error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:   error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << std::string(file_name) << "(" << line_num << ")" << std::endl;
    }
    return errorCode;
}

#define GET_LAST_ERROR() glErrorCheck(__FILE__, __LINE__)