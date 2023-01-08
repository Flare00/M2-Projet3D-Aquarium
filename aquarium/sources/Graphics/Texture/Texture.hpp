#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#define STB_IMAGE_IMPLEMENTATION

#include <GL/gl.h>
#include <string>
#include <Engine/Tools/Tools.hpp>
#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <iostream>

/// <summary>
/// A Texture Object for GPU rendering.
/// </summary>
class Texture {
public:
protected:
	std::string path;
	glm::vec4 fallback;
	int width, height, nbChannel;
	int fallbackChannel;
	GLuint texture_index = -1;
	unsigned char* texture_data;


	bool loaded;
	bool isFallBack = true;
public:

	/// <summary>
	/// Generate a Texture
	/// </summary>
	/// <param name="path">The path of the texture</param>
	/// <param name="fallback">The color fallback, if the texture does not exist</param>
	/// <param name="fallbackChannel">number of channel for the callback</param>
	Texture(std::string path = "", glm::vec4 fallback = glm::vec4(1.0), int fallbackChannel = 4)
	{
		this->path = path;
		this->fallback = fallback;
		this->fallbackChannel = fallbackChannel;
		this->LoadData();
	}

	/// <summary>
	/// Generate a fallback texture.
	/// </summary>
	/// <param name="fallback">The color of the texture.</param>
	Texture(glm::vec4 fallback)
	{
		this->path = "";
		this->fallback = fallback;
		this->LoadData();
	}

	/// <summary>
	/// Destroy the texture, and remove the data from GPU.
	/// </summary>
	~Texture() {
		glDeleteTextures(0, &texture_index);
	}

	/// <summary>
	/// Set the path of the texture, and reload.
	/// </summary>
	/// <param name="path"></param>
	void SetPath(std::string path) {
		if (this->path.compare(path) != 0) {
			this->path = path;
			LoadData();
		}
	}

	/// <summary>
	/// Change the fallback value, and reload.
	/// </summary>
	/// <param name="fallback"></param>
	void SetFallback(glm::vec4 fallback) {
		if (this->fallback != fallback) {
			this->fallback = fallback;
			LoadData();
		}
	}

	/// <summary>
	/// Generate the texture to the GPU.
	/// </summary>
	void GenerateTexture() {
		if (this->texture_index == -1) {
			glGenTextures(1, &this->texture_index);
		}
		if (this->texture_data) {
			GLenum format = GL_RGBA;
			if(this->nbChannel == 1){
				format = GL_RED;
			} else if (this->nbChannel == 3){
				format = GL_RGB;
			}
			glBindTexture(GL_TEXTURE_2D, this->texture_index);
			glTexImage2D(GL_TEXTURE_2D, 0, format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, this->texture_data);
			
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			this->loaded = true;
			if (this->path.size() > 0) {
				stbi_image_free(this->texture_data);
			}
			else {
				delete this->texture_data;
			}
		}
		else {
			this->loaded = false;
		}
	}

	/// <summary>
	/// Try to load texture file, if not possible, generate a texture with the fallback color. then Load the data to the GPU
	/// </summary>
	void LoadData() {
		if (this->path.size() > 0) {
			int nbC;
			this->texture_data = stbi_load(path.c_str(), &this->width, &this->height, &this->nbChannel, 0);
			if(this->texture_data == nullptr){
				printf("Failed to load texture : %s\n Generate Fallback.\n", this->path.c_str());
				GenerateColorData(this->fallback);
			}
			else {
				isFallBack = false;
			}
			
		}
		else {
			GenerateColorData(this->fallback);
		}

		this->GenerateTexture();
	}

	/// <summary>
	/// Generate a texture data 1 by 1 with the given color;
	/// </summary>
	/// <param name="color">The color.</param>
	void GenerateColorData(glm::vec4 color) {
		this->width = 1;
		this->height = 1;
		this->nbChannel = this->fallbackChannel;

		this->texture_data = new unsigned char[this->nbChannel];
		for (int i = 0; i < this->nbChannel; i++) {
			this->texture_data[i] = (int)(color[i] * 0xff);
		}

	}

	/// <summary>
	/// Return the texture GPU index.
	/// </summary>
	/// <returns>The texture GPU index.</returns>
	GLuint GetIndex() {
		return this->texture_index;
	}

	/// <summary>
	/// Bind the texture to the GPU.
	/// </summary>
	void Bind() {
		glBindTexture(GL_TEXTURE_2D, this->texture_index);
	}

	/// <summary>
	/// Get if it's fallback.
	/// </summary>
	/// <returns>It is fallback ?</returns>
	bool IsFallback() {
		return this->isFallBack;
	}
};

#endif