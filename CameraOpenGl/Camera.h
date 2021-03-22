#pragma once

int WIDTH = 800, HEIGHT = 600;

class Camera {
public:
	Camera() {
		// ������� ������
		// x - ������ ���������� ������, �������� ����� y - ������ ����������� �����, �������� ����, z - ��� ������, ��� ������ ������
		positionCamera = glm::vec3(0.0f, 0.0f, 3.0f);
		// ������� ����
		frontCamera = glm::vec3(0.0f, 0.0f, -1.0f);
		// ������ �����
		upCamera = glm::vec3(0.0f, 1.0f, 0.0f);
		// ����� - ����
		pitch = 0.0f;
		// ������� ������ - �����
		yaw = -90.0f;

	}
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(positionCamera, positionCamera + frontCamera, upCamera);
	}

	glm::mat4 GetProjectionMatrix() {
		// ���� ������, ����������� ������, ������� � ������� ��������� �������� ��������
		return glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	}

	void ReadInput() {
		// ������, ����������� � ����
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			positionCamera += frontCamera * speed;
		}
		// �����, ��������� ������������ ���� �������������
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			positionCamera -= glm::normalize(glm::cross(frontCamera, upCamera)) * speed;
		}
		// ������
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			positionCamera += glm::normalize(glm::cross(frontCamera, upCamera)) * speed;
		}
		// ����, ��������� �� ����
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			positionCamera -= frontCamera * speed;
		}
	}

	void MouseInput(sf::Window& window) {
		//�������
		//sf::Vector2i center(window.getSize().x / 2, window.getSize().y / 2);
		//sf::Mouse::setPosition(center, window);

		if (mouseMove) {
			mouseMove = false;
			xPos = sf::Mouse::getPosition(window).x;
			yPos = sf::Mouse::getPosition(window).y;
		}

		// �������� � �������� ��������� ����
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
	// ������� ������
	glm::vec3 positionCamera;
	glm::vec3 frontCamera;
	glm::vec3 upCamera;

	// �������� ������
	float speed = 0.05f;
	// ���� ������
	float yaw;
	float pitch;
	// ���������� ����
	float xPos = WIDTH / 2.0f;
	float yPos = HEIGHT / 2.0f;
	// �������� ����� � ������ ��� 
	bool mouseMove = true;
};