#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>
#include <memory>
#include "SceneObj.h"
#include "shader.h"
#include "stb_image.h"
using namespace std;
class SkyBox:SceneObj
{
public:
	SkyBox(shared_ptr<shader> shader);
	~SkyBox();
	void draw();
private:
	shared_ptr<shader> mshader;
	GLuint texture;
	GLuint VAO;
	void init() override;
};

