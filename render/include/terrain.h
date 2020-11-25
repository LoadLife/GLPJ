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
	terrain(shared_ptr<shader>& shader, const string MapPath,const string TexturePath) :
			shader_(shader)
	{
		init(MapPath, TexturePath);
	}

	void draw(glm::mat4& model, glm::vec3& camPos) const;
	~terrain();
	
private:
	void init(const string MapPath, const string TexturePath);
	vector<vertex> vertices_;
	GLuint VAO_;
	GLuint texture_;
	shared_ptr<shader> shader_;
};

