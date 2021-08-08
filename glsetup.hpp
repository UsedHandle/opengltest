#pragma once

// graphics
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>

void fbSizeCallback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void glfwGLsetup(){
	// Initizalizes glfw
	glfwInit();
	// Tells glfw to use opengl 3.3 with modern functions
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
    	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
}
