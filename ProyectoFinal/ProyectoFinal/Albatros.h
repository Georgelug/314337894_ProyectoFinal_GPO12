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
	Model* AlbatrosCola;

	// Positions, model and matrix 4X4 
	glm::vec3 initialPosition;
	glm::vec3 position;
	glm::mat4 model;
	glm::mat4 tmp;

	// auxiliar variables that allows to rotate all of the models
	float rotBody;
	float rotAlaIzq;
	float rotAlaDer;
	float rotCola;

	// auxiliar vairables that allows to implement states machines so that the penguin can be animated and be moved depending on a particular path
	bool estadoBody;
	bool estadoCola;
	bool estadoAlaIzq;
	bool estadoAlaDer;
	bool faseRecorrido;
	bool faseRecorrido1;
	bool faseRecorrido2;
	bool faseRecorrido3;
	float rotRecorrido;

	// auxiliar parameters that allows to parameterize the equations of the line and circumferences 
	float theta;
	float t;

	// Class constructor that recieves the shader (lightingshader) to render all of the models that make up a penguin and the initial position of the penguin
	// note: inside of this function atribuites of this class are initialized
	Albatros(Shader* lightingShader, glm::vec3 initialPosition) {

		this->lightingShader = lightingShader;
		this->tmp = this->model = glm::mat4(1);
		this->position = this->initialPosition = initialPosition;

		this->rotBody = -9.0f;
		this->rotAlaIzq = -9.0f;
		this->rotAlaDer = 0.0f;
		this->rotCola = 9.0f;

		this->estadoBody = true;
		this->estadoCola = true;
		this->estadoAlaIzq = true;
		this->estadoAlaDer = true;

		this->faseRecorrido = true;
		this->faseRecorrido1 = false;
		this->faseRecorrido2 = false;
		this->faseRecorrido3 = false;

		this->rotRecorrido = 0.0f;
		this->theta = 0.0f;
		this->t = 0.03f;

		AlbatrosCuerpo = new Model((char*)"Models/Escena/Penguin/AlbatrosBody.obj");
		AlbatrosAlaDer = new Model((char*)"Models/Escena/Penguin/AlbatrosAlaDer.obj");
		AlbatrosAlaIzq = new Model((char*)"Models/Escena/Penguin/AlbatrosAlaIzq.obj");
	}

	// method that allows to render and animate the penguin, the overload depends on the path we want to use to animate the instance of the class penguin
	// the 1st overload of this method alows to call the 1st overload of the private method called recorrido(); notice that doesn't recieave anything as a parameter
	// because the design of this 1st path (a diamond) doesn't need another variable
	void PenguinAnimation(GLint modelLoc) {
		rotationDinamic();
		PutPenguin(modelLoc);
		recorrido();
	}
	// the 2nd overload of this method alows to call the 2nd overload of the private method called recorrido(); notice that recieaves a variable called distance
	// the distance is a parameter that is used to be the diameter of ta circumference and at the same time a linear route
	void PenguinAnimation(GLint modelLoc, float distance) {
		rotationDinamic();
		PutPenguin(modelLoc);
		recorrido(distance / 2.0);
	}

	// private methods
private:
	// method that allows to render every part of the penguin's body using hierarchical modeling so that we can render each part pf the body and they can be animated nicely in a uniform way
	// simulating the natural movement of a penguin
	void PutPenguin(GLint modelLoc) {

		// /Body/head/legs
		this->model = glm::mat4(1);
		this->tmp = this->model = glm::translate(this->model, this->initialPosition);
		this->model = glm::translate(this->model, this->position);
		this->tmp = this->model = glm::rotate(model, glm::radians(this->rotRecorrido), glm::vec3(0.0f, 1.0f, 0.0));
		this->tmp = this->model = glm::rotate(model, glm::radians(this->rotBody), glm::vec3(1.0f, 0.0f, 0.0));
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

	// method that allows to implement a state machine in such a way the penguin can walk like a real one
	void rotationDinamic() {
		if (this->estadoBody) {
			if (this->rotBody < 9.0f) {
				this->rotBody += 0.5f;
			}
			else this->estadoBody = false;
		}
		else {
			if (this->rotBody > -9.0f) {
				this->rotBody -= 0.5f;
			}
			else this->estadoBody = true;
		}

		if (this->estadoCola) {
			if (this->rotCola > -9.0f) {
				this->rotCola -= 1.0f;
			}
			else this->estadoCola = false;
		}
		else {
			if (this->rotCola < 9.0f) {
				this->rotCola += 1.0f;
			}
			else this->estadoCola = true;
		}

		if (this->estadoAlaIzq) {
			if (this->rotAlaIzq < 0.0f) {
				this->rotAlaIzq += 1.0f;
			}
			else this->estadoAlaIzq = false;
		}
		else {
			if (this->rotAlaIzq > -9.0f) {
				this->rotAlaIzq -= 1.0f;
			}
			else this->estadoAlaIzq = true;
		}

		if (this->estadoAlaDer) {
			if (this->rotAlaDer < 9.0f) {
				this->rotAlaDer += 1.0f;
			}
			else this->estadoAlaDer = false;
		}
		else {
			if (this->rotAlaDer > 0.0f) {
				this->rotAlaDer -= 1.0f;
			}
			else this->estadoAlaDer = true;
		}

	}

	// complex animations
	// notice that there is an overloaded method called recorrido so that we can differentiate between the two animations
	// the 1st one is a semicircumference path
	void recorrido(float r) {

		if (this->faseRecorrido) {
			this->rotRecorrido = 90.0f;
			this->position.z -= 0.05;

			if (this->position.z < (this->initialPosition.z - 2.0f * r)) {
				this->theta = 0.0f;
				this->faseRecorrido = false;
				this->faseRecorrido1 = true;
			}
		}
		if (this->faseRecorrido1) {
			this->rotRecorrido = this->theta + 180.0f;
			this->position.x = this->initialPosition.x - r * sin(glm::radians(this->theta));
			this->position.z = this->initialPosition.z - r - r * cos(glm::radians(this->theta));
			this->theta += 0.1f;
			if (theta > 180.0f) {
				this->rotRecorrido = -90.0f;
				this->faseRecorrido = true;
				this->faseRecorrido1 = false;
			}
		}
	}
	// the 2nd one is a diamond path
	void recorrido() {
		if (this->faseRecorrido) {

			this->position.z -= this->t;
			this->position.x += this->t;
			this->rotRecorrido = 45.0f;

			if (this->position.z < this->initialPosition.z / 2) {

				this->faseRecorrido = false;
				this->faseRecorrido1 = true;
			}
		}

		if (this->faseRecorrido1) {

			this->position.z -= this->t;
			this->position.x -= this->t;
			this->rotRecorrido = 135.0f;

			if (this->position.x < 0) {

				this->faseRecorrido1 = false;
				this->faseRecorrido2 = true;
			}
		}

		if (this->faseRecorrido2) {

			this->position.z += this->t;
			this->position.x -= this->t;
			this->rotRecorrido = 225.0f;

			if (this->position.z > this->initialPosition.z / 2) {

				this->faseRecorrido2 = false;
				this->faseRecorrido3 = true;
			}
		}

		if (this->faseRecorrido3) {

			this->position.z += this->t;
			this->position.x += this->t;
			this->rotRecorrido = -45.0f;

			if (this->position.x > 0) {
				this->faseRecorrido3 = false;
				this->faseRecorrido = true;
			}
		}
	}

};

#endif
