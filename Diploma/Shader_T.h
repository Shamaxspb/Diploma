#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int id;

	Shader();
	Shader(const char* vertexShaderPath, const char* fragShaderPath);

	void generate(const char* vertexShaderPath, const char* fragShaderPath);

	void activate();

	// utility functions
	std::string loadShaderSrc(const char* filepath);
	GLuint compileShader(const char* filepath, GLuint type);

	// uniform functions
	void setBool(const std::string& name, bool val);
	void setInt(const std::string& name, int val);
	void setFloat(const std::string& name, float val);
	void set3Float(const std::string& name, glm::vec3 v);
	void set3Float(const std::string& name, float v1, float v2, float v3);
	void set4Float(const std::string& name, float v1, float v2, float v3, float v4);

	void setVec3(const std::string& name, glm::vec3 val);
	void setVec3(const std::string& name, float x, float y, float z);
	void setMat4(const std::string& name, glm::mat4 val);

};
#endif