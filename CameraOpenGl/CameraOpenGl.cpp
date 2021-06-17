#pragma once
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "stb_image.h"
#include "shaderLoader.h"
#include "TextureLoading.h"
#include "VertexArraysObject.h"
#include "VertexBufferObjects.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"

int main() {
	//начало
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "WindowLight",
		sf::Style::Titlebar | sf::Style::Close, settings);

	window.setMouseCursorVisible(false);
	//window.setMouseCursorGrabbed(true);

	glewExperimental = GL_TRUE;
	glEnable(GL_DEPTH_TEST);

	if (GLEW_OK != glewInit()) {
		std::cout << "Error:: glew not init =(" << std::endl;
		return -1;
	}
	//

	//шейдеры
	Shader LightingShader("files/shaders/lighting.vs", "files/shaders/lighting.fs");
	Shader LightCubeShader("files/shaders/cube.vs", "files/shaders/cube.fs");
	//

	//камера
	Camera camera;
	//

	//вершины куба
	float vertices[] = {
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	//позиции кубов
	glm::vec3 PositionsCube[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 4.0f, -5.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -6.3f),
		glm::vec3(-3.8f, 0.0f, -6.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -6.5f),
		glm::vec3(-1.7f, 0.0f, -6.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(3.5f, 1.0f, -1.5f)
	};

	//позиции светящихся кубов
	glm::vec3 PositionsLight[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-2.0f, 1.0f, -9.0f),
		glm::vec3(-1.0f, 4.0f, -9.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	//VBO - объекты вершинного буфера могут хранить большое количество вершин в памяти графического процессора.
	VertexArraysObject lightVAO;
	VertexBufferObjects VBO(vertices, sizeof(vertices) / sizeof(float));
	VBO.Binding();

	lightVAO.Binding();
	lightVAO.VertexAttribPointer(0, 3, 8 * sizeof(float), (void*)0);
	lightVAO.VertexAttribPointer(1, 3, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	lightVAO.VertexAttribPointer(2, 2, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//VAO для световых кубов
	VertexArraysObject cubeVAO;
	cubeVAO.Binding();
	cubeVAO.VertexAttribPointer(0, 3, 8 * sizeof(float), (void*)0);
	
	//Текстуры
	TextureLoading diffuseMap("files/images/box.jpg");
	TextureLoading specularMap("files/images/box2.jpg");
	//

	LightingShader.Use();
	LightingShader.SetInt("material.diffuse", 0);
	LightingShader.SetInt("material.specular", 1);

	bool isGo = true;
	while (true) {
		//обработка движений камеры
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
			case sf::Event::Closed:
				isGo = false;
				break;
			case sf::Event::KeyPressed:
				camera.ReadInput();
				break;
			case sf::Event::MouseMoved:
				camera.MouseInput(window);
				break;
			default:
				break;
			}
		}
		//

		//
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//

		//активируем шейдер
		LightingShader.Use();
		LightingShader.SetVec3("viewPos", camera.GetPosition());
		LightingShader.SetFloat("material.shininess", 32.0f);

		//направленный свет
		Light dirLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f));
		LightingShader.SetVec3("dirLight.direction", dirLight.direction);
		LightingShader.SetVec3("dirLight.ambient", dirLight.ambient);
		LightingShader.SetVec3("dirLight.diffuse", dirLight.diffuse);
		LightingShader.SetVec3("dirLight.specular", dirLight.specular);
		//

		//источники света
		for (int i = 0; i < 5; i++) {
			LightingShader.SetVec3("pointLights[" + std::to_string(i) + "].position", PositionsLight[i]);
			LightingShader.SetVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			LightingShader.SetVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			LightingShader.SetVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
			LightingShader.SetFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
			LightingShader.SetFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			LightingShader.SetFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032);
		}
		//

		//Прожектор
		LightingShader.SetVec3("spotLight.position", camera.GetPosition());
		LightingShader.SetVec3("spotLight.direction", camera.GetFront());
		LightingShader.SetVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		LightingShader.SetVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		LightingShader.SetVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		LightingShader.SetFloat("spotLight.constant", 1.0f);
		LightingShader.SetFloat("spotLight.linear", 0.09);
		LightingShader.SetFloat("spotLight.quadratic", 0.032);
		LightingShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.0f)));
		LightingShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		//

		//
		diffuseMap.Binding(0);
		specularMap.Binding(1);
		//

		auto view = camera.GetViewMatrix();
		auto projection = camera.GetProjectionMatrix();

		//рисуем коробки
		LightingShader.SetMat4("view", view);
		LightingShader.SetMat4("projection", projection);

		lightVAO.Binding();
		for (int i = 0; i < 12; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, PositionsCube[i]);
			model = glm::rotate(model, glm::radians(30.0f * (i + 3)), glm::vec3(1.0f, 0.3f, 0.5f));
			LightingShader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//

		//рисуем лампы
		LightCubeShader.Use();
		LightCubeShader.SetMat4("view", view);
		LightCubeShader.SetMat4("projection", projection);

		cubeVAO.Binding();
		for (unsigned int i = 0; i < 5; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, PositionsLight[i]);
			model = glm::scale(model, glm::vec3(0.7f));
			LightCubeShader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		window.display();
	}

	window.close();
	return 0;
}