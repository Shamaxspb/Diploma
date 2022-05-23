#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<ShaderProgram.h>
#include<Camera.h>

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iterator>


using namespace std;


// function for debug
template <typename T>
void PrintVector(const vector<T>& vec)
{
	for (const auto var : vec)
	{
		cout << var << endl;
	}
}


// function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);


// GLOBAL VARIABLES
// screen
const int START_WINDOW_WIDTH = 1440;	// created window width
const int START_WINDOW_HEIGHT = 900;	// created window height

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// initial mouse coordinates
float lastX = START_WINDOW_WIDTH / 2.0f;
float lastY = START_WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;	// if the first recieve of mouse input

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;


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
	// IN TOTAL:
	// 4728 values in waveVertices (theese are coordinates of vertices)
	// 1576 vertices
	// 1576 unique indicies
	// 787 triangles that form
	// 1574 polygons
	// thus
	// 4722 values in waveIndicies

	// CREATE VERTICES VECTOR ===============================================================================
	// variables to fill waveVertices vector:
	// - vector with all vertices
	vector<float> waveVertices;
	// - coordinates
	float x	= 0;
	float& x1 = x, & x2 = x;
	float y1 = 1000, y2 = -1000;	// width of the polygon
	float z = 0;					// initial z value that will be filled with data from text file
	float& z1 = z, & z2 = z;
	// - colors
	float r1 = 0.153f; float g1 = 0.718f; float b1 = 0.820f;	// color 1
	float r2 = 0.051f; float g2 = 0.388f; float b2 = 0.514f;	// color 2
	float r3 = 0.216f; float g3 = 0.424f; float b3 = 0.608f;	// color 3
	float r4 = 0.145f; float g4 = 0.259f; float b4 = 0.275f;	// color 4
	//float r4 = 0.659f; float g4 = 0.839f; float b4 = 0.937f;	// color 4
	
	fstream waveFile;

	// read file
	waveFile.open("WaveHeight.txt", fstream::in);
	if (!waveFile.is_open())
	{
		cout << "ERROR\tcouldn't open WaveHeight.txt" << endl;
		return -1;
	}

	// fill vector with vertices and colors
	unsigned int colorDecider = 2;
	while (!waveFile.eof())
	{
		waveFile >> z;

		if (colorDecider % 2 == 0)
		{
			waveVertices.push_back(x1);	// x1
			waveVertices.push_back(y1);	// y1
			waveVertices.push_back(z1);	// z1

			waveVertices.push_back(r1);	// r1
			waveVertices.push_back(g1);	// g1
			waveVertices.push_back(b1);	// b1

			waveVertices.push_back(x2);	// x2
			waveVertices.push_back(y2);	// y2
			waveVertices.push_back(z2);	// z2

			waveVertices.push_back(r2);	// r1
			waveVertices.push_back(g2);	// g1
			waveVertices.push_back(b2);	// b1
		}
		else
		{
			waveVertices.push_back(x1);	// x1
			waveVertices.push_back(y1);	// y1
			waveVertices.push_back(z1);	// z1

			waveVertices.push_back(r3);	// r1
			waveVertices.push_back(g3);	// g1
			waveVertices.push_back(b3);	// b1

			waveVertices.push_back(x2);	// x2
			waveVertices.push_back(y2);	// y2
			waveVertices.push_back(z2);	// z2

			waveVertices.push_back(r4);	// r1
			waveVertices.push_back(g4);	// g1
			waveVertices.push_back(b4);	// b1
		}

		x += 8;
		colorDecider++;
	}

	// fill vector with vertices only
	/*while (!waveFile.eof())
	{
		waveFile >> z;

		waveVertices.push_back(x1);
		waveVertices.push_back(y1);
		waveVertices.push_back(z1);

		waveVertices.push_back(x2);
		waveVertices.push_back(y2);
		waveVertices.push_back(z2);

		x += 8; // length of the polygon
	}*/
	//PrintVector(waveVertices);

	// CREATE INDICIES VECTOR ===============================================================================
	vector<int> waveIndicies;
	unsigned int index = 0;
	for (int i = 0; i < 787; i++)
	{
		waveIndicies.push_back(index);
		index += 1;
		waveIndicies.push_back(index);
		index += 2;
		waveIndicies.push_back(index);
		index -= 3;
		waveIndicies.push_back(index);
		index += 3;
		waveIndicies.push_back(index);
		index -= 1;
		waveIndicies.push_back(index);
		index += 0;
	}
	//PrintVector(waveIndicies);

	// create vector to blend color of water=================================================================
	vector<float> testVector = {
		//	 coordinates		color
		//	x	  y		z	  r		  g		  b	
			0.0f, 1.0f, 0.0f, 0.153f, 0.718f, 0.820f,	// 0 
			0.0f, 0.0f, 0.0f, 0.051f, 0.388f, 0.514f,	// 1 
			1.0f, 1.0f, 0.0f, 0.216f, 0.424f, 0.608f,	// 2 
			1.0f, 0.0f, 0.0f, 0.145f, 0.259f, 0.275f,	// 3 
			2.0f, 1.0f, 0.0f, 0.153f, 0.718f, 0.820f,	// 4 
			2.0f, 0.0f, 0.0f, 0.051f, 0.388f, 0.514f,	// 5 
			3.0f, 1.0f, 0.0f, 0.216f, 0.424f, 0.608f,	// 6 
			3.0f, 0.0f, 0.0f, 0.145f, 0.259f, 0.275f,	// 7 
			4.0f, 1.0f, 0.0f, 0.153f, 0.718f, 0.820f,	// 8 
			4.0f, 0.0f, 0.0f, 0.051f, 0.388f, 0.514f	// 9 
	};

	vector<int> testIndicies = {
		0, 1, 3,
		0, 3, 2,
		2, 3, 5,
		2, 5, 4,
		4, 5, 7,
		4, 7, 6,
		6, 7, 9,
		6, 9, 8
	};

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

	// uncomment theese to unlock look around with mouse and zoom in/out with mouseeheel
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// also uncomment WASD control in processInput() function to unlock movement with keyboard

	// tell GLFW to capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// check GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "ERROR:\tGLAD initialization failed" << endl;
		return -1;
	}
	
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// shader program compiling and linking
	Shader myShader("myVertexShader.vs", "myFragmentShader.fs");

	// creating mesh (20 triangles in NDC)
	float test2SquareVertices[] = {
		// position				// indicies of vertices
		-1.0f,  0.6f,  0.0f,	// A - 0 
		-1.0f, -0.6f,  0.0f,	// B - 1
		-0.8f,  0.6f, -0.2f,	// C - 2
		-0.8f, -0.6f, -0.2f,	// D - 3
		-0.6f,  0.6f,  0.2f,	// E - 4
		-0.6f, -0.6f,  0.2f,	// F - 5
		-0.4f,  0.6f, -0.2f,	// G - 6
		-0.4f, -0.6f, -0.2f,	// H - 7
		-0.2f,  0.6f,  0.2f,	// I - 8
		-0.2f, -0.6f,  0.2f,	// J - 9
		 0.0f,  0.6f, -0.2f,	// K - 10
		 0.0f, -0.6f, -0.2f,	// L - 11
		 0.2f,  0.6f,  0.2f,	// M - 12
		 0.2f, -0.6f,  0.2f,	// N - 13
		 0.4f,  0.6f, -0.2f,	// O - 14
		 0.4f, -0.6f, -0.2f,	// P - 15
		 0.6f,  0.6f,  0.2f,	// Q - 16
		 0.6f, -0.6f,  0.2f,	// R - 17
		 0.8f,  0.6f, -0.2f,	// S - 18
		 0.8f, -0.6f, -0.2f,	// T - 19
		 1.0f,  0.6f,  0.0f,	// U - 20
		 1.0f, -0.6f,  0.0f		// V - 21
	};

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

	// PASS DATA TO GPU
	unsigned int VAO, VBO, EBO;

	// create and bind VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//allocate memory in GPU and copy there data from testSquareVertices array
	//glBufferData(GL_ARRAY_BUFFER, sizeof(test2SquareVertices), test2SquareVertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, waveVertices.size() * sizeof(waveVertices), &waveVertices.front(), GL_STATIC_DRAW);	// actual
	//glBufferData(GL_ARRAY_BUFFER, testVector.size() * sizeof(testVector), &testVector.front(), GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// allocate memory in GPU and copy there data from testSquareIndicies array
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(test2SquareIndicies), test2SquareIndicies, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, waveIndicies.size() * sizeof(waveIndicies), &waveIndicies.front(), GL_STATIC_DRAW);	// actual
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, testIndicies.size() * sizeof(testIndicies), &testIndicies.front(), GL_STATIC_DRAW);

	// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);	// actual
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(testVector[0]), (void*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(waveVertices[0]), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	//	there is no ACTUAL since my wave has no color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(testVector[0]), (void*)(3 * sizeof(testVector[0])));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(waveVertices[0]), (void*)(3 * sizeof(waveVertices[0])));
	glEnableVertexAttribArray(1);

	myShader.Use();

	// TEST FIELD +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	// TEST FIELD ---------------------------------------------------------------------------------------------------------------------------------

	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop ==========================================================================================
	while (!glfwWindowShouldClose(window))
	{
		//per-frame logic
		float currentFrame	= static_cast<float>(glfwGetTime());	// getting current time of frame
		deltaTime			= currentFrame - lastFrame;
		lastFrame			= currentFrame;

		// input
		processInput(window);
		
		// RENDER
		// background color
		//glClearColor(0.63f, 0.46f, 0.84f, 1.0f);	// light purple
		//glClearColor(0.24f, 0.43f, 0.69f, 1.0f);	// sky above water
		glClearColor(0.72f, 0.79f, 0.96f, 1.0f);	// test (light grey)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		myShader.Use();

		// projection matrix 
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)START_WINDOW_WIDTH / (float)START_WINDOW_HEIGHT, 0.1f, 10000.0f);
		myShader.setMat4("projection", projection);
		
		// camera / view matrix
		glm::mat4 view = camera.GetViewMatrix();
		myShader.setMat4("view", view);

		// model matrix
		glm::mat4 model = glm::mat4(1.0f); // set identity matrix is neñessarily
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		myShader.setMat4("model", model);
				
		// WATER OBJECT TRANSFORMATION MATRICES
		glm::mat4 transform = glm::mat4(1.0f);	// identity matrix
		//transform = glm::rotate(transform, glm::radians(-30.0f), glm::vec3(1.0, 0.0, 0.0));					// rotate to a certain angle
		//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0, 0.0, 0.0));					// constant rotation
		//transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));											// scale
		//transform = glm::translate(transform, glm::vec3(0.1f, 0.0f, 0.0f) * ((float)glfwGetTime() * 0.5f));	// TRANSLATES RIGHT BY THIS
		//transform = glm::translate(transform, glm::vec3(1.0f, 0.0f, 0.0f) * (float)glfwGetTime() * 0.1f);
		myShader.setMat4("transform", transform);

		// render
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);	// for 20 triangles (indexed)
		glDrawElements(GL_TRIANGLES, 4722, GL_UNSIGNED_INT, 0);	// for water mesh (actual)
		//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);	// for water mesh 
		glBindVertexArray(0);

		// swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

// function block
void processInput(GLFWwindow* window)
{
	// ESC to close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// uncomment theese to move with WASD
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	// preventing cursor jumps
	if (firstMouse)
	{
		lastX		= xpos;
		lastY		= ypos;
		firstMouse	= false;
	}

	// offset from the last frame
	float xOffset	= xpos - lastX;
	float yOffset	= lastY - ypos; // reversed since y ranges bottom to top
	lastX			= xpos;
	lastY			= ypos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yOffset));
}