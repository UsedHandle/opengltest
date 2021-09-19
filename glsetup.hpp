#pragma once

// Window Events and Graphics
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>

using namespace std;

void glfwFbSizeCallback(GLFWwindow* window, int fbWidth, int fbHeight){
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, fbWidth, fbHeight);
}

void glfwErrorCallback(int error, const char* description){
	printf("GLFW Error (%d): \n%s \n", error, description);
	exit(EXIT_FAILURE);
}

void glfwGLsetup(){	
	glfwSetErrorCallback(glfwErrorCallback);
	glfwInit();
	
	// Opengl version 3.3 with modern functions
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_SAMPLES, 4); // Anti-aliasing (MSAA)
}

GLFWwindow* makeWindowEnv(int width, int height, const char* title){
	glfwGLsetup();

	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL); // width, height, monitor, child/parent window 
	if(!window){
		glfwTerminate();		
		cerr << "Failed to create a window with GLFW" << endl;
		exit(EXIT_FAILURE);
	}
	
	glfwSetFramebufferSizeCallback(window, glfwFbSizeCallback);
	glfwMakeContextCurrent(window); // Loads Opengl context

	gladLoadGL();
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		cerr << "Failed to initialize GLAD" << endl;
		exit(EXIT_FAILURE);	
	}

	glEnable(GL_DEPTH_TEST); // Depth Testing
	glEnable(GL_MULTISAMPLE);	// Anti-Aliasing (MSAA)

	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);
	
	return window;
}
