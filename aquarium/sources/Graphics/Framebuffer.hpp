#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

class Framebuffer
{
protected:
	GLuint framebuffer;
	GLuint renderbuffer;
	GLuint tex_color;

	int w = -1, h = -1;
	bool floating = false;
public:
	Framebuffer() {

	}

	~Framebuffer() {
		glDeleteRenderbuffers(1, &this->renderbuffer);
		glDeleteFramebuffers(1, &this->framebuffer);
		glDeleteTextures(1, &this->tex_color);
	}

	void Generate(int width = -1, int height = -1, bool floating = false) {
		//create the framebuffer
		glGenFramebuffers(1, &this->framebuffer);

		this->floating = floating;
		this->w = width;
		this->h = height;

		if (this->w < 0) {
			this->w = global.screen_width;
		}
		if (this->h < 0) {
			this->h = global.screen_height;
		}
		// create a color attachment texture
		glGenTextures(1, &this->tex_color);
		glBindTexture(GL_TEXTURE_2D, this->tex_color);

		if (floating) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->w, this->h, 0, GL_RGBA, GL_FLOAT, NULL);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->w, this->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->tex_color, 0);

		//create the renderbuffer
		glGenRenderbuffers(1, &this->renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, this->renderbuffer);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->w, this->h);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->renderbuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint GetTexColor() {
		return this->tex_color;
	}

	GLuint GetFramebuffer() {
		return this->framebuffer;
	}

	GLuint GetRenderbuffer() {
		return this->renderbuffer;
	}

	void WriteTextureToFile(std::string name) {
		if (w <= 0 || h <= 0)
			return;

		glBindTexture(GL_TEXTURE_2D, this->tex_color);

		if (floating) {
			GLsizei stride = 4 * this->w;
			std::vector<GLfloat> datas(stride*this->h);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, datas.data());
			std::vector <GLchar> datasChar;
			for (int i = 0, max = datas.size(); i < max; i++) {
				datasChar.push_back((char)(datas[i] * 255.0));
			}
			stbi_write_png(name.c_str(), w, h, 4, datasChar.data(), stride);
		}
		else {
			GLsizei stride = 4 * this->w;

			GLchar* datas = new GLchar[stride * h];
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, datas);


			stbi_write_png(name.c_str(), w, h, 4, datas, stride);
		}
	}

	int GetWidth() {
		return this->w;
	}

	int GetHeight() {
		return this->h;
	}
};




#endif