// std
#include <iostream>

// Window Events and Graphics
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Custom headers
#include "glInit.hpp"
#include "coordinates.hpp"
#include "shaderHandler.hpp"

#define WIDTH 800
#define HEIGHT 800

int main(){

	// Inits glfw and core profile
	glfwGLInit();

	// Creates window with a width of xxx, a height of xxx, a name of xxx, without fullscreen, and window to share resources with 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpengl", NULL, NULL);

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

	// Creates and VBO, VAO
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);

	// Chooses the VAO as the vertex array to be modified, binds a VBO to the VAO and passes vertices to the VBO
	// GL_STATIC_DRAW means that the data will not be changed, used many times, and will be drawn to the screen
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Chooses the EBO as the element to be modified, and passes in the indices
	// GL_STATIC_DRAW still means the same thing as above
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// Tells the vertex shader how the data from the binded vbo 
	// Tells glsl the location that was told in the .vert, how many numbers per point, the type, 
	// if it is normalized, the amount of bytes until the next vertex, and where the data begins in the vbo 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbinds the objects
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	int imgWidth, imgHeight, colChnl;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgBytes = stbi_load("textures/wall.jpg", &imgWidth, &imgHeight, &colChnl, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// glTextureParameteri() and glTexParameteri() are two different things :( 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgBytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imgBytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0 = glGetUniformLocation(shaderProgram, "tex0");
	glUseProgram(shaderProgram);
	glUniform1i(tex0, 0);
	
	using namespace glm;
	mat4 model = mat4(1.0f);
	mat4 view = mat4(1.0f);
	mat4 proj = mat4(1.0f);
	

	view = translate(view, vec3(0.0f, 0.0f, -3.0f));
	proj = perspective(radians(45.0f), (float)(WIDTH/HEIGHT), 0.1f, 100.0f); 

	// Area OpenGL renders from (0,0) to (width, height)
	glViewport(0, 0, WIDTH, HEIGHT);

	// Specify the color of the background to GL_COLOR_BUFFER_BIT
	glClearColor(0.19f, 0.24f, 0.27, 1.0f);

	glUseProgram(shaderProgram);
	
	// Provides matrices to shader program
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
	GLint projLoc = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(proj));
	
	// Makes depth buffer and allows closer objects be in front (rendered last) of others
	glEnable(GL_DEPTH_TEST);
	
	int frameCount = 0;
	float prevTime = 0;
	float time;
	string title;

	// while window is open 
	while(!glfwWindowShouldClose(window)){
		// Check for events (like resizing)
		glfwPollEvents();
		
		time = glfwGetTime();

		// Every ~100 frames set the title to fps
		if(!(frameCount % 100)){	
			title = "LearnOpengl " + to_string( (int)( 1/(time - prevTime) ) ) + " FPS";
			glfwSetWindowTitle(window, title.c_str());
		}

		model = rotate(model, radians(1.0f), vec3(1.0f, 1.0f, 1.0f));
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

		// Draw GL_COLOR_BUFFER_BIT to the back buffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
		glBindTexture(GL_TEXTURE_2D, texture);

		// Binds the VAO to draw the data
		glBindVertexArray(VAO);

		// Uses the VBO to draw triangles, the offset, and the amount of vertices
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/(sizeof(float)*5));	
		// Swap the drawn back buffer with the front buffer
		glfwSwapBuffers(window);

		prevTime = time;
		frameCount++;
	}

	glDeleteProgram(shaderProgram);
	glDeleteTextures(1, &texture);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	

	// Terminate glfw and the window
	glfwDestroyWindow(window);
	glfwTerminate();

}
