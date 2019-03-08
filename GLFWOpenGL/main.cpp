#include "pch.h"
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

// Vertex Shader
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

// Fragment Shader
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

// Main 
int main()
{
	glfwInit();
	// Windows Hints (Parameters of our window)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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

	// Define shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// Compile Shader
	glCompileShader(vertexShader);
	// Compile error
	GLint success;
	// Compile logs (buffer)
	GLchar infoLog[512];

	// Returns compilation status and values of parameters
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Define fragment shader (same process as vertex shader)
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Link shader (attach our shaders to shaderProgram)
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for link errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	// Generating error logs 
	if (!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Now they're part of the program we can delete those shaders.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Setup vertex data and attributes pointers
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,		// Left
		0.5f, -0.5f, 0.0f,		// Right
		0.0f, 0.5f, 0.0f		// Top
	};

	// Creation of Vertex Buffer Object and Vertex Array Object
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind VAO
	glBindVertexArray(VAO);
	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Attrib pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// unbind buffer and VA
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Window loop
	while(!glfwWindowShouldClose(window))
	{
		// Retrieve Events such as keypresses, mouses movements etc.
		glfwPollEvents();

		//Render 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Tell the program to use our shader program
		glUseProgram(shaderProgram);
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