#pragma once

// std
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

string getShader(string file){
	ifstream fileStream;
	fileStream.open(file);

	std::stringstream fileSStream;
	fileSStream << fileStream.rdbuf();
	fileStream.close();

	return fileSStream.str();
}

void compileShader(GLuint shader, const GLchar** source){
	glShaderSource(shader, 1, source, NULL);
	glCompileShader(shader);
}
