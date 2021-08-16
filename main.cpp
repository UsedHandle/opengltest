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
#include "glsetup.hpp"
#include "coordinates.hpp"
#include "shaderHandler.hpp"
#include "camera.hpp"

unsigned int width = 800;
unsigned int height = 800;

int main(){

	// Inits glfw and core profile
	glfwGLsetup();

	// Anti-Aliasing (MSAA)
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Creates window with a width of xxx, a height of xxx, a name of xxx, without fullscreen, and window to share resources with 
	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpengl", NULL, NULL);

	if(window == NULL){
		glfwTerminate();		
		cout << "Failed to create a window with GLFW" << std::endl;

		return -1;
	}
	
	// Whenever the window size changes, reset the viewport to the window size
	glfwSetFramebufferSizeCallback(window, fbSizeCallback);

	// Tells glfw that the window will be used
	glfwMakeContextCurrent(window);

	// Loads configurations for OpenGL
	gladLoadGL();
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		cout << "Failed to initialize GLAD" << std::endl;

		return -1;
	}
	
	GLuint shaderProgram = makeShaderProgram("shaders/shader.vert", "shaders/shader.frag");

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Chooses the VAO as the vertex array to be modified, binds a VBO to the VAO and passes vertices to the VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// GL_STATIC_DRAW means that the data will not be changed, used many times, and will be drawn to the screen
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Tells the vertex shader how the data from the binded vbo 
	// Tells glsl the location that was told in the .vert, how many numbers per point, the type, 
	// if it is normalized, the amount of bytes until the next vertex, and where the data begins in the vbo 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	int imgWidth, imgHeight, colChnl;
	// stbi reads images differently than how opengl reads
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

	GLuint tex0 = glGetUniformLocation(shaderProgram, "tex0");
	glUseProgram(shaderProgram);
	glUniform1i(tex0, 0);
	
	using namespace glm;
	mat4 model = mat4(1.0f);
	mat4 view = mat4(1.0f);
	mat4 proj = mat4(1.0f);
	

	view = translate(view, vec3(0.0f, 0.0f, -3.0f));
	// FOV, aspect ratio, nearest object you can see, farthest object you can see
	proj = perspective(radians(45.0f), (float)(width/height), 0.1f, 100.0f); 

	// Area OpenGL renders from (0,0) to (width, height)
	glViewport(0, 0, width, height);

	// Specify the color of the background to GL_COLOR_BUFFER_BIT
	glClearColor(0.19f, 0.24f, 0.27, 1.0f);

	glUseProgram(shaderProgram);
	
	Camera camera(width, height, 45.0f, vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));

	camera.setUniform(shaderProgram, "camera");

	// Provides matrices to shader program
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		
	// Makes depth buffer and allows closer objects be in front (rendered last) of others
	// Enables AA with multisampling (MSAA) if not defaulted to
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);	

	
	int frameCount = 0;

	string title;
	
	float prevTime = 0;
	float time;
	float deltaTime;
	
	while(!glfwWindowShouldClose(window)){
		// Check for events (like resizing)
		glfwPollEvents();
		
		time = glfwGetTime();
		deltaTime = time - prevTime;

		// Every ~100 frames set the title to fps
		if(!(frameCount % 100)){	
			title = "LearnOpengl " + to_string( (int)( 1/deltaTime ) ) + " FPS";
			glfwSetWindowTitle(window, title.c_str());
		}
		
		// Rotates as if rotating 1 degree 75 fps
		model = rotate(model, radians(75.0f * deltaTime), vec3(1.0f, 1.0f, 1.0f));
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));	
		
		camera.moveAndLook(window, shaderProgram, "camera", deltaTime, 75); 

		// Draw GL_COLOR_BUFFER_BIT to the back buffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
		glBindTexture(GL_TEXTURE_2D, texture);

		// Binds the VAO to draw the data
		glBindVertexArray(VAO);

		// Uses the VBO to draw triangles, the offset, and the amount of vertices
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/(sizeof(float)*5));	
		// Swap the drawn back buffer with the front buffer
		glfwSwapBuffers(window);

		frameCount++;
		prevTime = time;
	}

	glDeleteProgram(shaderProgram);
	glDeleteTextures(1, &texture);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	

	// Terminate glfw and the window
	glfwDestroyWindow(window);
	glfwTerminate();

}
