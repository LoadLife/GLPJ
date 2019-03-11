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
class Quad
{
public:

	Quad(shared_ptr<shader>& mshader,vector<vertex>& vertices,vector<GLuint>& indices,vector<string>& texturePathes);
	void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection,glm::vec3& camPos);
	~Quad();
private:
	GLuint VAO;
	shared_ptr<shader> mshader;
	vector<vertex> vertices;
	vector<GLuint> indices;
	void init();
	vector<string> texturePathes;
	vector<GLuint> textures;
};

