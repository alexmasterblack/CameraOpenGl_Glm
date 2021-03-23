#pragma once

#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "stb_image.h"
#include "shaderLoader.h"
#include "Shader.h"
#include "Camera.h"

int main() {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "First Window",
		sf::Style::Titlebar | sf::Style::Close);

	window.setMouseCursorVisible(false);

	window.setMouseCursorGrabbed(true);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Error:: glew not init =(" << std::endl;
		return -1;
	}

	Shader myShader("res/shaders/e4.vs", "res/shaders/e4.fs");

	Camera camera;

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("res/imgs/1.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	bool isGo = true;
	while (isGo) {
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

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);

		myShader.Use();

		glBindVertexArray(VAO);

		glm::mat4 model = glm::mat4(1.0f);
		auto view = camera.GetViewMatrix();
		auto prj = camera.GetProjectionMatrix();

		myShader.SetMat4("model", model);
		myShader.SetMat4("view", view);
		myShader.SetMat4("projection", prj);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window.display();
	}

	window.close();
	return 0;
}