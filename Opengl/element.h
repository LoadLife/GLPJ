#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <map>

struct  vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 tPos;
};

enum Direction
{
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


