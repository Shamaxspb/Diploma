#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<Shader_T.h>
#include<Texture.h>
#include<Mesh_T.h>
#include<Model_T.h>
#include<Light/light.h>
#include<Camera.h>

//#include<ShaderProgram.h>
//#include<Mesh.h>
//#include<Model.h>

#include"Models/Cube.hpp"
#include"Models/Lamp.hpp"

//#include<ShipMesh.h>
//#include<ShipModel.h>
//#include<filesystem.h>
//#include<FILE_3D.h>

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include<iostream>
#include<fstream>
#include<sstream>
#include<streambuf>
#include<string>
#include<vector>
#include<iterator>
#include"windows.h"

using namespace std;

// function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void setupMesh(unsigned int& VAO, unsigned int& VBO, std::vector<glm::vec3> object, unsigned int containerElements);
void setupMesh(unsigned int& VAO, unsigned int& VBO, std::vector<float> object, unsigned int containerElements);
void setupMesh(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, std::vector<float> object, 
	std::vector<unsigned int> objectIndices, unsigned int containerElements);

// GLOBAL VARIABLES
// screen
const int START_WINDOW_WIDTH = 1440;	// created window width
const int START_WINDOW_HEIGHT = 900;	// created window height

// camera
Camera camera(glm::vec3(-300.0f, 550.0f, -750.0f));
// initial mouse coordinates
float lastX = START_WINDOW_WIDTH / 2.0f;
float lastY = START_WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;	// if the first recieve of mouse input

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// lightning
//					x	     z			y
glm::vec3 lightPos(750.0f, -195000.0f, 0.0f);		// light source position
glm::vec3 cubePosition(420.0f, 750.0f, 0.0f);		// test cube position


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

	// distribute date between vectors
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

	// convert shipDrifting to glm::vec3
	std::vector<glm::vec3> shipDriftingV3;
	for (auto sd_i = shipDrifting.begin(); sd_i != shipDrifting.end(); )
	{
		float x, y, z;
		x = *sd_i;
		sd_i++;
		y = *sd_i;
		sd_i++;
		z = *sd_i;
		shipDriftingV3.emplace_back(x, y, z);
		if (sd_i != shipDrifting.end())
		{
			sd_i++;
		}
	}
	

	// WAVE FORM CONFIGURATION ============================================================================== Wave config

	float x = 0;					// polygon length
	float y = 50000;				// polygon width


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
	std::advance(wave_next, 1);
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
	std::advance(ci, 1);
	auto pi = waveCoordsUnique.begin();	// parallel vertex iterator
	auto fi = waveCoordsUnique.begin();	// further  vertex iterator
	std::advance(fi, 3);

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
	std::vector<unsigned int> lightSourceIndices = {
		//indices	// triangles
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

	


	SpotLight s =
	{
		camera.Position, camera.Front,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)),
		glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f)
	};

	// SHIP MODEL =========================================================================================== Ship model

	stbi_set_flip_vertically_on_load(true);
	//Model Ship("D:/Programming/Projects/Diploma/Diploma/Diploma/Models/S60_2400.obj");
	//Model Ship("D:/Programming/Projects/Diploma/Diploma/Diploma/backpack/backpack.obj");
	//ShipModel myShip("D:/Programming/Projects/Diploma/Diploma/Diploma/Models/S60_2400.obj");

	

	// SHADERS ============================================================================================== Shaders
	
	// shader program compiling and linking
	Shader lightingShader("Shaders/lightingShader.vs", "Shaders/lightingShader.fs");
	Shader lightSourceShader("Shaders/lightSourceShader.vs", "Shaders/lightSourceShader.fs");
	Shader modelShader("Shaders/model_loading.vs", "Shaders/model_loading.fs");

	Shader shaderT ("Shaders/myObject.vs", "Shaders/myObject.fs");
	Shader lampShaderT ("Shaders/myObject.vs", "Shaders/Lamp.fs");

	Cube cube(Material::emerald, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
	cube.init();

	Lamp lamp(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(-1.0f, -0.5f, -0.5f), glm::vec3(0.25f));
	lamp.init();

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,   2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f),
	};

	DirLight dirLight = { glm::vec3(-0.2, -1.0f, -0.3f), glm::vec3(0.1f), glm::vec3(0.4f), glm::vec3(0.75f) };

	Model m(glm::vec3(0.0f), glm::vec3(0.25f));
	m.loadModel("Models/lotr_troll/scene.gltf");

	// CREATE OBJECTS ======================================================================================= Objects
	
	// create wave 
	unsigned int waveVAO, waveVBO;
	setupMesh(waveVAO, waveVBO, wave, 2);

	// create light source
	unsigned int lightVAO, lightVBO, lightEBO;
	setupMesh(lightVAO, lightVBO, lightEBO, lightSource, lightSourceIndices, 3);

	// create test cube 
	unsigned int cubeVAO, cubeVBO; 
	setupMesh(cubeVAO, cubeVBO, testCube, 6);


	// PRE-RENDER CONIFGURATION ================================================================================ Pre-render config
	
	// OUT OF LOOP CONFIGS
	// iterators
	auto rotation_it = shipRotation.begin();
	auto rotation_end = shipRotation.rbegin();
	std::advance(rotation_it, 150);

	auto drifting_it = shipDriftingV3.begin();
	auto drifting_end = shipDriftingV3.rbegin();
	std::advance(drifting_it, 50);

	wave_it = waveHeight.begin();
	auto wave_end = waveHeight.rbegin();
	std::advance(wave_it, 50);

	float shipX = 0.0f;
	glm::vec3 initialWavePosition(0.0f, 0.0f, 0.0f);		// wave basis
	glm::vec3 initialShipPosition(1250.0f, 50.0f,1500.0f);	// ship basis

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// RENDER LOOP ========================================================================================== Render Loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame logic
		float currentFrame	= static_cast<float>(glfwGetTime());	// getting current time of frame
		deltaTime			= currentFrame - lastFrame;
		lastFrame			= currentFrame;

		// wave and ship deltas
		glm::vec3 waveMovement(shipX, 0.0f, 0.0f);		// wave delta
		glm::vec3 surfaceContact(0.0f, *wave_it, 0.0f);	// object bottom and wave surface synchronisation
		glm::vec3 shipDeltaPos = *drifting_it * 0.5f;	// ship drifting delta

		// rotation deltas
		float deltaPitch, deltaRoll, deltaYaw;
		deltaPitch = *rotation_it;
		if (*rotation_it != *rotation_end)
			rotation_it++;
		deltaRoll = *rotation_it;
		if (*rotation_it != *rotation_end)
			rotation_it++;
		deltaYaw = *rotation_it;

		// input
		processInput(window);
		

		// RENDER INITIATION
		glClearColor(0.72f, 0.79f, 0.96f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderT.activate();
		shaderT.set3Float("lightPos", lamp.pos);
		shaderT.set3Float("viewPos", camera.Position);

		shaderT.set3Float("light.ambient", lamp.ambient);
		shaderT.set3Float("light.diffuse", lamp.diffuse);
		shaderT.set3Float("light.specular", lamp.specular);

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)START_WINDOW_WIDTH / (float)START_WINDOW_HEIGHT, 0.1f, 200000.0f);

		shaderT.setMat4("view", view);
		shaderT.setMat4("projection", projection);

		cube.render(shaderT);

		lampShaderT.activate();
		lampShaderT.setMat4("view", view);
		lampShaderT.setMat4("projection", projection);
		lamp.render(lampShaderT);



		dirLight.render(shaderT);

		s.position = camera.Position;
		s.direction = camera.Front;
		s.render(shaderT);

		// WAVE CONFIGURATION
		// colors
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
		glm::vec3 objectColor((float)0 / 255, (float)116 / 255, (float)142 / 255);	// wave color
		// activate shader and pass there matricec
		//lightingShader.Use();
		lightingShader.activate();
		lightingShader.setVec3("objectColor", objectColor);
		lightingShader.setVec3("lightColor", lightColor);
		lightingShader.setVec3("lightPos", lightPos/* * glm::vec3(0.0f, -90.0f, 0.0f)*/);
		lightingShader.setVec3("viewPos", camera.Position);		
		// projection matrix and camera / view matrix
		projection = glm::perspective(glm::radians(camera.Zoom), (float)START_WINDOW_WIDTH / (float)START_WINDOW_HEIGHT, 0.1f, 200000.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		// model matrix
		model = glm::mat4(1.0f); // set identity matrix is neñessarily
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(-1.0, 0.0, 0.0) * (currentFrame * 150.0f));	// WAVE TRANSLATES BY THIS MATRIX (multiplier is the speed of wave)
		model = glm::translate(model, initialWavePosition + waveMovement);
		lightingShader.setMat4("model", model);
		// draw wave
		glBindVertexArray(waveVAO);
		glDrawArrays(GL_TRIANGLES, 0, wave.size());
		glBindVertexArray(0);


		//// TEST CUBE CONFIGURATION (orange one)
		// rotation
		//lightingShader.Use();
		lightingShader.activate();
		lightingShader.setVec3("objectColor", 1.0, 0.4f, 0.0f);
		lightingShader.setVec3("lightColor", lightColor);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		// drifting
		model = glm::translate(model, initialShipPosition + (surfaceContact + shipDeltaPos));	
		// rotation
		model = glm::rotate(model, glm::radians(deltaPitch), glm::vec3(0.0f, 0.0f, 1.0f));	// Pitch
		model = glm::rotate(model, glm::radians(deltaRoll), glm::vec3(1.0f, 0.0f, 0.0f));	// Roll
		model = glm::rotate(model, glm::radians(deltaYaw), glm::vec3(0.0f, 1.0f, 0.0f));	// Yaw
		model = glm::scale(model, glm::vec3(100.0f));
		lightingShader.setMat4("model", model);
		// draw test cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, testCube.size());
		glBindVertexArray(0);

		// TEST TEST TEST
		m.render(shaderT);
		

		// SHIP CONFIGURATION
		// ship model, ship mesh
		/*model = glm::mat4(1.0f);
		model = glm::translate(model, initialShipPosition);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelShader.setMat4("model", model);
		Ship.Draw(modelShader);*/
		// model, mesh
		/*glm::mat4 shipModel = glm::mat4(1.0f);
		shipModel = glm::translate(shipModel, initialShipPosition);
		shipModel = glm::scale(shipModel, glm::vec3(500.0f, 500.0f, 500.0f));
		lightingShader.setMat4("model", shipModel);
		myShip.Draw(lightingShader);*/

		// LIGHT SOURCE CONFIGURATION
		// matrices should be configured in order: Rotate, Translate, Scale
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(15.0f));
		lightSourceShader.setMat4("model", model);
		// draw light source
		glBindVertexArray(lightVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		Sleep(15);

		// swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();

		// scene state change
		if (*wave_it != *wave_end)
			wave_it++;
		else wave_it = waveHeight.begin();

		if (*rotation_it != *rotation_end)
			rotation_it++;
		else rotation_it = shipRotation.begin();

		if (*drifting_it != *drifting_end)
			drifting_it++;
		else drifting_it = shipDriftingV3.begin();

		shipX -= 25;
	}

	glDeleteVertexArrays(1, &waveVAO);
	glDeleteBuffers(1, &waveVBO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);
	glDeleteBuffers(1, &lightEBO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);

	cube.cleanup();
	lamp.cleanup();

	m.cleanup();

	glfwTerminate();
	return 0;
}

// FUNCTION BLOCK =========================================================================================== Function Block

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
					  std::vector<unsigned int> objectIndices, 
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, objectIndices.size() * sizeof(objectIndices[0]), &objectIndices.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, containerElements * sizeof(object[0]), (void*)0);
	glEnableVertexAttribArray(0);
}