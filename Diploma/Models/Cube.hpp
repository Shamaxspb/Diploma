#ifndef CUBE_HPP
#define CUBE_HPP

#include "../Model_T.h"
#include "../Material_T.h"

class Cube : public Model
{
public:
	glm::vec3 pos;
	glm::vec3 size;

	Material material;

	Cube(Material material, glm::vec3 pos, glm::vec3 size)
		: material(material), pos(pos), size(size) {}

	/*Cube(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f))
		: Model(pos, size) {}*/

	void init()
	{
		int noVertices = 36;

		float vertices[] = {
			// positions			 normals				tex coords
			-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,    0.0f, 0.0f,	// A
			 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,    1.0f, 0.0f,	// B
			 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,    1.0f, 1.0f,	// C
			 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,    1.0f, 1.0f,	// C
			-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,    0.0f, 1.0f,	// D
			-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,    0.0f, 0.0f,	// A

			-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,    0.0f, 0.0f,	// E
			 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,    1.0f, 0.0f,	// F
			 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,    1.0f, 1.0f,	// G
			 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,    1.0f, 1.0f,	// G
			-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,    0.0f, 1.0f,	// H
			-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,    0.0f, 0.0f,	// E

			-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,    1.0f, 0.0f,	// H
			-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,    1.0f, 1.0f,	// D 
			-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,    0.0f, 1.0f,	// A
			-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,    0.0f, 1.0f,	// A
			-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,    0.0f, 0.0f,	// E
			-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,    1.0f, 0.0f,	// H

			 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,    1.0f, 0.0f,	// G
			 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,    1.0f, 1.0f,	// C
			 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,    0.0f, 1.0f,	// B
			 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,    0.0f, 1.0f,	// B
			 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,    0.0f, 0.0f,	// F
			 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,    1.0f, 0.0f,	// G

			-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,    0.0f, 1.0f,	// A
			 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,    1.0f, 1.0f,	// B
			 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,    1.0f, 0.0f,	// F
			 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,    1.0f, 0.0f,	// F
			-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,    0.0f, 0.0f,	// E
			-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,    0.0f, 1.0f,	// A

			-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,    0.0f, 1.0f,	// D
			 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,    1.0f, 1.0f,	// C
			 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,    1.0f, 0.0f,	// G
			 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,    1.0f, 0.0f,	// G
			-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,    0.0f, 0.0f,	// H
			-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,    0.0f, 1.0f	// D
		};

		//float vertices[] = {
		//	// positions		 // textures
		//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	// A
		//	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,	// B
		//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	// C
		//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	// C
		//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	// D
		//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	// A

		//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 	// E
		//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 	// F
		//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 	// G
		//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 	// G
		//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	// H
		//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	// E

		//	-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,	// H
		//	-0.5f,  0.5f, -0.5f,  1.0f,  1.0f, 	// D 
		//	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f, 	// A
		//	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,	// A
		//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,	// E
		//	-0.5f,  0.5f,  0.5f,  1.0f,  0.0f, 	// H

		//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	// G
		//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	// C
		//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	// B
		//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	// B
		//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	// F
		//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	// G

		//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	// A
		//	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 	// B
		//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 	// F
		//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	// F
		//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	// E
		//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 	// A

		//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,	// D
		//	 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,	// C
		//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,	// G
		//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,	// G
		//	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,	// H
		//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f	// D
		//};

		std::vector<unsigned int> indices(noVertices);
		for (unsigned int i = 0; i < noVertices; i++)
		{
			indices[i] = i;
		}

		/*Texture tex0("assets/image1.jpg", "texture0");
		tex0.load();
		Texture tex1("assets/image2.png", "texture1");
		tex1.load();*/

		meshes.push_back(Mesh(Vertex::genList(vertices, noVertices), indices));
	}

	void render(Shader shader)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pos);
		model = glm::scale(model, size);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f));
		shader.setMat4("model", model);

		shader.set3Float("material.ambient", material.ambient);
		shader.set3Float("material.diffuse", material.diffuse);
		shader.set3Float("material.specular", material.specular);
		shader.setFloat("material.shininess", material.shininess);

		Model::render(shader);
	}
};


#endif