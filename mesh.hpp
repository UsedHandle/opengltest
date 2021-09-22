#pragma once

#include <vector>

using namespace std;
using namespace glm;

struct Vertex {
	vec3 Position;
	vec3 Normal;
	vec2 UV;
};

struct Texture {
	GLuint id;
	const char* type;
	const char* path;
};

struct Mesh {
	vector<Vertex>       vertices;
	vector<unsigned int> indices;
	vector<Texture>      textures;

	GLuint VAO, VBO, EBO;
	
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures){
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		
		setVAO();
	}
	void Draw(GLuint shaderProgram){
		unsigned int diffuseCount = 1;
		unsigned int specularCount = 1;

		for(unsigned int i = 0; i < textures.size(); i++){
			glActiveTexture(GL_TEXTURE0 + i);

			string number;
			string name = textures[i].type;
			if(name == "texture_diffuse")
				number =  to_string(diffuseCount++);
			else if(name == "texture_specular")
				number = to_string(specularCount++);
			
			glUniform1i(glGetUniformLocation(shaderProgram, (name + number).c_str()), i);			
			glBindTexture(GL_TEXTURE_2D, textures[i].id);

		}
		
		glActiveTexture(GL_TEXTURE0);
	
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // format, size, type, offset
		glBindVertexArray(0);
	}

	void setVAO(){
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);


		glBindVertexArray(VAO);
		
		// In a struct, its member variables are stored consecutively in memory
		// This property is used here

		// put the vertices vector in the VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
		// put the indices vector in the EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// Give positions at location 0
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); 
		
		// Give Normals at location 1
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		
		// Give UVs at location 2
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
		
		glBindVertexArray(0);
	}
};
