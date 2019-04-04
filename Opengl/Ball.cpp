#include "Ball.h"



Ball::Ball(shared_ptr<shader> mshader)
{
	this->mshader = mshader;
	init();
}
void Ball::init() {
	//初始化球顶点坐标
	for (float i = 90.0f; i >= -80; i -= 10.0f) {
		for (float j = 0.0f; j <= 360.0f; j += 20.0f) {
			float x1 = cos(glm::radians(i))*cos(glm::radians(j));
			float x2 = cos(glm::radians(i - 10.0f))*cos(glm::radians(j));
			float y1 = sin(glm::radians(i));
			float y2 = sin(glm::radians(i - 10.0f));
			float z1 = cos(glm::radians(i))*sin(glm::radians(j));
			float z2 = cos(glm::radians(i - 10.0f))*sin(glm::radians(j));
			vertex v1 = { glm::vec3(x1,y1,z1) };
			vertex v2 = { glm::vec3(x2,y2,z2) };
			this->vertices.push_back(v1);
			this->vertices.push_back(v2);
		}
	}

	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	mshader->use();
	//绑定块索引到绑定点
	GLuint matricesBlockIndex = glGetUniformBlockIndex(mshader->Program, "Matrices");
	glUniformBlockBinding(mshader->Program, matricesBlockIndex, 0);
	//设置球颜色
	mshader->setVec3("lightColor", 241.0f / 255.0f*1.5f, 155.0f / 255.0f*1.5f, 194.0f / 255.0f*1.5f);

	
}
void Ball::draw(glm::mat4& model) {
	mshader->use();
	mshader->setMat4("modelM", model);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
Ball::~Ball()
{
}
