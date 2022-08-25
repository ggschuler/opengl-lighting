#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/ext/matrix_float4x4.hpp>

class Shader
{
public:
	unsigned int ID;
	Shader(const char* vertPath, const char* fragPath);
	void use();
	void setInt(std::string& name, int value);
	void setBool(std::string& name, bool value);
	void setFloat(std::string& name, float value);
	void setMat4(std::string& name, const glm::mat4& mat) const;
};

#endif 
