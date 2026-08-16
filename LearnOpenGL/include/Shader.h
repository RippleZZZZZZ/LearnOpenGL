#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <glad/glad.h>

class Shader
{
private:
	mutable unsigned int shaderProgram;
	void remove(unsigned int shaderID) const;
public:
	Shader();
	void loadSources(const std::string &vertexFile, const std::string &fragmentFile) const;
	void compile(const std::string &shaderSource, GLenum type) const;
	void link() const;
	void use() const;

	unsigned int getProgram() const;
};