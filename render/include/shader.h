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
	//����shader
	shader(const std::string vertexPath, const std::string fragmentPath);


	//ʹ��shader
	void use();

	//����uniform����
	//��������
	void setBool(const std::string &name, bool value)const;
	//int����
	void setInt(const std::string &name, int value)const;
	//Float����
	void setFloat(const std::string &name, float value)const;
	//mat4����
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	//����
	~shader();
private:




};
