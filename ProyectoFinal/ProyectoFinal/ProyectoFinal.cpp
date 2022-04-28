#include <iostream>
#include <cmath>

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

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void PutModel_static(glm::mat4 model, GLint modelLoc, Model objeto, Shader lightingShader);
void PutModel_animated(glm::mat4 model, GLint modelLoc, Model objeto, Shader Anim);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(10.0f, 10.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.5f, 2.0f, 0.5f);
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(-5.0f,0.0f, 5.0f),    // 1er cuadrante
	glm::vec3(5.0f,0.0f, 5.0f),    // 2do cuadrante
	glm::vec3(-5.0f,0.0f, -5.0f), // 3er cuadrante
	glm::vec3(5.0f,0.0f,  -5.0f) // 4to cuadrante
};

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



glm::vec3 Light0 = glm::vec3(0);
glm::vec3 Light1 = glm::vec3(0);
glm::vec3 Light2 = glm::vec3(0);
glm::vec3 Light3 = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

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

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");
	Shader Anim2("Shaders/anim2.vs", "Shaders/anim2.frag");

	//#### Modelos #####
	// Edificio
	Model Edificio((char*)"Models/Escena/edificioTexturizadoV2.obj");

	// Exterior
	Model Suelo((char*)"Models/Escena/suelo.obj");
	Model Mar((char*)"Models/Escena/Mar.obj"); // animacion
	Model Estanques((char*)"Models/Escena/Estanques.obj"); // animacion
	Model icebergs((char*)"Models/Escena/icebergs.obj"); // animacion
	Model Snowman((char*)"Models/Escena/Snowman.obj"); 
	Model penguin((char*)"Models/Escena/Penguin/p1.obj"); // animacion compleja
	Model penguin1((char*)"Models/Escena/Penguin/p2.obj"); // animacion compleja

	// Interior
	Model PlantaEnergia((char*)"Models/Escena/PlantaEnergia/plantaEnergia.obj");
	Model TanqueOxigeno((char*)"Models/Escena/TanqueOxigeno/tanqueOxigeno.obj");
	Model TanqueOxigeno1((char*)"Models/Escena/TanqueOxigeno/tanqueOxigeno1.obj");
	Model Armario((char*)"Models/Escena/Interior/Armario.obj");
	Model Armario1((char*)"Models/Escena/Interior/Armario1.obj");
	Model Armario2((char*)"Models/Escena/Interior/Armario2.obj");
	Model tuberia((char*)"Models/Escena/Interior/tuberia.obj");

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



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);


		//// Point light 1
	 //   glm::vec3 lightColor;
		//lightColor.x= abs(sin(glfwGetTime() *Light0.x));
		//lightColor.y= abs(sin(glfwGetTime() *Light0.y));
		//lightColor.z= sin(glfwGetTime() *Light1.z);

		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.7f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),1.8f);


		//// SpotLight
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), lightPos.x, lightPos.y, lightPos.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), lightDir.x, lightDir.y, lightDir.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),1.0f, 1.0f, 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.35f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.44f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(45.0f)));
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(60.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

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


		//Carga de modelo 
		view = camera.GetViewMatrix();
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f,1.0f, 1.0f, 1.0f);

		//### Escena ###
		// objetos estaticos
		//Exterior
		PutModel_static(model,modelLoc,Suelo,lightingShader);
		PutModel_static(model,modelLoc,Edificio,lightingShader);
		PutModel_static(model,modelLoc,Snowman,lightingShader);
		PutModel_static(model,modelLoc,penguin,lightingShader);
		PutModel_static(model,modelLoc,penguin1,lightingShader);
		
		// objetos del interior
		PutModel_static(model, modelLoc, PlantaEnergia, lightingShader);
		PutModel_static(model, modelLoc, TanqueOxigeno, lightingShader);
		PutModel_static(model, modelLoc, TanqueOxigeno1, lightingShader);
		PutModel_static(model, modelLoc, Armario, lightingShader);
		PutModel_static(model, modelLoc, Armario1, lightingShader);
		PutModel_static(model, modelLoc, Armario2, lightingShader);
		PutModel_static(model, modelLoc, tuberia, lightingShader);


		glBindVertexArray(0);

		// objetos animados del exterior
		Anim.Use();

		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

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

		PutModel_animated(model, modelLoc, icebergs, Anim2);

		glBindVertexArray(0);

		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		//model = glm::mat4(1);
		//model = glm::translate(model, pointLightPositions[0]);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"),0);
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"),1.0f,1.0f,0.0f,0.75);
		//Esfera.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"),1.0f,1.0f,1.0f,1.0f);



		// Also draw the lamp object, again binding the appropriate shader
		//lampShader.Use();
		//// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		//modelLoc = glGetUniformLocation(lampShader.Program, "model");
		//viewLoc = glGetUniformLocation(lampShader.Program, "view");
		//projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//// Set matrices
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//model = glm::mat4(1);
		//model = glm::translate(model, lightPos);
		////model = glm::rotate(model, glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f));
		//model = glm::scale(model, glm::vec3(0.5f)); // Make it a smaller cube
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//// Draw the light object (using light's vertex attributes)
		//for (GLuint i = 0; i < 4; i++)
		//{
		//	model = glm::mat4(1);
		//	model = glm::translate(model, pointLightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glBindVertexArray(VAO);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		//glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

void PutModel_static(glm::mat4 model, GLint modelLoc, Model objeto, Shader lightingShader) {
	model = glm::mat4(1);
	model = glm::scale(model,glm::vec3(0.25f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
	objeto.Draw(lightingShader);
}
void PutModel_animated(glm::mat4 model, GLint modelLoc, Model objeto, Shader Anim) {
	model = glm::mat4(1);
	model = glm::scale(model, glm::vec3(0.25f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1f(glGetUniformLocation(Anim.Program, "time"), glfwGetTime());
	objeto.Draw(Anim);
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
	// Spotlight
	// teclas para mover la posicion
	if (keys[GLFW_KEY_T])
	{
		lightPos.z += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		lightPos.z -= 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		lightPos.x += 0.01f;
	}

	if (keys[GLFW_KEY_F])
	{
		lightPos.x -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		lightPos.y += 0.1f;
	}
	if (keys[GLFW_KEY_Y])
	{
		lightPos.y -= 0.01f;
	}

	// teclas para mover la direccion
	if (keys[GLFW_KEY_I])
	{
		lightDir.z += 0.01f;
	}
	if (keys[GLFW_KEY_K])
	{
		lightDir.z -= 0.01f;
	}

	if (keys[GLFW_KEY_L])
	{
		lightDir.x += 0.01f;
	}

	if (keys[GLFW_KEY_J])
	{
		lightDir.x -= 0.01f;
	}
	if (keys[GLFW_KEY_P])
	{
		lightDir.y += 0.1f;
	}
	if (keys[GLFW_KEY_O])
	{
		lightDir.y -= 0.01f;
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

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light0 = glm::vec3(1.0f, 0.0f, 1.0f);
			Light1 = glm::vec3(0.0f, 1.0f, 0.0f);
			Light2 = glm::vec3(1.0f, 0.0f, 0.0f);
			Light3 = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			Light0 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
			Light2 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
			Light3 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente

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