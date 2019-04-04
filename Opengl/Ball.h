#pragma once
#include<GL/glew.h>
#include<vector>
#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtc\type_ptr.hpp>
#include<memory>
#include"element.h"
#include"shader.h"
#include"stb_image.h"
using namespace std;
class Ball
{
public:
	Ball(shared_ptr<shader> shader);
	~Ball();
	void draw(glm::mat4& model);
private: 
	GLuint VAO;
	shared_ptr<shader> mshader;
	vector<vertex> vertices;
	void init();
};

