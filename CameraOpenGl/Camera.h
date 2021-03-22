#pragma once

int WIDTH = 800, HEIGHT = 600;

class Camera {
public:
	Camera() {
		// ïîçèöèÿ êàìåðû
		// x - êàìåðà ñäâèãàåòñÿ âïðàâî, êàðòèíêà âëåâî y - êàìåðà ïîäíèìàåòñÿ ââåðõ, êàðòèíêà âíèç, z - ÷åì áîëüøå, òåì äàëüøå êàìåðà
		positionCamera = glm::vec3(0.0f, 0.0f, 3.0f);
		// ïîçèöèÿ öåëè
		frontCamera = glm::vec3(0.0f, 0.0f, -1.0f);
		// âåêòîð ââåðõ
		upCamera = glm::vec3(0.0f, 1.0f, 0.0f);
		// ââåðõ - âíèç
		pitch = 0.0f;
		// ïîâîðîò âïðàâî - âëåâî
		yaw = -90.0f;

	}
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(positionCamera, positionCamera + frontCamera, upCamera);
	}

	glm::mat4 GetProjectionMatrix() {
		// ïîëå îáçîðà, ñîîòíîøåíèå ñòîðîí, áëèæíÿÿ è äàëüíÿÿ ïëîñêîñòü óñå÷åíèÿ ïèðàìèäû
		return glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	}

	void ReadInput() {
		// âïåðåä, ïðèáëèæåíèå ê öåëè
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			positionCamera += frontCamera * speed;
		}
		// âëåâî, âåêòîðíîå ïðîèçâåäåíèå äàåò ïåðïåíäèêóëÿð
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			positionCamera -= glm::normalize(glm::cross(frontCamera, upCamera)) * speed;
		}
		// âïðàâî
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			positionCamera += glm::normalize(glm::cross(frontCamera, upCamera)) * speed;
		}
		// âíèç, îòäàëåíèå îò öåëè
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			positionCamera -= frontCamera * speed;
		}
	}

	void MouseInput(sf::Window& window) {
		//òîðìîçà
		//sf::Vector2i center(window.getSize().x / 2, window.getSize().y / 2);
		//sf::Mouse::setPosition(center, window);

		if (mouseMove) {
			mouseMove = false;
			xPos = sf::Mouse::getPosition(window).x;
			yPos = sf::Mouse::getPosition(window).y;
		}

		// ñìåùåíèå ñ ïðîøëîãî ïîëîæåíèÿ ìûøè
		float xOffset = sf::Mouse::getPosition(window).x - xPos;
		float yOffset = yPos - sf::Mouse::getPosition(window).y;
		xPos = sf::Mouse::getPosition(window).x;
		yPos = sf::Mouse::getPosition(window).y;

		yaw += (xOffset * speed);
		pitch += (yOffset * speed);

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

	// ñêîðîñòü êàìåðû
	float speed = 0.05f;
	// óãëû ýéëåðà
	float yaw;
	float pitch;
	// êîîðäèíàòû ìûøè
	float xPos = WIDTH / 2.0f;
	float yPos = HEIGHT / 2.0f;
	// äâèæåíèå ìûøüþ â ïåðâûé ðàç 
	bool mouseMove = true;
};
