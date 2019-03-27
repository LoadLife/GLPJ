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
#include "Quad.h"
#include "element.h"
#include "shader.h"
#include "Camera.h"
#include "Ball.h"
#include "terrain.h"
#include "TextRender.h"
void ProcessInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
using namespace std;
//摄像机
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
	//初始化glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "Failed to initialize GLEW" << endl;
		return -1;

	}
	//开启混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//打印设备相关
	const GLubyte* firm = glGetString(GL_VENDOR);
	const GLubyte* identifier = glGetString(GL_RENDERER);
	printf("Realize By %s\n", firm);
	printf("Run On %s\n", identifier);
	glfwSetCursorPosCallback(window, mouse_callback);//鼠标回调函数
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//设置光标不可见

	vertex vertex1 = { glm::vec3(-0.2f, 0.2f, 0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec2(0.0f,1.0f) };
	vertex vertex2 = { glm::vec3( 0.2f, 0.2f, 0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec2(1.0f,1.0f) };
	vertex vertex3 = { glm::vec3( 0.2f,-0.2f, 0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec2(1.0f,0.0f) };
	vertex vertex4 = { glm::vec3(-0.2f,-0.2f, 0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec2(0.0f,0.0f) };
	vector<vertex> box{ vertex1,vertex2,vertex3,vertex4 };
	vector<GLuint> indices = { 0,1,2,
							   2,3,0 };
	//帧缓冲相关
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	    //纹理附件
	unsigned int texture[2];
	glGenTextures(2, texture);
	for (GLuint i = 0; i < 2; i++) {
		
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture[i]);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB16F, 800, 600, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D_MULTISAMPLE, texture[i], 0);
	}
	    //深度缓冲附件
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		//渲染到多个颜色缓冲
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	    //检测帧缓冲是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//中间帧缓冲
	GLuint intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
	    //纹理附件
	GLuint texture2[2];
	glGenTextures(2, texture2);
	for (GLuint i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, texture2[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, texture2[i], 0);
	}
		//渲染到多个颜色缓冲中并检测缓冲是否完整
	glDrawBuffers(2, attachments);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	//帧缓冲纹理渲染到屏幕四边形
	vector<GLfloat>vertices = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, 0.0f, 0.0f, 0.0f
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


	//屏幕
	shared_ptr<shader> sShader = make_shared<shader>(shader("../src/sr_v_shader.txt", "../src/sr_f_shader.txt"));
	//四边形
	shared_ptr<shader> qShader = make_shared<shader>(shader("../src/q_v_shader.txt", "../src/q_f_shader.txt"));
	vector<string> texturePathes = { "../src/floor.jpg" };
	shared_ptr<Quad> quad(new Quad(qShader, box, indices, texturePathes));
	//球
	shared_ptr<shader> bShader = make_shared<shader>(shader("../src/b_v_shader.txt", "../src/b_f_shader.txt"));
	shared_ptr<Ball> mball(new Ball(bShader));
	//地形
	shared_ptr<terrain> terrain(new terrain("../src/little.jpg", "../src/toby.jpg", qShader));
	//文本
	shared_ptr<shader> textShader = make_shared<shader>(shader("../src/t_v_shader.txt", "../src/t_f_shader.txt"));
	shared_ptr<TextRender>textRender = make_shared<TextRender>(TextRender(textShader));

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
		mball->draw(model2,view,projection);
		quad->draw(model,view,projection,cam.pos);
		terrain->draw(model3, view, projection, cam.pos);

		string x = to_string(cam.pos.x).substr(0, 4);
		string y = to_string(cam.pos.y).substr(0, 4);
		string z = to_string(cam.pos.z).substr(0, 4);
		string pos = x + " " + y + " " + z;
		textRender->draw(pos, 640.0f, 550.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
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
