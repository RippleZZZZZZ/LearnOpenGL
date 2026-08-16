#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include "Shader.h"

struct Vertice {
	float x;
	float y;
	float z;

	Vertice(float x, float y, float z) : x{ x }, y{ y }, z{ z } {};
};

GLFWwindow* window = nullptr;

static void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

static void inputHandler() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

static unsigned int compileShader(GLenum type, const char* source, unsigned int shaderProgram) {
	unsigned int shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &source, NULL);
	glCompileShader(shaderID);

	int success = NULL;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cerr << "Failed to compile shader " << type << ": " << infoLog << "\n";
	}

	glAttachShader(shaderProgram, shaderID);
	return shaderID;
}

static unsigned int createLetterEVAO() {
	std::array<float, 42> vertices{
		-0.5f,  0.8f, 0.0f, // top base of E
		-0.3f,  0.8f, 0.0f, // top base of E
		-0.3f, -0.8f, 0.0f, // bottom base of E
		-0.5f, -0.8f, 0.0f, // bottom base of E
		 0.5f,  0.8f, 0.0f, 
		 0.5f,  0.6f, 0.0f, 
		-0.3f,  0.6f, 0.0f, 
		-0.3f,  0.1f, 0.0f, 
		 0.5f,  0.1f, 0.0f, 
		 0.5f, -0.1f, 0.0f,  
		-0.3f, -0.1f, 0.0f, 
		-0.3f, -0.6f, 0.0f,
		 0.5f, -0.6f, 0.0f,
		 0.5f, -0.8f, 0.0f,
	};

	std::array<unsigned int, 24> indices{
		0, 1, 2,
		0, 3, 2,
		1, 4, 6,
		4, 5, 6,
		7, 8, 10,
		8, 9, 10,
		11, 12, 2,
		12, 13, 2
	};

	unsigned int VAO = NULL;
	unsigned int VBO = NULL;
	unsigned int EBO = NULL;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(1, 0);
	glBindVertexArray(0);
	
	return VAO;
}

static unsigned int createTriangleVAO(Vertice bottomLeft, Vertice bottomRight, Vertice top) {
	std::array<float, 18> vertices{
		bottomLeft.x, bottomLeft.y, bottomLeft.z,
		bottomRight.x, bottomRight.y, bottomRight.z,
		top.x, top.y, top.z
	};

	unsigned int VAO = NULL;
	unsigned int VBO = NULL;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(1, 0);
	glBindVertexArray(0);

	return VAO;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);

	if (!window) {
		std::cerr << "failed to create window";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "failed to init glad";
		return -1;
	}
	
	auto shader = Shader{};
	shader.loadSources("resources/shaders/vertex.vert", "resources/shaders/fragment.frag");
	shader.link();

	auto triangleVAO0 = createTriangleVAO(Vertice{ -1.2f, 0.5f, 0.0f }, Vertice{ -0.6f, 0.5f, 0.0f }, Vertice{ -0.9f, -0.5f, 0.0f });
	auto triangleVAO1 = createTriangleVAO(Vertice{ -0.8f, -0.5f, 0.0f }, Vertice{ -0.2f, -0.5f, 0.0f }, Vertice{ -0.5f, 0.5f, 0.0f });
	auto triangleVAO2 = createTriangleVAO(Vertice{ -0.4f, 0.5f, 0.0f }, Vertice{ 0.2f, 0.5f, 0.0f }, Vertice{ -0.1f, -0.5f, 0.0f });
	auto triangleVAO3 = createTriangleVAO(Vertice{ 0.0f, -0.5f, 0.0f }, Vertice{ 0.6f, -0.5f, 0.0f }, Vertice{ 0.3f, 0.5f, 0.0f });
	auto triangleVAO4 = createTriangleVAO(Vertice{ 0.4f, 0.5f, 0.0f }, Vertice{ 1.0f, 0.5f, 0.0f }, Vertice{ 0.7f, -0.5f, 0.0f });

	float xPos = 0.1f;

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		inputHandler();

		shader.use();

		float time = (float)glfwGetTime() * 3.5f;
		float color = (sin(time) / 2.0f) + 0.5f;
		auto fragmentUniform = glGetUniformLocation(shader.getProgram(), "color");
		glUniform3f(fragmentUniform, color, color, color);

		glBindVertexArray(triangleVAO0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		color = (sin(time - 0.9f) / 2.0f) + 0.5f;
		glUniform3f(fragmentUniform, color, color, color);

		glBindVertexArray(triangleVAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		color = (sin(time - 1.8f) / 2.0f) + 0.5f;
		glUniform3f(fragmentUniform, color, color, color);

		glBindVertexArray(triangleVAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		color = (sin(time - 2.7f) / 2.0f) + 0.5f;
		glUniform3f(fragmentUniform, color, color, color);

		glBindVertexArray(triangleVAO3);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		color = (sin(time - 3.6f) / 2.0f) + 0.5f;
		glUniform3f(fragmentUniform, color, color, color);

		glBindVertexArray(triangleVAO4);
		glDrawArrays(GL_TRIANGLES, 0, 4);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}