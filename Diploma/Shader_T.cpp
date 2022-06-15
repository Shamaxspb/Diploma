#include "Shader_T.h"

Shader::Shader() {}
Shader::Shader(const char* vertexShaderPath, const char* fragShaderPath)
{
	generate(vertexShaderPath, fragShaderPath);
}

void Shader::generate(const char* vertexShaderPath, const char* fragShaderPath)
{
	int success;
	char infoLog[512];

	GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragShader = compileShader(fragShaderPath, GL_FRAGMENT_SHADER);

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR: " << std::endl << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

void Shader::activate()
{
	glUseProgram(id);
}

std::string Shader::loadShaderSrc(const char* filename)
{
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	file.open(filename);

	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
	}
	else
	{
		std::cout << "Couldn't open " << filename << std::endl;
	}

	file.close();

	return ret;
}

GLuint Shader::compileShader(const char* filepath, GLuint type)
{
	int success;
	char infoLog[512];

	GLuint ret = glCreateShader(type);
	std::string shaderSrc = loadShaderSrc(filepath);
	const GLchar* shader = shaderSrc.c_str();
	glShaderSource(ret, 1, &shader, NULL);
	glCompileShader(ret);

	glGetShaderiv(ret, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ret, 512, NULL, infoLog);
		std::cout << "ERROR::VERTEX_SHADER_COMPILING_ERROR: " << std::endl << infoLog << std::endl;
	}

	return ret;
}

void Shader::setBool(const std::string& name, bool val)
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)val);
}

void Shader::setInt(const std::string& name, int val)
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), val);
}

void Shader::set3Float(const std::string& name, glm::vec3 v)
{
	set3Float(name, v.x, v.y, v.z);
}

void Shader::set3Float(const std::string& name, float v1, float v2, float v3)
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), v1, v2, v3);
}

void Shader::setVec3(const std::string& name, glm::vec3 val)
{
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &val[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::setMat4(const std::string& name, glm::mat4 val)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}