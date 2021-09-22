#pragma once

#include <assimp/Importer.hpp>   // C++ Importer
#include <assimp/scene.h>        // Scene data structure
#include <assimp/postprocess.h>  // Post proccessing flags

#include "mesh.hpp"
#include "textureHandler.hpp"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_JoinIdenticalVertices)
#define TEXTURE_PARAM GL_LINEAR

using namespace glm;

struct Model {
	vector<Mesh> meshes;
	string directory;

	Model(string path){
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, ASSIMP_LOAD_FLAGS);

		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
			cerr << "Error: Assimp: \n" << import.GetErrorString() << endl;
			exit(EXIT_FAILURE);
		}

		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);	
	}

	void Draw(GLuint shader){
		for(long unsigned int i = 0; i < meshes.size(); i++){		
			meshes[i].Draw(shader);
		}
	}

	void processNode(aiNode* node, const aiScene* scene){
		// place each mesh in the mesh vector	
		for(unsigned int i = 0; i < node->mNumMeshes; i++){
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(interpretMesh(mesh, scene));
		}
		
		// do the same for each of its children
		for(unsigned int i = 0; i < node->mNumChildren; i++){
			processNode(node->mChildren[i], scene);
		}

	}
	
	Mesh interpretMesh(aiMesh *mesh, const aiScene *scene){
		vector<Vertex>       vertices;
		vector<unsigned int> indices;
		vector<Texture>      textures;
		
		// Copy the position, normal, and UV of the mesh to verticies vector
		for(unsigned int i = 0; i < mesh->mNumVertices; i++){
			Vertex vertex;

			vertex.Position.x = mesh->mVertices[i].x;
			vertex.Position.y = mesh->mVertices[i].y;
			vertex.Position.z = mesh->mVertices[i].z;

			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;

			if(mesh->mTextureCoords[0]){
				vertex.UV.x = mesh->mTextureCoords[0][i].x;
				vertex.UV.y = mesh->mTextureCoords[0][i].y;
			} else
					vertex.UV = vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		
		// Copy mesh indicies to vector
		for(unsigned int i = 0; i < mesh->mNumFaces; i++){
			aiFace face = mesh->mFaces[i];
			for(unsigned int j = 0; j < face.mNumIndices; j++){
				indices.push_back(face.mIndices[j]);
			}
		}

		if(mesh->mMaterialIndex >= 0){
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			
			vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			
			vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const char* typeName){
			vector<Texture> textures;
			for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
					aiString textureName;
					mat->GetTexture(type, i, &textureName);
					
					string textureLoc = directory + '/' + string(textureName.C_Str()); 
					Texture texture;

					texture.id = setTexture(textureLoc.c_str(), 0, TEXTURE_PARAM);	
					texture.type = typeName;
					texture.path = textureName.C_Str();
					textures.push_back(texture);

			}
			return textures;
	}

	void deleteData(){
		for(unsigned int i = 0; i < meshes.size(); i++){
			glDeleteVertexArrays(1, &meshes[i].VAO);
			glDeleteBuffers(1, &meshes[i].VBO);
			glDeleteBuffers(1, &meshes[i].EBO);
			
			for(unsigned int j = 0; j < meshes[i].textures.size(); j++){
				glDeleteTextures(1, &meshes[i].textures[j].id);
			}	
		}
	}
};
