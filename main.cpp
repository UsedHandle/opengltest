// Custom headers
#include "glsetup.hpp"
#include "coordinates.hpp"
#include "shaderHandler.hpp"
#include "textureHandler.hpp"
#include "camera.hpp"

unsigned int width = 800;
unsigned int height = 800;

int main(){

	GLFWwindow* window = makeWindowEnv(width, height, "LearnOpengl");
	
	GLuint shaderProgram = makeShaderProgram("shaders/shader.vert", "shaders/shader.frag");
	glUseProgram(shaderProgram);

	GLuint texture = setTexture("textures/wall.jpg", 0, GL_NEAREST, shaderProgram, "tex0");	
	
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Data is static

	// shader location, numbers per point, type, normalizing, bytes between each attribute, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);


	glViewport(0, 0, width, height);

	glClearColor(0.19f, 0.24f, 0.27, 1.0f); // Specify to GL_COLOR_BUFFER_BIT


	string title;
	
	int frameCount = 0;
	float prevTime = 0;
	float time;
	float deltaTime;
	
	// widow width, height
	Camera camera(
		width,                   // widow width 
		height,                  // window height
		45.0f,                   // FOV
		vec3(0.0f, 0.0f, 2.0f),  // position
		vec3(0.0f, 0.0f, -1.0f), // orientation
		vec3(0.0f, 1.0f, 0.0f)); // up

	while(!glfwWindowShouldClose(window)){	
		glfwPollEvents(); // Check for events (like resizing)
		
		time = glfwGetTime();
		deltaTime = time - prevTime;

		// Every ~100 frames set the title to fps
		if(!(frameCount % 100)){	
			title = "LearnOpengl " + to_string( (int)( 1/deltaTime ) ) + " FPS";
			glfwSetWindowTitle(window, title.c_str());
		}
			
		camera.moveAndLook(window, shaderProgram, "camMat", deltaTime, 75); 

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
		glBindVertexArray(VAO);
		glDrawArrays( GL_TRIANGLES, 0, sizeof(vertices)/(sizeof(float)*5) ); // Type of vertex, offset, count	
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
