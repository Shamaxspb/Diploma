#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<ShaderProgram.h>
#include<Camera.h>

//#include<assimp/config.h>

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iterator>

class Vertex
{
public:
	Vertex(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
private:
	float x;
	float y;
	float z;
};


 //function for debug
template <typename T>
void PrintVector(const std::vector<T>& vec)
{
	for (const auto var : vec)
	{
		std::cout << var << std::endl;
	}
}

// function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void setupMesh(unsigned int& VAO, unsigned int& VBO, std::vector<glm::vec3> object, unsigned int containerElements);
void setupMesh(unsigned int& VAO, unsigned int& VBO, std::vector<float> object, unsigned int containerElements);
void setupMesh(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, std::vector<float> object, std::vector<unsigned int> objectIndicies, unsigned int containerElements);


// GLOBAL VARIABLES
// screen
const int START_WINDOW_WIDTH = 1440;	// created window width
const int START_WINDOW_HEIGHT = 900;	// created window height

// camera
Camera camera(glm::vec3(-250.0f, 750.0f, -500.0f));
// initial mouse coordinates
float lastX = START_WINDOW_WIDTH / 2.0f;
float lastY = START_WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;	// if the first recieve of mouse input

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// lightning
//					x	  z			y
glm::vec3 lightPos(0.0f, 570.0f, -3250.0f);			// light source position
glm::vec3 cubePosition(420.0f, 250.0f, 225.0f);		// test cube position


int main()
{
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
		std::cout << "ERROR\tWindow creation failed" << std::endl;
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
		std::cout << "ERROR:\tGLAD initialization failed" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// READ DATA FROM FILE
	std::vector<float> inputData;	// vector with all the data
	std::fstream testData;			// object to read that data
	testData.open("TestData.txt", std::fstream::in);
	if (!testData.is_open())
	{
		std::cout << "ERROR\tcouldn't open TestData.txt" << std::endl;
		return -1;
	}
	else
		std::cout << "Successfuly read TestData.txt\n" << std::endl;

	// write data to common vector
	while (!testData.eof())
	{
		float buffer;
		testData >> buffer;
		inputData.push_back(buffer);
	}

	// vectors to fill
	std::vector<float> waveHeight;		// wave height   data
	std::vector<float> shipRotation;	// ship rotation data
	std::vector<float> shipDrifting;	// ship drifting data
	std::vector<float>::iterator input_it = inputData.begin();
	for (auto input_it = inputData.begin(); input_it != inputData.end(); ++input_it)
	{
		++input_it; // skip first column (âðåìÿ) since it isn't used (yet?)
		shipRotation.push_back(*input_it);	// Pitch
		++input_it;
		shipRotation.push_back(*input_it);	// Roll
		++input_it;
		shipDrifting.push_back(*input_it);	// X
		++input_it;
		shipDrifting.push_back(*input_it);	// Y
		++input_it;
		shipRotation.push_back(*input_it);	// Yaw
		++input_it;
		shipDrifting.push_back(*input_it);	// Z
		++input_it;
		waveHeight.push_back(*input_it);	// wave height
	}
	//PrintVector(shipDrifting);
	//PrintVector(shipRotation);
	//PrintVector(waveHeight);


	// WAVE FORM CONFIGURATION ============================================================================== Wave config

	float x = 0;					// polygon length
	float y = 10000;				// polygon width


	// TEMPORARY DATA CONTAINERS ============================================================================ Temporary Containers

	// vector with UNIQUE vertex coordintes
	std::vector<glm::vec3> waveCoordsUnique;
	auto wave_it = waveHeight.begin();
	for (; wave_it != waveHeight.end(); ++wave_it)
	{
		waveCoordsUnique.emplace_back(x,  y, *wave_it);
		waveCoordsUnique.emplace_back(x, -y, *wave_it);
		x += 25;
	}

	// vector with coordinates in the right order to form wave mesh
	std::vector<glm::vec3> waveCoords; 
	auto wave_current = waveHeight.begin();
	auto wave_next = waveHeight.begin();
	advance(wave_next, 1);
	x = 0;	// reset x to default value
	for (int i = 0; i < waveHeight.size() - 1; i++)
	{
		float xn = x + 25.0f;
		// triangle 1
		waveCoords.emplace_back(x,   y, *wave_current);		// v0 | x0, y0, z0
		waveCoords.emplace_back(x,  -y, *wave_current);		// v1 | x1, y1, z1
		waveCoords.emplace_back(xn, -y, *wave_next);		// v2 | x2, y2, z2

		// triangle 2
		waveCoords.emplace_back(x,   y, *wave_current);		// v3 | x3, y3, z3
		waveCoords.emplace_back(xn,  y, *wave_next);		// v4 | x4, y4, z4
		waveCoords.emplace_back(xn, -y, *wave_next);		// v5 | x5, y5, z5

		// change values for next polygon
		x += 25;
		wave_current++;
		wave_next++;
	}

	// vector with wave normals
	std::vector<glm::vec3> waveNormals;
	auto ci = waveCoordsUnique.begin();	// current  vertex iterator
	advance(ci, 1);
	auto pi = waveCoordsUnique.begin();	// parallel vertex iterator
	auto fi = waveCoordsUnique.begin();	// further  vertex iterator
	advance(fi, 3);

	int size = 1574 / 2;
	for (int i = 0; i < size; i++)
	{
		glm::vec3 c = *ci;			// current  vertex
		glm::vec3 p = *pi;			// parallel vertex
		glm::vec3 f = *fi;			// further  vertex
		glm::vec3 ySide = p - c;	// parallel - current
		glm::vec3 xSide = f - c;	// further  - current
		waveNormals.emplace_back(glm::normalize(glm::cross(ySide, xSide)));
		
		ci++;						// next vertice
		c = *ci;
		xSide = p - c;
		ySide = f - c;
		waveNormals.emplace_back(glm::normalize(glm::cross(ySide, xSide)));
		if (i != size - 1)
		{
			// next polygon
			ci++;
			pi += 2;
			fi += 2;
		}
		else break;
	}


	// CREATE WAVE VECTOR =================================================================================== Wave
	// final vector with coords and normals in the right order
	std::vector<glm::vec3> wave;
	auto w_coord = waveCoords.begin();
	auto w_normal = waveNormals.begin();
	for (int i = 0; i < waveCoords.size(); i++)
	{
		if (i != waveNormals.size() - 1)
		{
			for (int j = 0; j < 3; j++)
			{
				wave.emplace_back(*w_coord);
				wave.emplace_back(*w_normal);
				w_coord++;
			}
			w_normal++;
		}
		else
		{
			for (int j = 0; j < 3; j++)
			{
				wave.emplace_back(*w_coord);
				wave.emplace_back(*w_normal);
				if (j != 2)
				{
					w_coord++;
				}
			}
			break;
		}
	}

	std::vector<glm::vec3> wave2;
	/*
	// polygon 1
	wave2.emplace_back(0.0f, 100.0f, -40.911f);
	wave2.emplace_back(0.585f, 0.0f, -0.811f);
	wave2.emplace_back(0.0f, -100.0f, -40.911f);
	wave2.emplace_back(0.585f, 0.0f, -0.811f);
	wave2.emplace_back(15.0f, -100.0f, -30.097f);
	wave2.emplace_back(0.585f, 0.0f, -0.811f);
	wave2.emplace_back(0.0f, 100.0f, -40.911f);
	wave2.emplace_back(0.585f, 0.0f, -0.811f);
	wave2.emplace_back(15.0f, 100.0f, -30.097f);
	wave2.emplace_back(0.585f, 0.0f, -0.811f);
	wave2.emplace_back(15.0f, -100.0f, -30.097f);
	wave2.emplace_back(0.585f, 0.0f, -0.811f);
	// polygon 2
	wave2.emplace_back(15.0f, 100.0f, -30.097f);
	wave2.emplace_back(0.600f, 0.0f, -0.799f);
	wave2.emplace_back(15.0f, -100.0f, -30.097f);
	wave2.emplace_back(0.600f, 0.0f, -0.799f);
	wave2.emplace_back(30.0f, -100.0f, -18.839f);
	wave2.emplace_back(0.600f, 0.0f, -0.799f);
	wave2.emplace_back(15.0f, 100.0f, -30.097f);
	wave2.emplace_back(0.600f, 0.0f, -0.799f);
	wave2.emplace_back(30.0f, 100.0f, -18.839f);
	wave2.emplace_back(0.600f, 0.0f, -0.799f);
	wave2.emplace_back(30.0f, -100.0f, -18.839f);
	wave2.emplace_back(0.600f, 0.0f, -0.799f);
	// polygon 3
	wave2.emplace_back(30.0f, 100.0f, -18.839f);
	wave2.emplace_back(0.577f, 0.0f, -0.817f);
	wave2.emplace_back(30.0f, -100.0f, -18.839f);
	wave2.emplace_back(0.577f, 0.0f, -0.817f);
	wave2.emplace_back(45.0f, -100.0f, -8.249f);
	wave2.emplace_back(0.577f, 0.0f, -0.817f);
	wave2.emplace_back(30.0f, 100.0f, -18.839f);
	wave2.emplace_back(0.577f, 0.0f, -0.817f);
	wave2.emplace_back(45.0f, 100.0f, -8.249f);
	wave2.emplace_back(0.577f, 0.0f, -0.817f);
	wave2.emplace_back(45.0f, -100.0f, -8.249f);
	wave2.emplace_back(0.577f, 0.0f, -0.817f);*/

	// create wave vertices vector
	// variables to fill waveVertices vector:
	std::vector<Vertex> waveVertices;	// - vertices with colors vector
	// - TEST colors (converting to 0:1 scope)
	float r1 = (float) 55 / 255, g1 = (float)219 / 255, b1 = (float)230 / 255;	// color 1 (Turquoise)
	float r2 = (float)100 / 255, g2 = (float)250 / 255, b2 = (float)255 / 255;	// color 2 (Electric)
	float r3 = (float) 82 / 255, g3 = (float)230 / 255, b3 = (float)243 / 255;	// color 3 (Spray)
	float r4 = (float) 21 / 255, g4 = (float)226 / 255, b4 = (float)233 / 255;	// color 4 (Bright turquoise)
			
	// wave indicies
	std::vector<unsigned int> waveIndicies;	// indicies vector
	unsigned int index = 0;
	// fill vector with indicies ()
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

	// CREATE LIGHT SOURCE ================================================================================== Light Source
	// light source data
	std::vector<float> lightSource = {
		// position				// Index of vertex
		-1.0f,  1.0f, -1.0f,	// A 0
		 1.0f,  1.0f, -1.0f,	// B 1
		 1.0f, -1.0f, -1.0f,	// C 2
		-1.0f, -1.0f, -1.0f,	// D 3
		-1.0f,  1.0f,  1.0f,	// E 4
		 1.0f,  1.0f,  1.0f,	// F 5
		 1.0f, -1.0f,  1.0f,	// G 6
		-1.0f, -1.0f,  1.0f,	// H 7
	};
	std::vector<unsigned int> lightSourceIndicies = {
		//indicies	// triangles
		0, 3, 2,	// ADC
		0, 2, 1,	// ACB
		1, 2, 6,	// BCG
		1, 6, 5,	// BGF
		4, 0, 1,	// EAB
		4, 1, 5,	// EBF
		4, 0, 3,	// EAD
		4, 3, 7,	// EDH
		7, 3, 2,	// HDC
		7, 2, 6,	// HCG
		4, 7, 6,	// EHG
		4, 6, 5		// EGF
	};

	// CREATE TEST CUBE ===================================================================================== Test Cube
	// test cube data (unindexed)
	std::vector<float> testCube = {
		// positions		 // normals 
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	// A
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	// B
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	// C
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	// C
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	// D
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	// A

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	// E
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	// F
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	// G
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	// G
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	// H
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	// E

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	// H
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	// D 
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	// A
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	// A
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	// E
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	// H

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	// G
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	// C
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	// B
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	// B
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	// F
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	// G

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,	// A
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,	// B
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,	// F
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,	// F
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,	// E
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,	// A

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	// D
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	// C
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	// G
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	// G
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	// H
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f	// D
	};


	// SHADERS ============================================================================================== Shaders
	// shader program compiling and linking
	Shader lightingShader("Shaders/lightingShader.vs", "Shaders/lightingShader.fs");
	Shader lightSourceShader("Shaders/lightSourceShader.vs", "Shaders/lightSourceShader.fs");


	// CREATE OBJECTS ======================================================================================= Objects
	// create wave 
	unsigned int waveVAO, waveVBO;
	setupMesh(waveVAO, waveVBO, wave, 2);

	// TEST WAVE
	/*
	unsigned int wave2VAO, wave2VBO;
	glGenVertexArrays(1, &wave2VAO);
	glGenBuffers(1, &wave2VBO);

	glBindVertexArray(wave2VAO);
	glBindBuffer(GL_ARRAY_BUFFER, wave2VBO);
	glBufferData(GL_ARRAY_BUFFER, wave2.size() * sizeof(glm::vec3), &wave2.front(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(wave2[0]), (void*)0);
	glEnableVertexAttribArray(0);

	// normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(wave2[0]), (void*)(2 * sizeof(wave2[0])));
	glEnableVertexAttribArray(1);*/

	// create light source
	unsigned int lightVAO, lightVBO, lightEBO;
	setupMesh(lightVAO, lightVBO, lightEBO, lightSource, lightSourceIndicies, 3);

	// create cube 
	unsigned int cubeVAO, cubeVBO; 
	setupMesh(cubeVAO, cubeVBO, testCube, 6);

	// TEST FIELD +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	// TEST FIELD ---------------------------------------------------------------------------------------------------------------------------------

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// render loop ==========================================================================================
	while (!glfwWindowShouldClose(window))
	{
		//per-frame logic
		float currentFrame	= static_cast<float>(glfwGetTime());	// getting current time of frame
		deltaTime			= currentFrame - lastFrame;
		lastFrame			= currentFrame;

		// input
		processInput(window);
		

		// RENDER INITIATION
		// background color
		glClearColor(0.72f, 0.79f, 0.96f, 1.0f);	// light grey
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// WAVE CONFIGURATION
		// colors
		glm::vec3 lightColor((float)243/255, (float)218 / 255, (float)170 / 255);
		glm::vec3 objectColor((float)82 / 255, (float)230 / 255, (float)243 / 255);	// wave color
		// activate shader
		lightingShader.Use();
		lightingShader.setVec3("objectColor", objectColor);	// test cube config
		lightingShader.setVec3("lightColor", lightColor);
		lightingShader.setVec3("lightPosition", lightPos);
		lightingShader.setVec3("viewPos", camera.Position);		
		// projection matrix and camera / view matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)START_WINDOW_WIDTH / (float)START_WINDOW_HEIGHT, 0.1f, 20000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		// model matrix
		glm::mat4 model = glm::mat4(1.0f); // set identity matrix is neñessarily
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.0, 0.0, 0.0) * (currentFrame * 175.0f));	// WAVE TRANSLATES BY THIS MATRIX (multiplier is the speed of wave)
		lightingShader.setMat4("model", model);
		// draw wave
		glBindVertexArray(waveVAO);
		//glDrawElements(GL_TRIANGLES, waveIndicies.size(), GL_UNSIGNED_INT, 0);	// for water mesh (actual)
		glDrawArrays(GL_TRIANGLES, 0, wave.size()); // FRAME NOT IN MODULE ERROR
		glBindVertexArray(0);


