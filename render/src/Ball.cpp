#include "Ball.h"

void Ball::init() {
	//initial ball position
	for (float i = 90.0f; i >= -80; i -= 10.0f) {
		for (float j = 0.0f; j <= 360.0f; j += 20.0f) {
			float x1 = cos(glm::radians(i))*cos(glm::radians(j));
			float y1 = sin(glm::radians(i));
			float z1 = cos(glm::radians(i))*sin(glm::radians(j));

			float x2 = cos(glm::radians(i - 10.0f))*cos(glm::radians(j));
			float y2 = sin(glm::radians(i - 10.0f));
			float z2 = cos(glm::radians(i - 10.0f))*sin(glm::radians(j));

			vertex v1 = { glm::vec3(x1,y1,z1) };
			vertex v2 = { glm::vec3(x2,y2,z2) };
			vertices_.push_back(v1);
			vertices_.push_back(v2);
		}
	}

	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertex), &vertices_[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	shader_->use();
	//bind index to point
	GLuint matricesBlockIndex = glGetUniformBlockIndex(shader_->program_, "Matrices");
	glUniformBlockBinding(shader_->program_, matricesBlockIndex, 0);
	//set ball's color
	shader_->setVec3("lightColor", 241.0f / 255.0f*1.5f, 155.0f / 255.0f*1.5f, 194.0f / 255.0f*1.5f);
}

void Ball::draw(glm::mat4& model) const {
	shader_->use();
	shader_->setMat4("modelM", model);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices_.size());
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
