#pragma once

// std
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

string getShader(string file){
	ifstream fileStream(file);

	stringstream fileSStream;
	fileSStream << fileStream.rdbuf();
	fileStream.close();

	return fileSStream.str();
}

void compileShader(GLuint shader, const GLchar** source){
	glShaderSource(shader, 1, source, NULL);
	glCompileShader(shader);
}

GLuint makeShader(const char* vertexShaderFile, const char* fragmentShaderFile){
	string vshaderInput = getShader(vertexShaderFile);
	const char* vertexShaderSource = vshaderInput.c_str();
	string fshaderInput = getShader(fragmentShaderFile);
	const char* fragmentShaderSource = fshaderInput.c_str();

	// Creates vertex shader, links source code to shader, and compiles it
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShader, &vertexShaderSource);
	
	// Simliar to above but it creates a fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShader, &fragmentShaderSource);

	// Creates shader program, attaches shaders, and connects the io of shaders 	
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	// Deletes shaders as they are already in the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
