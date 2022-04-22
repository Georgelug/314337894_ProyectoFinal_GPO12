#ifndef SCENE_H
#define SCENE_H

#include <iostream>

class Scene {

public:

	Scene(){
		Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
		Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
		Model Edificio((char*)"Models/Escena/edificioTexturizadoV2.obj");
		Model Suelo((char*)"Models/Esfera/Suelo.obj");
	}

};

#endif