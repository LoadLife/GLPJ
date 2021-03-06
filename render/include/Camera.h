#pragma once
#include <iostream>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "element.h"
class Camera
{
	
public:
	glm::vec3 pos_;
	Camera(glm::vec3 &pos,glm::vec3 &target);
	void updatePos(Direction dir);
	void updateMouse(float xoffset,float yoffset);
	glm::mat4 getLookAt();
	~Camera();
private:
	glm::vec3 front_;
	glm::vec3 up_;
	float yaw_;
	float pitch_;
};

