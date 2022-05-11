#ifndef PENGUIN_H
#define PENGUIN_H

#include <iostream>

class Penguin{

public:
	// shader to use
	Shader *lightingShader;

	// Parts of the penguin that they can be rotated
	Model* penguinCuerpo;
	Model* penguinAlaDer;
	Model* penguinAlaIzq;
	Model* penguinCola;
	
	// Positions, model and matrix 4X4 
	glm::vec3 initialPosition;
	glm::vec3 position;
	glm::mat4 model;
	glm::mat4 tmp;


	float rotBody;
	float rotAlaIzq;
	float rotAlaDer;
	float rotCola;

	bool estadoBody;
	bool estadoCola;
	bool estadoAlaIzq;
	bool estadoAlaDer;
	bool faseRecorrido;
	bool faseRecorrido1;
	float rotRecorrido;
	float theta;
	
	Penguin(Shader *lightingShader, glm::vec3 initialPosition){

		this->lightingShader = lightingShader;
		this->tmp = this->model = glm::mat4(1);
		this->position = this->initialPosition =  initialPosition;

		this->rotBody = -9.0f;
		this->rotAlaIzq = -9.0f;
		this->rotAlaDer = 0.0f;
		this->rotCola = 9.0f;
		this->estadoBody = true;
		this->estadoCola = true;
		this->estadoAlaIzq= true;
		this->estadoAlaDer= true;
		this->faseRecorrido = true;
		this->faseRecorrido1 = false;
		this->rotRecorrido = 0.0f;
		this->theta = 0.0f;

		penguinCuerpo = new Model((char*)"Models/Escena/Penguin/penguinCuerpo.obj");
		penguinAlaDer = new Model((char*)"Models/Escena/Penguin/penguinAlaDer.obj");
		penguinAlaIzq = new Model((char*)"Models/Escena/Penguin/penguinAlaIzq.obj");
		penguinCola = new Model((char*)"Models/Escena/Penguin/penguinColaobj.obj");
	}

	void PenguinAnimation(GLint modelLoc,float distance = 20.0f) {
		rotationDinamic();
		PutPenguin(modelLoc);
		recorrido(distance/2.0);
	}
	
	void PenguinAnimation(GLint modelLoc) {
		rotationDinamic();
		PutPenguin(modelLoc);
		recorrido();
	}

private:
	void PutPenguin(GLint modelLoc){
		
		// Cuerpo
		this->model = glm::mat4(1);
		this->tmp = this->model = glm::translate(this->model, this->initialPosition);
		this->model = glm::translate(this->model, this->position);
		this->tmp = this->model = glm::rotate(model, glm::radians(this->rotRecorrido), glm::vec3(0.0f, 1.0f, 0.0));
		this->tmp =  this->model = glm::rotate(model, glm::radians(this->rotBody), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));
		glUniform1i(glGetUniformLocation(this->lightingShader->Program, "activaTransparencia"), 0);
		penguinCuerpo->Draw(*lightingShader);

		// Ala izquierda
		this->model = glm::mat4(1);
		this->model = glm::translate(this->model, this->position);
		this->model = glm::translate(this->tmp, glm::vec3(-0.0574 ,1.9875,0.6));
		//this->model = glm::rotate(model, glm::radians(this->rotRecorrido), glm::vec3(0.0f, 1.0f, 0.0));
		this->model = glm::rotate(model, glm::radians(this->rotAlaIzq), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));
		glUniform1i(glGetUniformLocation(this->lightingShader->Program, "activaTransparencia"), 0);
		penguinAlaIzq->Draw(*lightingShader);
		
		// Ala derecha
		this->model = glm::mat4(1);
		this->model = glm::translate(this->model, this->position);
		this->model = glm::translate(this->tmp, glm::vec3(-0.0574, 1.9875, -0.6));
		//this->model = glm::rotate(model, glm::radians(this->rotRecorrido), glm::vec3(0.0f, 1.0f, 0.0));
		this->model = glm::rotate(model, glm::radians(this->rotAlaDer), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));
		glUniform1i(glGetUniformLocation(this->lightingShader->Program, "activaTransparencia"), 0);
		penguinAlaDer->Draw(*lightingShader);
		
		// Cola
		this->model = glm::mat4(1);
		this->model = glm::translate(this->model, this->position);
		this->model = glm::translate(this->tmp, glm::vec3(0.0f));
		//this->model = glm::rotate(model, glm::radians(this->rotRecorrido), glm::vec3(0.0f, 1.0f, 0.0));
		this->model = glm::rotate(model, glm::radians(this->rotCola), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));
		glUniform1i(glGetUniformLocation(this->lightingShader->Program, "activaTransparencia"), 0);
		penguinCola->Draw(*lightingShader);

	}

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
			else this->estadoCola= false;
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
			else this->estadoAlaIzq= true;
		}

		if (this-> estadoAlaDer) {
			if (this->rotAlaDer < 9.0f) {
				this->rotAlaDer += 1.0f;
			}
			else this->estadoAlaDer = false;
		}
		else {
			if (this->rotAlaDer > 0.0f) {
				this->rotAlaDer -= 1.0f;
			}
			else this->estadoAlaDer= true;
		}

	}

	void recorrido(float r) {
		
		if (this->faseRecorrido) {
			this->rotRecorrido = 90.0f;
			this->position.z -= 0.05;
			
			if (this->position.z < (this->initialPosition.z - 2.0f*r)){
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
			if (theta> 180.0f){
				this->rotRecorrido = -90.0f;
				this->faseRecorrido = true;
				this->faseRecorrido1 = false;
			}
		}
	}
	
	void recorrido() {
		switch (this->faseRecorrido1){
			case 0:
				
				break;
			case 1:
				
				break;
		}
	}

};

#endif