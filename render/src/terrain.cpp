#include "terrain.h"



terrain::terrain(const char* MapPath, const char* TexturePath,shared_ptr<shader>& shader)
{
	mshader = shader;
	init(MapPath, TexturePath);
		
}



void terrain::init(const char* MapPath, const char* TexturePath) {

	int width, height, nrChannels;
	unsigned char* mData = stbi_load(MapPath, &width, &height, &nrChannels, 0);
	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width; j++) {
			vertex v1, v2;
			if (i % 2 == 0) {
				v1.pos = glm::vec3(j, (float)0.002*mData[(i*width + j) * 3], i);
				v1.tPos = glm::vec2(static_cast<float>(j) / static_cast<float>(width - 1), 1.0f - static_cast<float>(i) / static_cast<float>(height - 1));
				v2.pos = glm::vec3(j, (float)0.002*mData[((i + 1)*width + j) * 3], i + 1);
				v2.tPos = glm::vec2(static_cast<float>(j) / static_cast<float>(width - 1), 1.0f - static_cast<float>(i + 1) / static_cast<float>(height - 1));

				this->vertices.push_back(v1);
				this->vertices.push_back(v2);
				
			}
			else {
				v1.pos = glm::vec3(width - 1 - j, (float)0.002*mData[(i*width + width - 1 - j) * 3], i);
				v1.tPos = glm::vec2(static_cast<float>(width - 1 - j) / static_cast<float>(width - 1), 1.0f - static_cast<float>(i) / static_cast<float>(height - 1));
				v2.pos = glm::vec3(width - 1 - j, (float)0.002*mData[(((i + 1)*width) + width - 1 - j) * 3], i + 1);
				v2.tPos = glm::vec2(static_cast<float>(width - 1 - j) / static_cast<float>(width - 1), 1.0f - static_cast<float>(i + 1) / static_cast<float>(height - 1));
				this->vertices.push_back(v1);
				this->vertices.push_back(v2);

			}

		}
	}
	stbi_image_free(mData);
	GLuint VBO;
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
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
	
	
	//³õÊ¼»¯ÎÆÀí
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &this->texture);
    mData = stbi_load(TexturePath, &width, &height, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	if (mData) {  
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width,height, 0, GL_RGB, GL_UNSIGNED_BYTE, mData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fail to load texture" << endl;
	
	stbi_image_free(mData);
	mshader->use();
	GLuint matricesBlockIndex = glGetUniformBlockIndex(mshader->Program, "matrices");
	glUniformBlockBinding(mshader->Program, matricesBlockIndex, 0);
	
}

void terrain::draw(glm::mat4& model, glm::vec3& camPos)  const
{
	mshader->use();
	mshader->setVec3("lightColor", 241.0f / 255.0f*1.5f, 155.0f / 255.0f*1.5f, 194.0f / 255.0f*1.5f);
	mshader->setInt("texture0", 0);
	mshader->setVec3("lightPos", 0.0f, 0.6f, -0.2f);

	mshader->setMat4("modelM", model);
	mshader->setVec3("camPos", camPos.x, camPos.y, camPos.z);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, this->vertices.size());
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}
terrain::~terrain()
{
}
