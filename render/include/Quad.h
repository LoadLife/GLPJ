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
#include "stb_image.h"
using namespace std;
class Quad :public SceneObj
{
public:

	Quad(shared_ptr<shader>& shader,vector<vertex>& vertices,vector<GLuint>& indices,vector<string>& texturePathes);
	void draw(glm::mat4& model,glm::vec3& camPos) const;
	~Quad();
private:
	GLuint VAO_;
	shared_ptr<shader> shader_;
	vector<vertex> vertices_;
	vector<GLuint> indices_;
	void init() override;
	vector<string> texturePathes_;
	vector<GLuint> textures_;
};

