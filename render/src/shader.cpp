#include "shader.h"
using namespace std;
shader::shader(const string vertexPath, string fragmentPath) {

	string vertexCode, fragmentCode;
	ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(ifstream::badbit);
	fShaderFile.exceptions(ifstream::badbit);
	try {

		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e) {

		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {

		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED \n" << infoLog << endl;

	}
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED \n" << infoLog << endl;

	}
	this->program_ = glCreateProgram();
	glAttachShader(this->program_, vertex);
	glAttachShader(this->program_, fragment);
	glLinkProgram(this->program_);
	glGetProgramiv(this->program_, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->program_, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINK_FAILED \n" << infoLog << endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);


}
shader::~shader() {

}
void shader::use() {
	glUseProgram(this->program_);
}

void shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(program_, name.c_str()), (int)value);
}

void shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}

void shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(program_, name.c_str()), value);
}

void shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(program_, name.c_str()), x, y, z);
}