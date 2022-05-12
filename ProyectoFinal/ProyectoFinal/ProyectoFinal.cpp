#include <iostream>
#include <cmath>
#include <stdlib.h>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Penguin.h"

// Function prototypes

// Function that makes posible to listen the events from the keyboard
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Function that makes posible to listen the events from the mouse
void MouseCallback(GLFWwindow* window, double xPos, double yPos);

// Function that allows to move the camera depending on the events catched from the keyboard and mouse
void DoMovement();

// Function that allows to render a static model according to an intance from class Model, this function receives as parameters: 
// a Matrix 4X4 named model, the model's (Matrix 4x4) location from the shader named modelLoc, 
// the model that is wanted to redender and the shader that is going to be used to render the model 
void PutModel_static(glm::mat4 model, GLint modelLoc, Model objeto, Shader lightingShader);

// Function that allows to render a seal according to an intance from class Model, this function receives as parameters: 
// a Matrix 4X4 named model, the model's (Matrix 4x4) location from the shader named modelLoc, 
// the model named Foca that is wanted to redender and the shader that is going to be used to render the model 
void PutFoca(glm::mat4 model, GLint modelLoc, Model objeto, Shader lightingShader);

// Function that allows us to render all of the windows of our antartic station, this function receives as parameters:
// a Matrix 4X4 named model, the model's (Matrix 4x4) location from the shader named modelLoc, 
// the model (in this case we use the model named Ventanas) that is wanted to redender and the shader that is going to be used to render the model 
void PutGlass(glm::mat4 model, GLint modelLoc, Model objeto, Shader lightingShader);

// Function that allows us to render all of the lamps of our antartic station, in this case it's implemented two pointlights to light up inside of the main room, this function receives as parameters:
// a Matrix 4X4 named model, the model's (Matrix 4x4) location from the shader named modelLoc, 
// the model (in this case we use the model named Lamp1 and Lamp2) that is wanted to redender, the shader that is going to be used to render the model 
// and finally the index of the position declared and saved in the next code lines
void PutLamps(glm::mat4 model, GLint modelLoc, Model objeto, Shader lightingShader, int pointLightPosition);

// Function that allows us to render a fake bear to scare penguins away from the main entrance, this bear is going to be rotating about its y-axis, this function receives as parameters:
// a Matrix 4X4 named model, the model's (Matrix 4x4) location from the shader named modelLoc, 
// two models (the bear and its base), the shader that is going to be used to render the model 
// and finally the position were is wanted to be rendered our fake Bear
void PutPolarBear(glm::mat4 model, GLint modelLoc, Model Oso, Model baseOso, Shader lightingShader, glm::vec3 pos);

// Function that allows us to render and implement a simple animation coded from the shaders anim and anim2 this function receives as parameters:
// a Matrix 4X4 named model, the model's (Matrix 4x4) location from the shader named modelLoc, 
// two models (the bear and its base) and the shader that is going to be used to render the model
// note: inside of this function it's implemented a time given from the function glfwGetTime()
// and send its value to the shaders anim o anim2 dipending of the requirements of the model that is wanted to render
void PutModel_animated(glm::mat4 model, GLint modelLoc, Model objeto, Shader Anim);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 10.0f, 0.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(3.98986,7.5499,0.32613),    // lamp 1
	glm::vec3(18.9702,7.5499,0.32613),    // lamp 2 
};

