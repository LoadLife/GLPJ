#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <GL\glew.h>

class  shader
{
public:
	GLuint Program;
	shader(const std::string vertexPath, const std::string fragmentPath);
	void use();
	void setBool(const std::string &name, bool value)const;
	void setInt(const std::string &name, int value)const;
	void setFloat(const std::string &name, float value)const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	~shader();
};
