#include "Camera.h"

Camera::Camera()
{
	// позиция камеры
	// x - камера сдвигается вправо, картинка влево y - камера поднимается вверх, картинка вниз, z - чем больше, тем дальше камера
	positionCamera = glm::vec3(0.0f, 0.0f, 3.0f);
	// позиция цели
	frontCamera = glm::vec3(0.0f, 0.0f, -1.0f);
	// вектор вверх
	upCamera = glm::vec3(0.0f, 1.0f, 0.0f);
	// вверх - вниз
	pitch = 0.0f;
	// поворот вправо - влево
	yaw = -90.0f;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(positionCamera, positionCamera + frontCamera, upCamera);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	// поле обзора, соотношение сторон, ближняя и дальняя плоскость усечения пирамиды
	return glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

void Camera::ReadInput()
{
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

void Camera::MouseInput(sf::Window& window)
{
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

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontCamera = glm::normalize(front);
}