		// TEST CUBE CONFIGURATION
		lightingShader.Use();
		lightingShader.setVec3("objectColor", 1.0, 0.4f, 0.0f);
		lightingShader.setVec3("lightColor", lightColor);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePosition);
		model = glm::scale(model, glm::vec3(100.0f));
		lightingShader.setMat4("model", model);
		// draw test cube
		glBindVertexArray(cubeVAO);
		//glDrawElements(GL_TRIANGLES, testCubeIndicies.size(), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, testCube.size() / 6);	// amount of vertices divided by number of components
		glBindVertexArray(0);


		// LIGHT SOURCE CONFIGURATION
		// matrices can be configured in any order, order in multiplication in shaders - thats what really matters
		lightSourceShader.Use();
		lightSourceShader.setMat4("projection", projection);
		lightSourceShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(15.0f));
		lightSourceShader.setMat4("model", model);
		// draw light source
		glBindVertexArray(lightVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		// swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &waveVAO);
	glDeleteBuffers(1, &waveVBO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);
	glDeleteBuffers(1, &lightEBO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);

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
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
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

void setupMesh(unsigned int& VAO, unsigned int& VBO, std::vector<glm::vec3> object, unsigned int containerElements)
{
	unsigned int stride = containerElements * sizeof(object[0]), offset = stride / 2;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, object.size() * sizeof(object[0]), &object.front(), GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, containerElements * sizeof(object[0]), (void*)0);
	glEnableVertexAttribArray(0);

	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	glEnableVertexAttribArray(1);
}

void setupMesh(unsigned int& VAO, unsigned int& VBO, std::vector<float> object, unsigned int containerElements)
{
	unsigned int stride = containerElements * sizeof(object[0]), offset = stride / 2;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, object.size() * sizeof(object[0]), &object.front(), GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, containerElements * sizeof(object[0]), (void*)0);
	glEnableVertexAttribArray(0);

	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	glEnableVertexAttribArray(1);
}

void setupMesh(unsigned int& VAO,
					  unsigned int& VBO, 
					  unsigned int& EBO, 
					  std::vector<float> object, 
					  std::vector<unsigned int> objectIndicies, 
					  unsigned int containerElements)
{
	unsigned int stride = containerElements * sizeof(object[0]), offset = stride / 2;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, object.size() * sizeof(object), &object.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, objectIndicies.size() * sizeof(objectIndicies[0]), &objectIndicies.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, containerElements * sizeof(object[0]), (void*)0);
	glEnableVertexAttribArray(0);
}