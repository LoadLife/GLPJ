#pragma once
#include <glm/glm.hpp>
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