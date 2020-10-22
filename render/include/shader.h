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
	//构造shader
	shader(const std::string vertexPath, const std::string fragmentPath);


	//使用shader
	void use();

	//设置uniform变量
	//布尔类型
	void setBool(const std::string &name, bool value)const;
	//int类型
	void setInt(const std::string &name, int value)const;
	//Float类型
	void setFloat(const std::string &name, float value)const;
	//mat4类型
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	//析构
	~shader();
private:




};
