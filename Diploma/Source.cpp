#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<ShaderProgram.h>

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iterator>

using namespace std;



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

template <typename T>
void PrintVector(const vector<T>& vec)
{
	for (const auto var : vec)
	{
		cout << var << endl;
	}
}

const int START_WINDOW_WIDTH = 1366;
const int START_WINDOW_HEIGHT = 768;

class Vector3d
{
private:
	float x;
	float y;
	float z;

public:
	Vector3d(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
	float GetX() { return x; }
	float GetY() { return y; }
	float GetZ() { return z; }
};


int main()
{
	// CREATE VERTICES VECTOR ===============================================================================
	// variables to fill verticesContainer
	vector<float> verticesContainer;	// vector with all vertices
	float x = 0;
	float& x1 = x, & x2 = x;
	float y1 = 16, y2 = y1 - 12;
	float z = 0;
	float& z1 = z, & z2 = z;
	fstream waveFile;

	waveFile.open("WaveHeight.txt", fstream::in);
	if (!waveFile.is_open())
	{
		cout << "ERROR\tcouldn't open WaveHeight.txt" << endl;
		return -1;
	}

	while (!waveFile.eof())
	{
		waveFile >> z;

		verticesContainer.push_back(x1);
		verticesContainer.push_back(y1);
		verticesContainer.push_back(z1);

		verticesContainer.push_back(x2);
		verticesContainer.push_back(y2);
		verticesContainer.push_back(z2);

		x += 6;
	}

	PrintVector(verticesContainer);

	// ==============================================================================

	// initializing GLFW ====================================================================================
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creating window
	GLFWwindow* window = glfwCreateWindow(START_WINDOW_WIDTH, START_WINDOW_HEIGHT, "Object Visualisation", NULL, NULL);
	// check window
	if (window == NULL)
	{
		cout << "ERROR\tWindow creation failed" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// check GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "ERROR:\tGLAD initialization failed" << endl;
		return -1;
	}

	// TEST FIELD +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++





	// TEST FIELD -----------------------------------------------------------------------------------------------------

	// shader program compiling and linking
	Shader myShader("myVertexShader.vs", "myFragmentShader.fs");

	// creating mesh (20 triangles in NDC)
	float test2SquareVertices[] = {
		// position			// indicies of vertices
		-1.0f,  0.6f, 0.0f,	// A - 0 
		-1.0f, -0.6f, 0.0f,	// B - 1
		-0.8f,  0.6f, 0.0f,	// C - 2
		-0.8f, -0.6f, 0.0f,	// D - 3
		-0.6f,  0.6f, 0.0f,	// E - 4
		-0.6f, -0.6f, 0.0f,	// F - 5
		-0.4f,  0.6f, 0.0f,	// G - 6
		-0.4f, -0.6f, 0.0f,	// H - 7
		-0.2f,  0.6f, 0.0f,	// I - 8
		-0.2f, -0.6f, 0.0f,	// J - 9
		 0.0f,  0.6f, 0.0f,	// K - 10
		 0.0f, -0.6f, 0.0f,	// L - 11
		 0.2f,  0.6f, 0.0f,	// M - 12
		 0.2f, -0.6f, 0.0f,	// N - 13
		 0.4f,  0.6f, 0.0f,	// O - 14
		 0.4f, -0.6f, 0.0f,	// P - 15
		 0.6f,  0.6f, 0.0f,	// Q - 16
		 0.6f, -0.6f, 0.0f,	// R - 17
		 0.8f,  0.6f, 0.0f,	// S - 18
		 0.8f, -0.6f, 0.0f,	// T - 19
		 1.0f,  0.6f, 0.0f,	// U - 20
		 1.0f, -0.6f, 0.0f,	// V - 21
	};

	// test mesh (20 triangles in world coordinates)
	//float test2SquareVertices[] = {
	//	// position				// indicies of vertices
	//	0.0f,  16.0f, -40.911f,	// A - 0 
	//	0.0f,  4.0f,  -40.911f,	// B - 1
	//	6.0f,  16.0f, -30.098f,	// C - 2
	//	6.0f,  4.0f,  -30.098f,	// D - 3
	//	12.0f, 16.0f, -18.839f,	// E - 4
	//	12.0f, 4.0f,  -18.839f,	// F - 5
	//	18.0f, 16.0f, -8.2492f,	// G - 6
	//	18.0f, 4.0f,  -8.2492f,	// H - 7
	//	24.0f, 16.0f,  1.6721f,	// I - 8
	//	24.0f, 4.0f,   1.6721f,	// J - 9
	//	30.0f, 16.0f,  10.479f,	// K - 10
	//	30.0f, 4.0f,   10.479f,	// L - 11
	//	36.0f, 16.0f,  17.725f,	// M - 12
	//	36.0f, 4.0f,   17.725f,	// N - 13
	//	42.0f, 16.0f,  23.744f,	// O - 14
	//	42.0f, 4.0f,   23.744f,	// P - 15
	//	48.0f, 16.0f,  29.206f,	// Q - 16
	//	48.0f, 4.0f,   29.206f,	// R - 17
	//	54.0f, 16.0f,  32.662f,	// S - 18
	//	54.0f, 4.0f,   32.662f,	// T - 19
	//	60.0f, 16.0f,  34.669f,	// U - 20
	//	60.0f, 4.0f,   34.669f,	// V - 21
	//};

	unsigned int test2SquareIndicies[] = {
		0, 1, 3,	// triangle ABD
		0, 3, 2,	// triangle ADC
		2, 3, 5,	// triangle CDF
		2, 5, 4,	// triangle CFE
		4, 5, 7,	// triangle EFH
		4, 7, 6,	// triangle EHG
		6, 7, 9,	// triangle GHJ
		6, 9, 8,	// triangle GJI
		8, 9, 11,	// triangle IJL
		8, 11, 10,	// triangle ILK
		10, 11, 13,	// triangle KLN
		10, 13, 12,	// triangle KNM
		12, 13, 15,	// triangle MNP
		12, 15, 14,	// triangle MPO
		14, 15, 17,	// triangle OPR
		14, 17, 16,	// triangle ORQ
		16, 17, 19,	// triangle QRT
		16, 19, 18,	// triangle QTS
		18, 19, 21,	// triangle STV
		18, 21, 20	// triangle SVU
	};

	// basic triangle
	float goodOldTriangle[] = {
		-0.5f, -0.5f, 0.0f,	// A
		 0.0f,  0.5f, 0.0f,	// B
		 0.5f, -0.5f, 0.0f	// C
	};

	unsigned int VAO, VBO, EBO;

	// create and bind VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//allocate memory in GPU and copy there data from testSquareVertices array
	glBufferData(GL_ARRAY_BUFFER, sizeof(test2SquareVertices), test2SquareVertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, verticesContainer.size(), &verticesContainer[0], GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// allocate memory in GPU and copy there data from testSquareIndicies array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(test2SquareIndicies), test2SquareIndicies, GL_STATIC_DRAW);

	// finding attribs
	/*unsigned int posAttrib = glGetAttribLocation(static Shader::ID, "vertexPosition");
	unsigned int colAttrib = glGetAttribLocation(Shader::ID, "vertexColor");*/


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/


	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	myShader.Use();

	// render loop ==========================================================================================
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		glClearColor(0.63f, 0.46f, 0.84f, 1.0f);
		//glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate shader
		myShader.Use();

		// PERSPECTIVE PROJECTIION
		// model matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		// view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection = glm::mat4(1.0f);
		// projection matrix				fov				  (aspect ratio	1367		/				768)	   near  far
		projection = glm::perspective(glm::radians(45.0f), (float)START_WINDOW_WIDTH / (float)START_WINDOW_HEIGHT, 0.1f, 100.0f);

		// retrieve matrix uniform locations
		unsigned int modelLoc = glGetUniformLocation(myShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");

		// pass matrices to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

		myShader.setMat4("projection", projection);



		// CAMERA
		// position
		glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
		// direction	(positive z-axis)
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);	// scene origin
		glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
		// right		(positive x-axis)
		glm::vec3 up = glm::vec3(0.0f, 0.1f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		// up			(positive y-axis)
		glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));

		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;

		//glm::mat4 view = glm::lookAt(
		//	glm::vec3(camX, 0.0f, camZ),	// position
		//	glm::vec3(0.0f, 0.0f, 3.0f),	// target
		//	glm::vec3(0.0f, 0.0f, 3.0f));	// up vector

		// TRANSFORMATION MATRIX
		glm::mat4 transform = glm::mat4(1.0f);
		//glm::vec4 translation = glm::vec4() + 0.1f * (float)glfwGetTime();

		//transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0, 1.0, 1.0));
		//transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));
		transform = glm::translate(transform, glm::vec3(0.1f, 0.0f, 0.0f) * ((float)glfwGetTime() * 1.0f));  //TRANSLATES RIGHT BY THIS

		// get matrix's uniform location and set matrix		
		unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0); // for 4 triangles
		glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0); // for 4 triangles
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

		// swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

// function block
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void drawPolgyon()
{

}
