#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <glad/glad.h>

class Shader
{
private:
	mutable std::string shaderSource;
	unsigned int shaderProgram;
	void deleteShader(unsigned int shaderID) const;
public:
	Shader();
	void loadShaderSource(const std::string &shaderFile);
	void compileShader(GLenum type) const;
	void linkProgram() const;
	void useShader() const;

	unsigned int getProgram() const;
};