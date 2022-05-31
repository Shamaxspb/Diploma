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

// GLOBAL VARIABLES
// screen
const int START_WINDOW_WIDTH = 1440;	// created window width
const int START_WINDOW_HEIGHT = 900;	// created window height

// camera
Camera camera(glm::vec3(0.0f, 500.0f, -500.0f));
// initial mouse coordinates
float lastX = START_WINDOW_WIDTH / 2.0f;
float lastY = START_WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;	// if the first recieve of mouse input

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// lightning
glm::vec3 lightPosition(1000.0f, 1700.0f, 2500.0f);	// light sourse position


int main()
{
	// READ DATA FROM FILE
	std::vector<float> InputData;	// vector with all the data
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
		InputData.push_back(buffer);
	}

	// vectors to fill
	std::vector<float> waveHeight;		// wave height   data
	std::vector<float> shipRotation;	// ship rotation data
	std::vector<float> shipDrifting;	// ship drifting data
	std::vector<float>::iterator input_it = InputData.begin();
	for (auto input_it = InputData.begin(); input_it != InputData.end(); ++input_it)
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

	// CREATE WAVE VERTICES VECTOR ==========================================================================

	// variables to fill waveVertices vector:
	std::vector<Vertex> waveVertices;	// - vertices with colors vector
	// - positions
	float x	= 0;
	float& x1 = x, & x2 = x;
	float y1 = 100, y2 = -100;	// width of the polygon

	// - colors (converting to 0:1 scope)
	/*float r1 = (float)39 / 255, g1 = (float)183 / 255, b1 = (float)209 / 255;	// color 1
	float r2 = (float)13 / 255, g2 = (float) 99 / 255, b2 = (float)131 / 255;	// color 2
	float r3 = (float)55 / 255, g3 = (float)108 / 255, b3 = (float)155 / 255;	// color 3
	float r4 = (float)37 / 255, g4 = (float) 66 / 255, b4 = (float) 70 / 255;	// color 4*/
	// - TEST colors (converting to 0:1 scope)
	float r1 = (float) 55 / 255, g1 = (float)219 / 255, b1 = (float)230 / 255;	// color 1 (Turquoise)
	float r2 = (float)100 / 255, g2 = (float)250 / 255, b2 = (float)255 / 255;	// color 2 (Electric)
	float r3 = (float) 82 / 255, g3 = (float)230 / 255, b3 = (float)243 / 255;	// color 3 (Spray)
	float r4 = (float) 21 / 255, g4 = (float)226 / 255, b4 = (float)233 / 255;	// color 4 (Bright turquoise)

	// - normals
	
	// filling vector with values
	// "I wonder if this could be used as triangles to form a normals"
	auto wave_it = waveHeight.begin();
	for (; wave_it != waveHeight.end(); ++wave_it)
	{
		waveVertices.emplace_back(x1, y1, *wave_it);
		waveVertices.emplace_back(x2, y2, *wave_it);
		x += 15;
	}
	
	std::fstream waveFile;
	// read file
	waveFile.open("WaveHeight.txt", std::fstream::in);
	if (!waveFile.is_open())
	{
		std::cout << "ERROR\tcouldn't open WaveHeight.txt" << std::endl;
		return -1;
	}

	// fill vector with vertices and colors
	unsigned int colorDecider = 2;
	/*while (!waveFile.eof())
	{
		waveFile >> z;

		if (colorDecider % 2 == 0)
		{
			// vertex1 position
			waveVertices.push_back(x1);	// x1
			waveVertices.push_back(y1);	// y1
			waveVertices.push_back(z1);	// z1
			// vertex1 color
			waveVertices.push_back(r1);	// r1
			waveVertices.push_back(g1);	// g1
			waveVertices.push_back(b1);	// b1

			// vertex2 position
			waveVertices.push_back(x2);	// x2
			waveVertices.push_back(y2);	// y2
			waveVertices.push_back(z2);	// z2

			// vertex2 color
			waveVertices.push_back(r2);	// r1
			waveVertices.push_back(g2);	// g1
			waveVertices.push_back(b2);	// b1
		}
		else
		{
			// vertex3 position
			waveVertices.push_back(x1);	// x1
			waveVertices.push_back(y1);	// y1
			waveVertices.push_back(z1);	// z1

			// vertex3 color
			waveVertices.push_back(r3);	// r1
			waveVertices.push_back(g3);	// g1
			waveVertices.push_back(b3);	// b1

			// vertex4 position
			waveVertices.push_back(x2);	// x2
			waveVertices.push_back(y2);	// y2
			waveVertices.push_back(z2);	// z2

			// vertex4 color
			waveVertices.push_back(r4);	// r1
			waveVertices.push_back(g4);	// g1
			waveVertices.push_back(b4);	// b1
		}

		x += 15;
		colorDecider++;
	}*/
	

	// NORMALS
	

	// CREATE WAVE INDICIES VECTOR ==========================================================================

	std::vector<int> waveIndicies;	// indicies vector
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

	// mesh to test color blending ==========================================================================
	std::vector<float> testVector = {
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
	std::vector<int> testIndicies = {
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

	// shader program compiling and linking
	Shader waveShader("Shaders/waveShader.vs", "Shaders/waveShader.fs");
	Shader lightingShader("Shaders/lightningShader.vs", "Shaders/lightningShader.fs");	// not in use atm
	Shader lightSourceShader("Shaders/lightSourceShader.vs", "Shaders/lightSourceShader.fs");

	// PASS WAVE TO GPU
	unsigned int waveVAO, waveVBO, waveEBO;

	// create and bind wave VAO
	glGenVertexArrays(1, &waveVAO);
	glGenBuffers(1, &waveVBO);
	glGenBuffers(1, &waveEBO);

	glBindVertexArray(waveVAO);

	glBindBuffer(GL_ARRAY_BUFFER, waveVBO);
	glBufferData(GL_ARRAY_BUFFER, waveVertices.size() * sizeof(waveVertices), &waveVertices.front(), GL_STATIC_DRAW);	// actual
	//glBufferData(GL_ARRAY_BUFFER, testVector.size() * sizeof(testVector), &testVector.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waveEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, waveIndicies.size() * sizeof(waveIndicies), &waveIndicies.front(), GL_STATIC_DRAW);	// actual
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, testIndicies.size() * sizeof(testIndicies), &testIndicies.front(), GL_STATIC_DRAW);

	// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(waveVertices[0]), (void*)0);	// actual
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(waveVertices[0]), (void*)0);	// vertices with no color
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(waveVertices[0]), (void*)0);			// actual (vector<Vertex>)
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(testVector[0]), (void*)0);		// for color testing
	glEnableVertexAttribArray(0);

	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(waveVertices[0]), (void*)(3 * sizeof(waveVertices[0])));	// actual
	////glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(testVector[0]), (void*)(3 * sizeof(testVector[0])));
	//glEnableVertexAttribArray(1);

	// PASS LIGHT SOURCE TO GPU
	unsigned int lightVAO, lightVBO, lightEBO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);
	glGenBuffers(1, &lightEBO);

	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, lightSource.size() * sizeof(lightSource), &lightSource.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, lightSourceIndicies.size() * sizeof(lightSourceIndicies), &lightSourceIndicies.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(lightSource[0]), (void*)0);
	glEnableVertexAttribArray(0);

	waveShader.Use();

	// TEST FIELD +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	

	// TEST FIELD ---------------------------------------------------------------------------------------------------------------------------------

	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	waveShader.Use();
	waveShader.setVec3("lightPosition", lightPosition);

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

		// WAVE RENDER
		// activate shader
		waveShader.Use();
		// set wave color config
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
		glm::vec3 objectColor((float)82 / 255, (float)230 / 255, (float)243 / 255);
		waveShader.setVec3("lightColor", lightColor);
		waveShader.setVec3("objectColor", objectColor);
		
		// projection matrix and camera / view matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)START_WINDOW_WIDTH / (float)START_WINDOW_HEIGHT, 0.1f, 30000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		waveShader.setMat4("projection", projection);
		waveShader.setMat4("view", view);		
		
		// model matrix
		glm::mat4 model = glm::mat4(1.0f); // set identity matrix is neñessarily
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(-1.0, 0.0, 0.0) * (currentFrame * 175.0f));	// WAVE TRANSLATES BY THIS MATRIX (multiplier is the speed of wave)
		waveShader.setMat4("model", model);
				
		// draw wave
		glBindVertexArray(waveVAO);
		glDrawElements(GL_TRIANGLES, waveIndicies.size(), GL_UNSIGNED_INT, 0);	// for water mesh (actual)
		glBindVertexArray(0);

		// LIGHT SOURCE RENDER
		// matrices can be configured in any order, order in multiplication in shaders - thats what really matters
		lightSourceShader.Use();
		lightSourceShader.setMat4("projection", projection);	
		lightSourceShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition);
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
	glDeleteBuffers(1, &waveEBO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);

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