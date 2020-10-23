#define GLEW_STATIC
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <memory>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <map>
#include "config.h"
#include "Quad.h"
#include "element.h"
#include "shader.h"
#include "Camera.h"
#include "Ball.h"
#include "terrain.h"
#include "TextRender.h"
#include "SkyBox.h"

void ProcessInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
using namespace std;
Camera cam(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "July", nullptr, nullptr);

	if (window == nullptr) {
		cout << "Fail to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "Failed to initialize GLEW" << endl;
		return -1;

	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	const GLubyte* firm = glGetString(GL_VENDOR);
	const GLubyte* identifier = glGetString(GL_RENDERER);
	printf("Realize By %s\n", firm);
	printf("Run On %s\n", identifier);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	vertex vertex1 = { glm::vec3(-0.2f, 0.2f, 0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec2(0.0f,1.0f) };
	vertex vertex2 = { glm::vec3( 0.2f, 0.2f, 0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec2(1.0f,1.0f) };
	vertex vertex3 = { glm::vec3( 0.2f,-0.2f, 0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec2(1.0f,0.0f) };
	vertex vertex4 = { glm::vec3(-0.2f,-0.2f, 0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec2(0.0f,0.0f) };
	vector<vertex> box{ vertex1,vertex2,vertex3,vertex4 };
	vector<GLuint> indices = { 0,1,2,
							   2,3,0 };
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	GLuint texture[2];
	glGenTextures(2, texture);
	for (GLuint i = 0; i < 2; i++) {
		
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture[i]);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB16F, 800, 600, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D_MULTISAMPLE, texture[i], 0);
	}
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	GLuint intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
	GLuint texture2[2];
	glGenTextures(2, texture2);
	for (GLuint i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, texture2[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, texture2[i], 0);
	}
	glDrawBuffers(2, attachments);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	vector<GLfloat>vertices = {
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, -1.0f, 0.0f, 0.0f
	};

	GLuint srVAO, srVBO, srEBO;
	glGenVertexArrays(1, &srVAO);
	glGenBuffers(1, &srVBO);
	glGenBuffers(1, &srEBO);

	glBindVertexArray(srVAO);
	glBindBuffer(GL_ARRAY_BUFFER, srVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, srEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	GLuint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, 128, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
	shared_ptr<shader> sShader = make_shared<shader>(shader(string(resource_path) + "/sr_v_shader.txt", string(resource_path) + "/sr_f_shader.txt"));
	shared_ptr<shader> qShader = make_shared<shader>(shader(string(resource_path) + "/q_v_shader.txt", string(resource_path) + "/q_f_shader.txt"));
	unique_ptr<shader> bShader(new shader(string(resource_path) + "/b_v_shader.txt", string(resource_path) + "/b_f_shader.txt"));

	vector<string> texturePathes = { string(resource_path) + "/floor.jpg" };
	shared_ptr<Quad> quad(new Quad(qShader, box, indices, texturePathes));
	shared_ptr<terrain> terrain(new terrain(qShader, string(resource_path) + "/little.jpg", string(resource_path) + "/toby.jpg"));
	shared_ptr<Ball> mball(new Ball(bShader));
	/*
	shared_ptr<shader> textShader = make_shared<shader>(shader("../src/t_v_shader.txt", "../src/t_f_shader.txt"));
	shared_ptr<TextRender>textRender = make_shared<TextRender>(TextRender(textShader));
	*/
	unique_ptr<shader> skyShader(new shader(string(resource_path) + "/sky_v_shader.txt", string(resource_path) + "/sky_f_shader.txt"));
	shared_ptr<SkyBox> skyBox(new SkyBox(skyShader));

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.8f, 1.8f, 1.8f));

	glm::mat4 model2;
	model2 = glm::translate(model2, glm::vec3(0.0f, 0.6f, -0.2f));
	model2 = glm::scale(model2, glm::vec3(0.1f, 0.1f, 0.1f));
	glm::mat4 model3;
	while (!glfwWindowShouldClose(window))
	{			
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glm::mat4 view = cam.getLookAt();
		glm::mat4 projection = glm::perspective(glm::radians(50.0f), 800.0f / 600.0f, 0.01f, 1000.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

	
		mball->draw(model2);
		terrain->draw(model3, cam.pos);
		quad->draw(model,cam.pos);
		skyBox->draw();
		/*	
		string x = to_string(cam.pos.x).substr(0, 4);
		string y = to_string(cam.pos.y).substr(0, 4);
		string z = to_string(cam.pos.z).substr(0, 4);
		string pos ="loc : "+ x + " " + y + " " + z;
		textRender->draw(pos, 620.0f, 550.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
		*/
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO); 
		glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); 
		sShader->use();
		sShader->setInt("srTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2[1]);
		glBindVertexArray(srVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		ProcessInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
} 



void ProcessInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		cam.updatePos(FRONT);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.updatePos(BACK);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.updatePos(LEFT);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.updatePos(RIGHT);
}


bool firstMouse = true;
float lastX,lastY;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}
	float xoffset = static_cast<float>(xpos) - lastX;
	float yoffset = lastY - static_cast<float>(ypos);
	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);
	cam.updateMouse(xoffset, yoffset);
}
