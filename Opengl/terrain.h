#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <memory>
#include "element.h"
#include "shader.h"
#include "stb_image.h"
using namespace std;

class terrain
{
public:
	terrain(const char* MapPath,const char* TexturePath,shared_ptr<shader>& shader);
	void draw(glm::mat4& model, glm::vec3& camPos) const;
	~terrain();
	
private:
	void init(const char* MapPath, const char* TexturePath);
	vector<vertex> vertices;
	GLuint VAO;
	GLuint texture;
	shared_ptr<shader> mshader;
};

