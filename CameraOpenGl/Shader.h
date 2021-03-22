#pragma once

class Shader {
public:
	Shader(std::string vertexPath, std::string fragmentPath) {
		shaderProgram = LoadShaders(vertexPath, fragmentPath);
	}
	void Use() {
		glUseProgram(shaderProgram);
	}
	void SetMat4(std::string id, const glm::mat4& data) {
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, id.c_str()), 1, GL_FALSE, &data[0][0]);
	}

private:
	unsigned int shaderProgram;
};