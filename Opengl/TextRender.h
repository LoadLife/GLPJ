#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <memory>
#include "shader.h"
#include "element.h"
using namespace std;
class TextRender
{
public:
	TextRender(shared_ptr<shader> shader);
	~TextRender();
	void draw(string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) const;
private:
	void init();
	GLuint VAO;
	GLuint VBO;
	shared_ptr<shader> mshader;
	glm::mat4 projection;
};

