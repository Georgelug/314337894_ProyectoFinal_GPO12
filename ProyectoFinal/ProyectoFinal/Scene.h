#ifndef SCENE_H
#define SCENE_H

#include <iostream>

class Scene {

public:
	Shader *lightingShader;
	Shader *lampShader;

	Model *Suelo;
	Model *Snowman;
	Model* Edificio;
	
	glm::mat4 proyection;
	glm::mat4 view;
	glm::mat4 model;



	Scene(){
		lightingShader = new Shader("Shaders/lighting.vs", "Shaders/lighting.frag");
		lampShader = new Shader("Shaders/lamp.vs", "Shaders/lamp.frag");
		Suelo = new Model((char*)"Models/Escena/suelo.obj");
		Snowman = new Model((char*)"Models/Escena/Snowman.obj");
		Edificio = new Model((char*)"Models/Escena/edificioTexturizadoV2.obj");
	}



};

#endif