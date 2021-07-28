#pragma once

// graphics
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>

void glfwGLInit(){
	// Initizalizes glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tells glfw to use a profile with modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
