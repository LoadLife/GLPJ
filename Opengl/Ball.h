#pragma once
#include<GL/glew.h>
#include<vector>
#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtc\type_ptr.hpp>
#include"element.h"
#include"shader.h"
#include"stb_image.h"
using namespace std;
class Ball
{
public:
	Ball(shader* shader);
	~Ball();
	void draw(glm::mat4 model,glm::mat4 view, glm::mat4 projection);
private: 
	GLuint VAO;
	shader* mshader;
	vector<vertex> vertices;
	void init();
};

