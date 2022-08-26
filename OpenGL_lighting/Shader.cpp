#include "Shader.h"

Shader::Shader(const char* vertPath, const char* fragPath)
{
	std::string vertCode;
	std::string fragCode;
	std::ifstream vertShaderFile;
	std::ifstream fragShaderFile;
	vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertShaderFile.open(vertPath);
		fragShaderFile.open(fragPath);
		std::stringstream vertShaderStream, fragShaderStream;
		vertShaderStream << vertShaderFile.rdbuf();
		fragShaderStream << fragShaderFile.rdbuf();
		vertShaderFile.close();
		fragShaderFile.close();
		vertCode = vertShaderStream.str();
		fragCode = fragShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR: File not successfully read" << std::endl;
	}

	const char* vertShaderCode = vertCode.c_str();
	const char* fragShaderCode = fragCode.c_str();

	unsigned int vert, frag;
	int success;
	char infoLog[512];

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertShaderCode, NULL);
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vert, 512, NULL, infoLog);
		std::cout << "ERROR: Vertex shader compilation failed.\n" << infoLog << std::endl;
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragShaderCode, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		std::cout << "ERROR: Fragment shader compilation failed.\n" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vert);
	glAttachShader(ID, frag);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR: Shader program compilation failed.\n" << infoLog << std::endl;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec3(const std::string& name, glm::vec3& vec)
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


