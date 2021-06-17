#pragma once
#include <SFML/Window.hpp>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:
	Camera();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	void ReadInput();
	void MouseInput(sf::Window&);

	}
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(positionCamera, positionCamera + frontCamera, upCamera);
	}

	glm::mat4 GetProjectionMatrix() {
		// поле обзора, соотношение сторон, ближняя и дальняя плоскость усечения пирамиды
		return glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	}

	void ReadInput() {
		// вперед, приближение к цели
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			positionCamera += frontCamera * speed;
		}
		// влево, векторное произведение дает перпендикуляр
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			positionCamera -= glm::normalize(glm::cross(frontCamera, upCamera)) * speed;
		}
		// вправо
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			positionCamera += glm::normalize(glm::cross(frontCamera, upCamera)) * speed;
		}
		// вниз, отдаление от цели
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			positionCamera -= frontCamera * speed;
		}
	}

	void MouseInput(sf::Window& window) {
		//тормоза
		//sf::Vector2i center(window.getSize().x / 2, window.getSize().y / 2);
		//sf::Mouse::setPosition(center, window);

		if (mouseMove) {
			mouseMove = false;
			xPos = sf::Mouse::getPosition(window).x;
			yPos = sf::Mouse::getPosition(window).y;
		}

		// смещение с прошлого положения мыши
		float xOffset = sf::Mouse::getPosition(window).x - xPos;
		float yOffset = yPos - sf::Mouse::getPosition(window).y;
		xPos = sf::Mouse::getPosition(window).x;
		yPos = sf::Mouse::getPosition(window).y;

		yaw += (xOffset * speed);
		pitch += (yOffset * speed);

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		frontCamera = glm::normalize(front);
	}

private:
	// ïîçèöèÿ êàìåðû
	glm::vec3 positionCamera;
	glm::vec3 frontCamera;
	glm::vec3 upCamera;

	// скорость камеры
	float speed = 0.05f;
	// óãëû ýéëåðà
	float yaw;
	float pitch;
	// координаты мыши
	float xPos = WIDTH / 2.0f;
	float yPos = HEIGHT / 2.0f;
	// движение мышью в первый раз 
	bool mouseMove = true;
};
