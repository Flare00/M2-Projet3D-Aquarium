#ifndef __TEXTURE_FLOATING_POINT_HPP__
#define __TEXTURE_FLOATING_POINT_HPP__
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class TextureFloatingPoint {
public:
	enum Precision {
		F32,
		F16
	};
protected:
	int width, height, nbChannel;
	GLuint texture_index = -1;
	Precision precision;
public:
	TextureFloatingPoint(int width = 512, int height = 512, int nbChannel = 1, Precision precision = F16) {
		this->width = size;
		this->height = size;
		this->nbChannel = nbChannel < 1 ? 1 : (nbChannel > 4 ? 4 : nbChannel);
		this->precision = precision;
		GenerateTexture();
	}

	void GetInternalAndNormalFormat(GLenum* internalFormat, GLenum* format) {
		if (this->nbChannel == 1) {
			internalFormat = (this->precision == F16 ? GL_R16F : GL_R32F);
			format = GL_RED;
		}
		else if (this->nbChannel == 2) {
			internalFormat = (this->precision == F16 ? GL_RG16F : GL_RG32F);
			format = GL_RG;
		}
		else if (this->nbChannel == 3) {
			internalFormat = (this->precision == F16 ? GL_RGB16F : GL_RGB32F);
			format = GL_RGB;
		}
		else if (this->nbChannel == 4) {
			internalFormat = (this->precision == F16 ? GL_RGBA16F : GL_RGBA32F);
			format = GL_RGBA;
		}
	}

	void GenerateTexture() {
		if (this->texture_index == -1) {
			glGenTextures(1, &this->texture_index);
		}

		float data[height][width][nbChannel];

		for (int i = 0; i < this->height; i++) {
			for (int j = 0; j < this->height; j++) {
				for (int k = 0; k < this->nbChannel; k++) {
					data[i][j][k] = 0.0f;
				}
			}
		}

		GLenum i_format = GL_RGBA16F;
		GLenum format = GL_RGBA;
		GetInternalAndNormalFormat(&i_format, &format);

		glBindTexture(GL_TEXTURE_2D, this->texture_index);
		glTexImage2D(GL_TEXTURE_2D, 0, i_format, this->width, this->height, 0, format, GL_FLOAT, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	GLuint GetIndex() {
		return this->texture_index;
	}

	void Bind() {
		glBindTexture(GL_TEXTURE_2D, this->texture_index);
	}
};

#endif // !__TEXTURE_FLOATING_POINT_HPP__