// vertices of the model that allows to render triangles so that the object from the class object is going to be render
float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Vertices that allows to render triangles and in turn the cube where is implemented the images that make up the skybox of the scene
GLfloat skyboxVertices[] = {
	// Positions
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

// indices that allows to use the EBO (Element buffer object) so that it is rendered triangles in such way that make up well the skybox of the scene
GLuint indices[] =
{  // Note that we start from 0!
	0,1,2,3,
	4,5,6,7,
	8,9,10,11,
	12,13,14,15,
	16,17,18,19,
	20,21,22,23,
	24,25,26,27, 
	28,29,30,31,
	32,33,34,35
};

// Positions all containers
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);


	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	//#### Shaders ####
	// Shaders that allows to render lights and models illuminated objects
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	// Shaders that allows to render animation in real time by this shaders
	Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");
	Shader Anim2("Shaders/anim2.vs", "Shaders/anim2.frag");
	// Shader that allows to render the scene skybox
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	//#### Models #####
	// Building
	Model Edificio((char*)"Models/Escena/edificio.obj");
	Model Ventanas((char*)"Models/Escena/Instalaciones/ventana.obj");
	Model BaseLamps((char*)"Models/Escena/Instalaciones/baselamps.obj");
	Model Lamp1((char*)"Models/Escena/Instalaciones/lamp1.obj");
	Model Lamp2((char*)"Models/Escena/Instalaciones/lamp2.obj");

	 //Exterior
	Model Suelo((char*)"Models/Escena/suelo.obj");
	Model Mar((char*)"Models/Escena/Mar.obj"); 
	Model Estanques((char*)"Models/Escena/Estanques.obj"); 
	Model icebergs((char*)"Models/Escena/icebergs.obj"); 
	Model Focas((char*)"Models/Escena/Foca/Focas.obj");
	Model Oso((char*)"Models/Escena/OsoPolar/polar.obj");
	Model BaseOso((char*)"Models/Escena/BaseOso.obj");
	
	// penguins, this intances are from the class Penguin, this class alows us to render penguins animated and use their  two methods that implents animated paths
	Penguin penguin1(&(lightingShader),glm::vec3(40.0f, -1.0f,0.0f));
	Penguin penguin2(&(lightingShader),glm::vec3(0.0f, -0.5f,-50.0f));


	// Interior
	Model PlantaEnergia((char*)"Models/Escena/PlantaEnergia/plantaEnergia.obj"); 
	Model TanqueOxigeno((char*)"Models/Escena/TanqueOxigeno/tanqueOxigeno.obj"); 
	Model Armario((char*)"Models/Escena/Interior/Armario.obj"); 
	Model Tuberia((char*)"Models/Escena/Interior/tuberia.obj"); 
	Model Apagadores((char*)"Models/Escena/Instalaciones/apagadores.obj"); 
	Model Botas((char*)"Models/Escena/Instalaciones/botas.obj"); 
	Model Botiquines((char*)"Models/Escena/Instalaciones/botequin.obj"); 
	Model Extinguidores((char*)"Models/Escena/Instalaciones/FireExtinguisher/Extinguidores.obj");
	Model Martillo((char*)"Models/Escena/Instalaciones/martillo.obj");
	Model Mesa((char*)"Models/Escena/Instalaciones/mesa.obj");
	Model Piolets((char*)"Models/Escena/Instalaciones/piolet.obj");
	
	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	//glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures for the skybox
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/night/right.tga");
	faces.push_back("SkyBox/night/left.tga");
	faces.push_back("SkyBox/night/top.tga");
	faces.push_back("SkyBox/night/bottom.tga");
	faces.push_back("SkyBox/night/back.tga");
	faces.push_back("SkyBox/night/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light, in the scene it is night
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f,1.0f,1.0f);

		// lamps
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.972, 0.956, 0.909);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.972, 0.956, 0.909);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.1f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.2f);
		 
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.972, 0.956, 0.909);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.972, 0.956, 0.909);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.1f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"),0.2f);


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

		view = camera.GetViewMatrix();
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f,1.0f, 1.0f, 1.0f);

		//### Escena ###
		// objetos estaticos
		//Exterior
		PutModel_static(model,modelLoc,Suelo,lightingShader);
		PutModel_static(model,modelLoc,Edificio,lightingShader);
		
		// indoor objects
		PutModel_static(model, modelLoc, PlantaEnergia, lightingShader);
		PutModel_static(model, modelLoc, TanqueOxigeno, lightingShader);
		PutModel_static(model, modelLoc, Armario, lightingShader);
		PutModel_static(model, modelLoc, Tuberia, lightingShader);
		PutModel_static(model, modelLoc, Apagadores, lightingShader);
		PutModel_static(model, modelLoc, Botas, lightingShader);
		PutModel_static(model, modelLoc, Botiquines, lightingShader);
		PutModel_static(model, modelLoc, Extinguidores, lightingShader);
		PutModel_static(model, modelLoc, Martillo, lightingShader);
		PutModel_static(model, modelLoc, Mesa, lightingShader);
		PutModel_static(model, modelLoc, Piolets, lightingShader);
		PutModel_static(model, modelLoc, BaseLamps, lightingShader);
		PutFoca(model, modelLoc, Focas, lightingShader);
		PutPolarBear(model,modelLoc, Oso, BaseOso, lightingShader, glm::vec3(30.0f,-1.0f,10.0f));

		// Windows
		PutGlass(model, modelLoc, Ventanas, lightingShader);
		
		// ilumination
		PutLamps(model, modelLoc, Lamp1, lightingShader, 0);
		PutLamps(model, modelLoc, Lamp2, lightingShader, 1);

		// Penguins
		penguin1.PenguinAnimation(modelLoc,40.0f);
		penguin2.PenguinAnimation(modelLoc);


		glBindVertexArray(0);

		// outside animated objects
		Anim.Use();

		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// it is rendered with animation the Sea and the ponds, simulating the proper movement of water
		PutModel_animated(model, modelLoc, Mar, Anim);
		PutModel_animated(model, modelLoc, Estanques, Anim);

		glBindVertexArray(0);

		Anim2.Use();

		modelLoc = glGetUniformLocation(Anim2.Program, "model");
		viewLoc = glGetUniformLocation(Anim2.Program, "view");
		projLoc = glGetUniformLocation(Anim2.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// it is rendered with animation the icebergs simulating their floating 
		PutModel_animated(model, modelLoc, icebergs, Anim2);

		glBindVertexArray(0);

	


		// lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 2; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.1f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	//glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

void PutModel_static(glm::mat4 model, GLint modelLoc, Model objeto, Shader lightingShader) {
	model = glm::mat4(1);
	model = glm::scale(model,glm::vec3(0.15f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
	objeto.Draw(lightingShader);
}
void PutFoca(glm::mat4 model, GLint modelLoc, Model objeto, Shader lightingShader) {
	model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(90.0f,-2.0f,80.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
	objeto.Draw(lightingShader);
}
void PutModel_animated(glm::mat4 model, GLint modelLoc, Model objeto, Shader Anim) {
	model = glm::mat4(1);
	model = glm::scale(model, glm::vec3(0.15f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1f(glGetUniformLocation(Anim.Program, "time"), glfwGetTime());
	objeto.Draw(Anim);
}

void PutPolarBear(glm::mat4 model, GLint modelLoc, Model Oso, Model baseOso, Shader lightingShader, glm::vec3 pos) {
	// Base rotacion
	model = glm::mat4(1);
	model = glm::translate(model, pos);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
	baseOso.Draw(lightingShader);
	// Oso polar
	model = glm::mat4(1);
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians((float)glfwGetTime()*10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
	Oso.Draw(lightingShader);

}

void PutGlass(glm::mat4 model, GLint modelLoc, Model objeto, Shader lightingShader){
	glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::mat4(1);
	model = glm::scale(model, glm::vec3(0.15f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
	glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 0.25f, 0.25f, 0.25f, 0.7);
	objeto.Draw(lightingShader);
	glDisable(GL_BLEND);  //Desactiva el canal alfa 
	glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);
}

void PutLamps(glm::mat4 model, GLint modelLoc, Model objeto, Shader lightingShader, int pointLightPosition){
	glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::mat4(1);
	model = glm::translate(model, pointLightPositions[pointLightPosition]);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
	glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.9);
	objeto.Draw(lightingShader);
	glDisable(GL_BLEND);  //Desactiva el canal alfa 
	glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);
}



// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}