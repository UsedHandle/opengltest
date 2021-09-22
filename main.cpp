#include "glsetup.hpp"
#include "coordinates.hpp"
#include "shaderHandler.hpp"
#include "textureHandler.hpp"
#include "camera.hpp"
#include "model.hpp"

// In glsetup.hpp
// static int width = 800;
// static int height = 800;

static const char* title = "LearnOpengl";

static int targetFps = 75;

int main(){

	GLFWwindow* window = makeWindowEnv(width, height, title);
	
	GLuint shaderProgram = makeShaderProgram("shaders/shader.vert", "shaders/shader.frag");
	glUseProgram(shaderProgram);

	Model model("models/cyborg/cyborg.obj");
	glClearColor(0.19f, 0.24f, 0.27, 1.0f); // Specify GL_COLOR_BUFFER_BIT

	string title;	
	int frameCount = 0;
	float prevTime = 0;
	float time;
	float deltaTime;
	float fps;

	Camera camera(width,                   // window width
		            height,                  // window height 
		            50.0f,                   // FOV (degrees)
		            vec3(0.0f, 0.0f, 2.0f),  // position
		            vec3(0.0f, 0.0f, -1.0f), // orientation
		            vec3(0.0f, 1.0f, 0.0f)); // up

	
	while(!glfwWindowShouldClose(window)){	
		glfwPollEvents(); // Check for events (like resizing)
		
		time = glfwGetTime();
		deltaTime = targetFps * (time - prevTime);
		fps = targetFps/deltaTime;
		
		camera.moveAndLook(window, shaderProgram, "camMat", deltaTime, targetFps); 

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
		model.Draw(shaderProgram);
	
		glfwSwapBuffers(window);

		frameCount++;
		prevTime = time;
	}

	glDeleteProgram(shaderProgram);

	model.deleteData(); // Delete textures, VAOs, VBOs, and EBOs

	// Terminate glfw and the window
	glfwDestroyWindow(window);
	glfwTerminate();
}
