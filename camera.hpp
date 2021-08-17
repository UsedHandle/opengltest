#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace glm;

struct Camera {
	vec3 position;
	vec3 orientation;
	vec3 up;

	mat4 view = mat4(1.0f);
	mat4 proj = mat4(1.0f);

	float speed = 0.1;
	bool prevEscPress = true;
	bool escPress = false;

	bool isEsc = false;

	Camera(int width, int height, float FOV, vec3 position, vec3 orientation, vec3 up){
		this->position = position;
		this->orientation = orientation;
		this->up = up;
		
		this->view = translate(this->view, position);	
		proj = perspective(radians(FOV), (float)(width/height), 0.1f, 100.0f); 
	}

	void setUniform(GLuint shaderProgram, const char* uniform){		
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, uniform), 1, GL_FALSE, value_ptr(proj * view));		
	}

	void moveAndLook(GLFWwindow* window, GLuint shaderProgram, const char* uniform, float deltaTime, int fps){
		// Keyboard
		vec3 target = vec3(0.0f);
		if(glfwGetKey(window, GLFW_KEY_W)){
			target += orientation;
		} 
		if(glfwGetKey(window, GLFW_KEY_S)){
			target -= orientation;
		}
		if(glfwGetKey(window, GLFW_KEY_D)){
			target += cross(orientation, up);
		}
		if(glfwGetKey(window, GLFW_KEY_A)){
			target -= cross(orientation, up);
		}
		
		// normalized movement as if running x fps 
		if(length(target)) target = fps*deltaTime * speed*normalize(target); 
		position += target;	
		if(length(target)) cout << length(target)<< endl;

		// Escape	
		escPress = glfwGetKey(window, GLFW_KEY_ESCAPE);
		if(prevEscPress && !escPress){	
			isEsc ? 
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL): 
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
 
			isEsc = !isEsc;
		}	
		prevEscPress = escPress;

		view = lookAt(position, position+orientation, up);
		setUniform(shaderProgram, uniform);
	}	
};
