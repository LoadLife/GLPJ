#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <memory>
#include "element.h"
#include "shader.h"
#include "SceneObj.h"
using namespace std;
class Ball:public SceneObj
{
public:
	Ball(unique_ptr<shader>& shader) : 
		mshader(std::move(shader))
	{
		init();
	}
	~Ball() {}
	void draw(glm::mat4& model) const;
private: 
	GLuint VAO;
	unique_ptr<shader> mshader;
	vector<vertex> vertices;
	void init() override;
};

