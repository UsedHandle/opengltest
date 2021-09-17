#pragma once

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // image loading

GLuint makeTexture( int textureUnit, GLint texParam, GLenum format, unsigned char* bytes, int imgWidth, int imgHeight){

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParam);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}

GLuint setTexture( const char* file, int textureUnit, GLint texParam, GLuint shaderProgram, const char* uniform ){	
	stbi_set_flip_vertically_on_load(true);
	int imgWidth, imgHeight, colChnl;
	unsigned char* imgBytes = stbi_load(file, &imgWidth, &imgHeight, &colChnl, 0);
	
	GLenum format;

	if(colChnl == 1)
		format = GL_RED;	
	else if(colChnl == 3)
		format = GL_RGB;	
	else if(colChnl == 4)
		format = GL_RGBA;
	else {
		cerr << "Unknown format:" << file << endl; 
		exit(EXIT_FAILURE);
	}

  if(imgBytes){
    GLuint texture = makeTexture( textureUnit, texParam, format, imgBytes, imgWidth, imgHeight );
    stbi_image_free(imgBytes);
    glUniform1i(glGetUniformLocation(shaderProgram, uniform), textureUnit);

  	return texture;

	} else {
    cerr << "Failed to find and read texture: " << file << endl;
    exit(EXIT_FAILURE);
  }

}
