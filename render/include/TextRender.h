
#pragma once
#include "ft2build.h"
#include FT_FREETYPE_H
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <memory>
#include "shader.h"
#include "element.h"
#include "SceneObj.h"
using namespace std;
class TextRender:public SceneObj
{
public:
	TextRender(unique_ptr<shader>& shader);
	~TextRender();
	void draw(string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) const;
private:
	void init() override;
	GLuint VAO_;
	GLuint VBO_;
	unique_ptr<shader> shader_;
	glm::mat4 projection_;
};

