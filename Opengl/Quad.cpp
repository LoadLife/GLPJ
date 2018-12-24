#include "Quad.h"


Quad::Quad(shader* mshader, vector<vertex> vertices, vector<GLuint> indices,vector<string> texturePathes)
{
	this->mshader = mshader;
	this->vertices = vertices;
	this->indices = indices;
	this->texturePathes = texturePathes;
	init();
}

void Quad::init() {
	//配置顶点属性 以及索引属性
	GLuint VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//初始化纹理
	vector<GLuint> textures(this->texturePathes.size());
	this->textures = textures;
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(this->textures.size(), &textures[0]);
	
	for (int i = 0; i < this->textures.size(); i++) {
		unsigned char* data = stbi_load(this->texturePathes[i].c_str(), &width, &height, &nrChannels, 0);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			std::cout << "Fail to load texture:" << texturePathes[i];
		stbi_image_free(data);
		this->mshader->use();
		this->mshader->setInt("texture" + to_string(i), i);
		this->mshader->setVec3("lightPos",0.0f,0.6f,-0.2f);
		this->mshader->setVec3("lightColor", 241.0f / 255.0f, 158.0f / 255.0f, 194.0f / 255.0f);
	}
}


void Quad::draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::vec3 camPos) {

	this->mshader->use();
	this->mshader->setMat4("modelM", model);
	this->mshader->setMat4("viewM", view);
	this->mshader->setMat4("projectionM", projection);
	this->mshader->setVec3("camPos", camPos.x, camPos.y, camPos.z);

	for (int i = 0; i < this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
	}
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


Quad::~Quad()
{
	delete this->mshader;
}
