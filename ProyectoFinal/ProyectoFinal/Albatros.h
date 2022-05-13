#ifndef ALBATROS_H
#define ALBATROS_H

#include <iostream>

// Class Penguin that allows to render an animated penguin with a tour (non linear animation) on the scene
class Albatros {

public:
	// shader to use
	Shader* lightingShader;

	// Parts of the penguin that they can be rotated
	Model* AlbatrosCuerpo;
	Model* AlbatrosAlaDer;
	Model* AlbatrosAlaIzq;


	// Positions, model and matrix 4X4 
	glm::vec3 initialPosition;
	glm::vec3 position;
	glm::mat4 model;
	glm::mat4 tmp;

	// auxiliar variables that allows to rotate all of the models
	float rotBody;
	float rotAlaIzq;
	float rotAlaDer;
	float rotRecorrido;
	// auxiliar vairables that allows to implement states machines so that the penguin can be animated and be moved depending on a particular path
	bool estadoBody;
	bool estadoAlaIzq;
	bool estadoAlaDer;

	// auxiliar parameters that allows to parameterize the equations of the line and circumferences 
	float theta;

	// Class constructor that recieves the shader (lightingshader) to render all of the models that make up an Albatros and the initial position of the penguin
	// note: inside of this function atribuites of this class are initialized
	Albatros(Shader* lightingShader, glm::vec3 initialPosition) {

		this->lightingShader = lightingShader;
		this->tmp = this->model = glm::mat4(1);
		this->position = this->initialPosition = initialPosition;

		this->rotBody = -15.0f;
		this->rotAlaIzq = 0.0f;
		this->rotAlaDer = 0.0f;
		this->rotRecorrido = 0.0f;

		this->estadoBody = true;
		this->estadoAlaIzq = true;
		this->estadoAlaDer = true;
		
		this->theta = 0.0f;

		AlbatrosCuerpo = new Model((char*)"Models/Escena/Albatros/AlbatrosBody.obj");
		AlbatrosAlaDer = new Model((char*)"Models/Escena/Albatros/AlbatrosAlaDer.obj");
		AlbatrosAlaIzq = new Model((char*)"Models/Escena/Albatros/AlbatrosAlaIzq.obj");
	}

	// this method alows to call the private method called recorrido(); notice that recieaves a variable called distance
	// the distance is a parameter that is used to be the diameter of a circumference
	void AlbatrosAnimation(GLint modelLoc, float distance) {
		rotationDinamic();
		PutAlbatros(modelLoc);
		recorrido(distance / 2.0);
	}

	// private methods
private:
	// method that allows to render every part of the Albatros's body using hierarchical modeling so that we can render each part of the body and they can be animated nicely in a uniform way
	// simulating the natural Albatros movement
	void PutAlbatros(GLint modelLoc) {

		// /Body/head/legs
		this->model = glm::mat4(1);
		this->tmp = this->model = glm::translate(this->model, this->initialPosition);
		this->model = glm::translate(this->model, this->position);
		this->tmp = this->model = glm::rotate(model, glm::radians(this->rotBody), glm::vec3(1.0f, 0.0f, 0.0));
		this->tmp = this->model = glm::rotate(model, glm::radians(this->rotRecorrido), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));
		glUniform1i(glGetUniformLocation(this->lightingShader->Program, "activaTransparencia"), 0);
		AlbatrosCuerpo->Draw(*lightingShader);

		// left wing
		this->model = glm::mat4(1);
		this->model = glm::translate(this->model, this->position);
		this->model = glm::translate(this->tmp, glm::vec3(0.0f));
		this->model = glm::rotate(model, glm::radians(this->rotAlaIzq), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));
		glUniform1i(glGetUniformLocation(this->lightingShader->Program, "activaTransparencia"), 0);
		AlbatrosAlaIzq->Draw(*lightingShader);

		// right wing
		this->model = glm::mat4(1);
		this->model = glm::translate(this->model, this->position);
		this->model = glm::translate(this->tmp, glm::vec3(0.0f));
		this->model = glm::rotate(model, glm::radians(this->rotAlaDer), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));
		glUniform1i(glGetUniformLocation(this->lightingShader->Program, "activaTransparencia"), 0);
		AlbatrosAlaDer->Draw(*lightingShader);

	}

	// method that allows to implement a state machine in such a way the bird can fly like a real one
	void rotationDinamic() {
		this->rotBody = -15.0f;
		if (this->estadoAlaIzq) {
			if (this->rotAlaIzq < 45.0f) {
				this->rotAlaIzq += 1.0f;
			}
			else this->estadoAlaIzq = false;
		}
		else {
			if (this->rotAlaIzq > 0.0f) {
				this->rotAlaIzq -= 1.0f;
			}
			else this->estadoAlaIzq = true;
		}

		if (this->estadoAlaDer) {
			if (this->rotAlaDer > -45.0f) {
				this->rotAlaDer -= 1.0f;
			}
			else this->estadoAlaDer = false;
		}
		else {
			if (this->rotAlaDer < 0.0f) {
				this->rotAlaDer += 1.0f;
			}
			else this->estadoAlaDer = true;
		}

	}

	// complex animations
	// method that moves the bird in a circumference path
	void recorrido(float r) {
		this->rotRecorrido = this->theta + 180.0f;
		this->position.x = this->initialPosition.x - r * sin(glm::radians(this->theta));
		this->position.z = this->initialPosition.z - r - r * cos(glm::radians(this->theta));
		this->theta += 0.25f;
		if (theta > 360.0f) {
			this->theta = 0.0f;
		}
	}
	
};
#endif