#include "Shader.h"

void Shader::deleteShader(unsigned int shaderID) const {
	glDeleteShader(shaderID);
}

Shader::Shader() {
	shaderProgram = glCreateProgram();
}

void Shader::loadShaderSource(const std::string &shaderFile) {
	std::ifstream inputShader(shaderFile);
	if (!inputShader.is_open()) {
		std::cerr << "failed to open shader file\n";
		return;
	}

	std::string shaderContent = "";
	std::string line = "";
	while (std::getline(inputShader, line)) {
		shaderContent += line + "\n";
	}
	inputShader.close();

	#ifdef _DEBUG
	std::cout << shaderContent + "\n";
	#endif


	shaderSource = shaderContent;
}

void Shader::compileShader(GLenum shaderType) const {
	const char* source = shaderSource.c_str();
	unsigned int shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &source, NULL);
	glCompileShader(shaderID);

	int success;
	char infoLog[512];

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	#ifdef _DEBUG
	if (!success) {
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cerr << "failed to compile " << shaderType << " shader: " << infoLog;
	}
	#endif

	glAttachShader(shaderProgram, shaderID);

	deleteShader(shaderID);
}

void Shader::linkProgram() const {
	glLinkProgram(shaderProgram);
}

void Shader::useShader() const {
	glUseProgram(shaderProgram);
}

unsigned int Shader::getProgram() const {
	return shaderProgram;
}