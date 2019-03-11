#include "pch.h"
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// shader.h
#include "shader.h"

// soil2 - texture application
#include <SOIL2.h>

const GLint WIDTH = 800, HEIGHT = 600;

// Main 
int main()
{
	glfwInit();
	// Windows Hints (Parameters of our window)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);            	// OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // OpenGL Core Profile
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // OpenGL Compatibility
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);					    // OpenGL Windows resizable ?

	// Window object that we can use with GLFW's functions
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	// Check whether or not the windows is successfully created
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	// Enable Modern GLEW
	glewExperimental = GL_TRUE;

	// Check if GLEW is successfully initialized
	if(GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
	}
	
	glViewport(0, 0, screenWidth, screenHeight);

	// Enable texture alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	Shader ourShader("core.vs", "core.frag");

	// Setup vertex data and attributes pointers
	GLfloat vertices[] =
	{
		// Positions           // Colors
		0.5f, 0.5f, 0.0f,      1.0f, 0.0f, 0.0f,      1.0f, 1.0f,   // Top right
		0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,      1.0f, 0.0f,   // Bottom right
		-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,      0.0f, 0.0f,   // Bottom left
		-0.5f, 0.5f, 0.0f,     1.0f, 1.0f, 1.0f,      0.0f, 1.0f,   // Top Left 
	};
	GLuint indices[] =
	{
		0, 1, 3, // First triangle
		1, 2, 3
	};

	// Creation of Vertex Buffer Object and Vertex Array Object
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &EBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Color attribute | 6 * sizeof(GLfloat) -> 6 means 2 values for each vertex
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	// unbind VA
	glBindVertexArray(0);

	// Load and create a texture
	GLuint texture;
	int width, height;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Window loop
	while(!glfwWindowShouldClose(window))
	{
		// Retrieve Events such as keypresses, mouses movements etc.
		glfwPollEvents();

		//Render 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Tell the program to use our shader program
		ourShader.Use();
		// Bind VAO in order to draw arrays
		glBindVertexArray(VAO);
		// Draw them
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// Unbind them by passing 0 to the method.
		glBindVertexArray(0);

		// Swap screen buffers
		glfwSwapBuffers(window);
	}
	// De-allocate resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Terminate GLFW

	glfwTerminate();
	return EXIT_SUCCESS;
}