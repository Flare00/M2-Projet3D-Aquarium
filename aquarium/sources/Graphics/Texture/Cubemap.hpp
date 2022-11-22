#ifndef __CUBE_MAP_HPP__
#define __CUBE_MAP_HPP__
#define STB_IMAGE_IMPLEMENTATION
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <vector>
#include <string>
#include <stb_image.h>

class Cubemap {
public :
	enum Mode {
		DEPTH,
		RGBA
	};

protected:
	Mode mode;
	GLuint texture_index = -1;
	std::vector<std::string> faces;
	int width, height;
public:
	Cubemap(size_t depth_width, size_t depth_height) {
		this->mode = Mode::DEPTH;
		this->width = depth_width;
		this->height = depth_height;
	}
	Cubemap(std::vector<std::string> faces) {
		this->mode = Mode::RGBA;
		this->faces = faces;
	}

	Generate() {
		glGenTextures(1, &this->texture_index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture_index);

		if (mode == Mode::DEPTH) {
			for (size_t i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			}
		}
		else if (mode == Mode::RGBA) {
			for (size_t i = 0, max = faces.size(); i < max; i++) {
				int width, height;
				unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
				if (data) {
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data)
				}
				stbi_image_free(data);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	GLuint GetIndex() {
		return this->texture_index;
	}

	void Bind() {
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture_index);
	}
};

#endif
