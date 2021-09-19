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
	int width, height;

	vec3 position;
	vec3 orientation;
	vec3 up;
	mat4 view = mat4(1.0f);
	mat4 proj = mat4(1.0f);


	float sensitivity = 50.0f;
	bool isEsc = true;
	double xPos, yPos;
	double last_xPos = 0; 
	double last_yPos = 0;
	float xRot, yRot;
	
	float speed = 0.05;

	Camera(int width, int height, float FOV, vec3 position, vec3 orientation, vec3 up){
		this->position = position;
		this->orientation = orientation;
		this->up = up;

		this->width = width;
		this->height = height;

		this->last_xPos = width/2;
		this->last_yPos = height/2;
		
		this->view = translate(this->view, position);	
		this->proj = perspective(radians(FOV), (float)(width/height), 0.1f, 100.0f); 
	}

	void setUniform(GLuint shaderProgram, const char* uniform){		
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, uniform), 1, GL_FALSE, value_ptr(proj * view));		
	}

	void moveAndLook(GLFWwindow* window, GLuint shaderProgram, const char* uniform, float deltaTime, int targetFps){	
		// Keyboard
		vec3 target = vec3(0.0f);
		if(glfwGetKey(window, GLFW_KEY_W))
			target += orientation;

		if(glfwGetKey(window, GLFW_KEY_S))
			target -= orientation;
		
		if(glfwGetKey(window, GLFW_KEY_D))
			target += cross(orientation, up);
		
		if(glfwGetKey(window, GLFW_KEY_A))
			target -= cross(orientation, up);
	
		// normalized movement as if running x fps 
		if(length(target)) target = deltaTime * speed*normalize(target); 
		
		position += target;	

		// Arrow camera rotation
		if(glfwGetKey(window, GLFW_KEY_LEFT))
			orientation = rotate(orientation, radians(deltaTime * sensitivity * 10 / width), up);
		
		if(glfwGetKey(window, GLFW_KEY_RIGHT))
			orientation = rotate(orientation, radians(deltaTime * sensitivity * -10 / width), up);
		
		if(glfwGetKey(window, GLFW_KEY_UP)){
			vec3 yRotOrientation = rotate(orientation, radians(deltaTime * sensitivity * 10 / height), cross(orientation, up));	
			if(abs(angle(yRotOrientation, up) - radians(90.0f)) <= radians(85.0f)) // Prevents oddities from gimbal lock
				orientation = yRotOrientation;
		}	
		if(glfwGetKey(window, GLFW_KEY_DOWN)){
			vec3 yRotOrientation = rotate(orientation, radians(deltaTime * sensitivity * -10 / height), cross(orientation, up));
			if(abs(angle(yRotOrientation, up) - radians(90.0f)) <= radians(85.0f))
				orientation = yRotOrientation;
	
		}

		// Cursor	
		if(glfwGetKey(window, GLFW_KEY_ESCAPE)){	
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			isEsc = true;
		}
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && isEsc){
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	
			glfwGetCursorPos(window, &last_xPos, &last_yPos);
			isEsc = false;	
		}
		
		// Mouse
		if(!isEsc){
				glfwGetCursorPos(window, &xPos, &yPos);
				double xOffset = xPos - last_xPos;
				double yOffset = yPos - last_yPos;

				xRot =  -deltaTime * sensitivity * (xOffset) / width;
				yRot =  -deltaTime * sensitivity * (yOffset) / height;

				orientation = rotate(orientation, radians(xRot), up);	
				vec3 yRotOrientation = rotate(orientation, radians(yRot), cross(orientation, up));
				
				// If the angle between up and the rotated orientation is less than 175 degrees proceed
				// This prevents oddities from gimbal lock
				if(abs(angle(yRotOrientation, up) - radians(90.0f)) <= radians(85.0f))
					orientation = yRotOrientation;
				
				last_xPos = xPos;
				last_yPos = yPos;
		}
	
		view = lookAt(position, position+orientation, up);
		setUniform(shaderProgram, uniform);
	
	}	
};
