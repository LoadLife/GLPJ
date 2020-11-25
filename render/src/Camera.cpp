#include "Camera.h"

Camera::Camera(glm::vec3& pos, glm::vec3& target)
{
	pos_ = pos;
	up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw_ = -90.0f;
	pitch_ = 0.0f;
	front_ = glm::vec3(cos(glm::radians(pitch_)) * cos(glm::radians(yaw_)), 
            sin(glm::radians(pitch_)), cos(glm::radians(pitch_)) * sin(glm::radians(yaw_)));
}

void Camera::updatePos(Direction dir) {

	float speed = 0.0015f * 30.0f;
	if (dir == FRONT)
		pos_ += speed * front_;
	else if (dir == BACK)
		pos_ -= speed * front_;
	else if (dir == LEFT)
		pos_ -= speed * glm::normalize(glm::cross(front_, up_));
	else if (dir == RIGHT)
		pos_ += speed * glm::normalize(glm::cross(front_, up_));
		
}

void Camera::updateMouse(float xoffset, float yoffset) {
	float MouseSensitivity = 0.016f;
	yaw_ += xoffset * MouseSensitivity;
	pitch_ += yoffset * MouseSensitivity;
	if (pitch_ > 89.0f)
		pitch_ = 89.0f;
	if (pitch_ < -89.0f)
		pitch_ = -89.0f;
	front_ = glm::vec3(cos(glm::radians(pitch_)) * cos(glm::radians(yaw_)), 
            sin(glm::radians(pitch_)), cos(glm::radians(pitch_)) * sin(glm::radians(yaw_)));
}

glm::mat4 Camera::getLookAt() {
	glm::mat4 view = glm::lookAt(pos_, pos_ + front_, up_);
	return view;
}

Camera::~Camera()
{
}
