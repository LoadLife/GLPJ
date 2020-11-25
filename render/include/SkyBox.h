#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>
#include <memory>
#include "SceneObj.h"
#include "stb_image.h"
#include "shader.h"
using namespace std;
class SkyBox:SceneObj
{
public:
	SkyBox(unique_ptr<shader>& shader) : 
		shader_(std::move(shader))
	{
		init();
	}
	~SkyBox();
	void draw();
private:
	unique_ptr<shader> shader_;
	GLuint texture_;
	GLuint VAO_;
	void init() override;
};

