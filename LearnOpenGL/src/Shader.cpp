#include "Shader.h"

void Shader::remove(unsigned int shaderID) const {
	glDeleteShader(shaderID);
}

Shader::Shader() {
	shaderProgram = glCreateProgram();
}

void Shader::loadSources(const std::string &vertexFile, const std::string &fragmentFile) const {
	std::ifstream vertexShader(vertexFile);
	if (!vertexShader.is_open()) {
		std::cerr << "failed to open vertex shader file\n";
		return;
	}

	std::string vertexContent = "";
	std::string line = "";
	while (std::getline(vertexShader, line)) {
		vertexContent += line + "\n";
	}
	vertexShader.close();

	#ifdef _DEBUG
		std::cout << "Vertex Shader: \n";
		std::cout << vertexContent + "\n";
	#endif

	std::ifstream fragmentShader(fragmentFile);
	if (!fragmentShader.is_open()) {
		std::cerr << "failed to open fragment shader file\n";
		return;
	}

	std::string fragmentContent = "";
	line = "";
	while (std::getline(fragmentShader, line)) {
		fragmentContent += line + "\n";
	}
	fragmentShader.close();

	#ifdef _DEBUG
		std::cout << "Fragment Shader: \n";
		std::cout << vertexContent + "\n";
	#endif

	compile(vertexContent, GL_VERTEX_SHADER);
	compile(fragmentContent, GL_FRAGMENT_SHADER);
}

void Shader::compile(const std::string &shaderSource, GLenum shaderType) const {
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

	remove(shaderID);
}

void Shader::link() const {
	glLinkProgram(shaderProgram);
}

void Shader::use() const {
	glUseProgram(shaderProgram);
}

unsigned int Shader::getProgram() const {
	return shaderProgram;
}