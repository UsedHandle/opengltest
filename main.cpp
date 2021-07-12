// Windowing and OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>

// Custom headers
#include "glInit.hpp"
#include "coordinates.hpp"
#include "shaderHandler.hpp"

using namespace std;

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
	
	// Creates shader program
	GLuint shaderProgram = makeShader("shaders/shader.vert", "shaders/shader.frag");

	// Creates and VBO, VAO, and EBO
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Chooses the VAO as the vertex array to be modified, binds a VBO to the VAO and passes vertices to the VBO
	// GL_STATIC_DRAW means that the data will not be changed, used many times, and will be drawn to the screen
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Chooses the EBO as the element to be modified, and passes in the indices
	// GL_STATIC_DRAW still means the same thing as above
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// Tells the vertex shader how the data from the binded vbo 
	// Tells glsl the location that was told in the .vert, how many numbers per point, the type, 
	// if it is normalized, the amount bytes in each point, and where the data begins in the vbo 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// Unbinds the objects
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Area OpenGL renders from (0,0) to (800, 800)
	glViewport(0, 0, 800, 800);

	// Specify the color of the background to GL_COLOR_BUFFER_BIT
	glClearColor(0.07f, 0.13f, 0.17, 1.0f);

	glUseProgram(shaderProgram);

	// while window is open 
	while(!glfwWindowShouldClose(window)){
		// Check for events (like resizing)
		glfwPollEvents();

		// Draw GL_COLOR_BUFFER_BIT to the back buffer 
		glClear(GL_COLOR_BUFFER_BIT);	
		
		// Binds the VAO to draw the data
		glBindVertexArray(VAO);
		
		// Uses the EBO to draw triangles, the amount of points to draw, the type of values, and the offset
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		// Swap the drawn back buffer with the front buffer
		glfwSwapBuffers(window);
	}
	
	glDeleteProgram(shaderProgram);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


	// Terminate glfw and the window
	glfwDestroyWindow(window);
	glfwTerminate();

}
