#pragma once

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
