// graphics
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>

// Custom headers
#include "glInit.hpp"
#include "coordinates.hpp"
#include "shaderReader.hpp"

string vertexShaderSource = getShader("shaders/shader.vert");

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
	
	cout << vertexShaderSource;

	// Area OpenGL renders from (0,0) to (800, 800)
	glViewport(0, 0, 800, 800);

	// Specify the color of the background to GL_COLOR_BUFFER_BIT
	glClearColor(0.07f, 0.13f, 0.17, 1.0f);

	// When glfw's window is not closed do the while loop
	while(!glfwWindowShouldClose(window)){
		// Allows for the window to respond to events like resizing
		glfwPollEvents();

		// Clear the back buffer and assign new color
		glClear(GL_COLOR_BUFFER_BIT);

		// Swaps the drawn back buffer with the front buffer
		glfwSwapBuffers(window);
	}
	
	// Terminate glfw and the window
	glfwDestroyWindow(window);
	glfwTerminate();

}
