#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "camera.h"
#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	GLFWwindow* window;

	// Initialize the library 
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context 
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Examples", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current 
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR!" << std::endl;
	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	Shader lightShader("src/lighting.vs", "src/lighting.fs");
	Shader lampShader("src/lamp.vs", "src/lamp.fs");

	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
	};

	unsigned int cubeVAO, VBO;
	glCreateVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)( 3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;
	glCreateVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Loop until the user closes the window 
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input 
		processInput(window);

		// Render here 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render Cube
		lightShader.Use();
		// Init Uniforms
		lightShader.SetVec3("light.position", lightPos);
		lightShader.SetVec3("viewPos", camera.Position);

		//light properties
		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.5f);
		lightColor.z = sin(glfwGetTime() * 2.4f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = lightColor * glm::vec3(0.2f); // low influence
		lightShader.SetVec3("light.ambient", ambientColor);
		lightShader.SetVec3("light.diffuse", diffuseColor);
		lightShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//material properties
		lightShader.SetVec3("material.ambient", 1.0f, 0.4f, 0.5f);
		lightShader.SetVec3("material.specular", 0.5f, 1.0f, 0.4f);
		lightShader.SetVec3("material.diffuse", 0.5f, 0.5f, 0.5f);
		lightShader.SetFloat("material.shininess", 32.0f);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightShader.SetMat4("view", view);
		lightShader.SetMat4("projection", projection);

		// World transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightShader.SetMat4("model", model);

		// Render Cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// RenderLamp
		lampShader.Use();
		lampShader.SetMat4("projection", projection);
		lampShader.SetMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		lampShader.SetMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Swap front and back buffers 
		glfwSwapBuffers(window);

		// Poll for and process events 
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}