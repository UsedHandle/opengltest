#pragma once

// std
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

string getFile(const char* fileLoc){
	ifstream fileStream(fileLoc);
	if(!fileStream){ 
		cerr << fileLoc << " not found\n"; 
		exit(EXIT_FAILURE); 
	}

	stringstream fileSStream;
	fileSStream << fileStream.rdbuf();
	fileStream.close();

	return fileSStream.str();
}

void compileShader(GLuint shader, const GLchar** source){
	glShaderSource(shader, 1, source, NULL);
	glCompileShader(shader);
}

void debugShader(GLuint shader, const char* fileLoc){	
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << fileLoc << "\n" << infoLog << "\n\n";
	}
}

void debugShaderProgram(GLuint shaderProgram){
	int  success;
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			cout << "Shader Program Failed to link/create" << endl;
			exit(EXIT_FAILURE);
	}
}

GLuint makeShaderProgram(const char* vertexShaderLoc, const char* fragmentShaderLoc){
	string vshaderInput = getFile(vertexShaderLoc);
	const char* vertexShaderSource = vshaderInput.c_str();
	string fshaderInput = getFile(fragmentShaderLoc);
	const char* fragmentShaderSource = fshaderInput.c_str();

	// Creates vertex shader, links source code to shader, and compiles it
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShader, &vertexShaderSource);
	debugShader(vertexShader, vertexShaderLoc);

	// Simliar to above but it creates a fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShader, &fragmentShaderSource);
	debugShader(fragmentShader, fragmentShaderLoc);

	// Creates shader program, attaches shaders, and connects the io of shaders 	
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	debugShaderProgram(shaderProgram);

	// Deletes shaders as they are already in the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
