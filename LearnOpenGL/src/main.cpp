#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

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

[[nodiscard]] static unsigned int createTriangleVAO(Vertice bottomLeft, Vertice bottomRight, Vertice top) {
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

[[nodiscard]] static unsigned int createRectangleVAO() {
	std::array<float, 30> vertices{
		-0.4f, -0.4f, 0.0f,		0.0f, 0.0f, 0.0f, 
		 0.4f, -0.4f, 0.0f,		1.0f, 0.0f, 0.0f,
		 0.4f,  0.4f, 0.0f,		1.0f, 1.0f, 0.0f,
		-0.4f,  0.4f, 0.0f,		0.0f, 1.0f, 0.0f

	};

	std::array<unsigned int, 6> indices{
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VAO, VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(1, 0);
	glBindVertexArray(0);

	return VAO;
}

[[nodiscard]] static unsigned int createTexture(const std::string &file) {
	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	const char* fileLocation = file.c_str();
	unsigned char* data = stbi_load(fileLocation, &width, &height, &nrChannels, 0);

	if (!data) {
		std::cerr << "failed to load image data: " << stbi_failure_reason() << "\n";
		return 0;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	stbi_image_free(data);
	return texture;
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

	auto rectangleVAO = createRectangleVAO();
	auto texture = createTexture("resources/textures/duhh.png");

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		inputHandler();

		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(rectangleVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}