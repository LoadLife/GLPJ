#include "Quad.h"

Quad::Quad(shared_ptr<shader>& shader, vector<vertex>& vertices, vector<GLuint>& indices,vector<string>& texturePathes)
{
	shader_ = shader;
	vertices_ = vertices;
	indices_ = indices;
	texturePathes_ = texturePathes;
	init();
}

void Quad::init() {
	GLuint VBO, EBO;
	glGenVertexArrays(1, &VAO_);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertex), &vertices_[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), &indices_[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	vector<GLuint> textures(texturePathes_.size());
	textures_ = textures;
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(textures_.size(), &textures_[0]);
	
	for (int i = 0; i < static_cast<int>(textures_.size()); i++) {
		unsigned char* data = stbi_load(texturePathes_[i].c_str(), &width, &height, &nrChannels, 0);
		glBindTexture(GL_TEXTURE_2D, textures_[i]);
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
			std::cout << "Fail to load texture:" << texturePathes_[i];
		stbi_image_free(data);
	}
	shader_->use();
	GLuint matricesBlockIndex = glGetUniformBlockIndex(shader_->program_, "matrices");
	glUniformBlockBinding(shader_->program_, matricesBlockIndex, 0);
}


void Quad::draw(glm::mat4& model,glm::vec3& camPos) const{

	shader_->use();
	shader_->setInt("texture0", 0);
	shader_->setVec3("lightPos", 0.0f, 0.6f, -0.2f);
	shader_->setVec3("lightColor", 241.0f / 255.0f, 158.0f / 255.0f, 194.0f / 255.0f);
	shader_->setMat4("modelM", model);
	shader_->setVec3("camPos", camPos.x, camPos.y, camPos.z);

	for (int i = 0; i < static_cast<int>(textures_.size()); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures_[i]);
	}
	glBindVertexArray(VAO_);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


Quad::~Quad()
{
}
