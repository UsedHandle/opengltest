// Windowing and OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>
#include <string>

// Custom headers
#include "glInit.hpp"
#include "coordinates.hpp"
#include "shaderHandler.hpp"

using namespace std;

// glShaderSource() only takes in chars and not strings
// note: this is only a temp. solution. I am planning to make shaders into objects like in learnopengl.com 's tutorial 
// Gets shader from file
string vertexShaderSourceString = getShader("shaders/shader.vert");
const char* vertexShaderSource = vertexShaderSourceString.c_str();
string fragmentShaderSourceString = getShader("shaders/shader.frag");
const char* fragmentShaderSource = fragmentShaderSourceString.c_str(); 

int main(){
	// Inits glfw and core profile
	glfwGLInit();

	// Creates window with a width of xxx, a height of xxx, a name of xxx, without fullscreen, and window to share resources with 
	GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpengl", NULL, NULL);

	// Checks if there was no errors
	if(window == NULL){
		glfwTerminate();
		
		std::cout << "Failed to create a window with GLFW" << std::endl;

		return -1;
	}

	// Tells glfw that the window will be used
	glfwMakeContextCurrent(window);

	// Loads configurations for OpenGL
	gladLoadGL();

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;

		return -1;
	}

	// Creates vertex shader, links source code to shader, and compiles it
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShader, &vertexShaderSource);

	// Simliar to above but it creates a fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShader, &fragmentShaderSource);
	
	// Creates shader program, attaches shaders, and connects the io of shaders 
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Deletes shaders as they are already in the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Area OpenGL renders from (0,0) to (800, 800)
	glViewport(0, 0, 800, 800);

	// Specify the color of the background to GL_COLOR_BUFFER_BIT
	glClearColor(0.07f, 0.13f, 0.17, 1.0f);

	// while window is open 
	while(!glfwWindowShouldClose(window)){
		// Check for events (like resizing)
		glfwPollEvents();

		// Draw GL_COLOR_BUFFER_BIT to the back buffer 
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// Swap the drawn back buffer with the front buffer
		glfwSwapBuffers(window);
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Terminate glfw and the window
	glfwDestroyWindow(window);
	glfwTerminate();

}
