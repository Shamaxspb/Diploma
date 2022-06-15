#ifndef LIGHT_H
#define LIGHT H

#include<glm/glm.hpp>

#include<Shader_T.h>

struct DirLight
{
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void render(Shader shader);
};

struct PointLight
{
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void render(Shader shader);
};

struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;

	float cutOff;		// angle between direction and inner border
	float outerCutOff;	// angle between direction and outer border

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void render(Shader shader);
};

#endif