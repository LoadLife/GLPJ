#include "Camera.h"



Camera::Camera(glm::vec3& pos, glm::vec3& target)
{
	this->pos = pos;
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->yaw = -90.0f;
	this->pitch = 0.0f;
	this->front = glm::vec3(cos(glm::radians(pitch))*cos(glm::radians(yaw)),sin(glm::radians(pitch)),cos(glm::radians(pitch))*sin(glm::radians(yaw)));
}

void Camera::updatePos(Direction dir) {

	float speed = 0.0015f*10;
	if (dir == FRONT)
		this->pos += speed*this->front;
	else if (dir == BACK)
		this->pos -= speed*this->front;
	else if (dir == LEFT)
		this->pos -= speed*glm::normalize(glm::cross(this->front, this->up));
	else if (dir == RIGHT)
		this->pos += speed*glm::normalize(glm::cross(this->front, this->up));
		
}
void Camera::updateMouse(float xoffset, float yoffset) {
	float MouseSensitivity = 0.016f;
	yaw += xoffset*MouseSensitivity;
	pitch += yoffset*MouseSensitivity;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	this->front = glm::vec3(cos(glm::radians(pitch))*cos(glm::radians(yaw)), sin(glm::radians(pitch)), cos(glm::radians(pitch))*sin(glm::radians(yaw)));
}

glm::mat4 Camera::getLookAt() {
	glm::mat4 view = glm::lookAt(pos, pos + front, up);
	return view;
}
Camera::~Camera()
{
}
