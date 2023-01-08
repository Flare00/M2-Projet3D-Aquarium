#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

/// <summary>
/// Framebuffer object, with framebuffer and renderbuffer. Permit to do more advanced Graphical operations.
/// </summary>
class Framebuffer
{
protected:
	GLuint framebuffer;
	GLuint renderbuffer;
	GLuint tex_color;
	GLuint tex_depth;

	int w = -1, h = -1;
	bool floating = false;

	/// <summary>
	/// Generate Framebuffer textures.
	/// </summary>
	void GenTextures(){
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

		// create a depth attachment texture
		glGenTextures(1, &this->tex_depth);
		glBindTexture(GL_TEXTURE_2D, this->tex_depth);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, this->w, this->h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY); // GL_INTENSITY16 ?
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
public:
	/// <summary>
	/// Create a framebuffer.
	/// </summary>
	Framebuffer() {

	}

	/// <summary>
	/// Destroy the framebuffer, and free the GPU data.
	/// </summary>
	~Framebuffer() {
		glDeleteRenderbuffers(1, &this->renderbuffer);
		glDeleteFramebuffers(1, &this->framebuffer);
		glDeleteTextures(1, &this->tex_color);
	}

	/// <summary>
	/// Generate the framebuffer.
	/// </summary>
	/// <param name="width">The width (-1 = screen width)</param>
	/// <param name="height">The height (-1 = screen height)</param>
	/// <param name="floating">Is floating point ?</param>
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
		

		GenTextures();

		glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->tex_color, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->tex_depth, 0);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->tex_stencil, 0);

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

	/// <summary>
	/// Return the color texture.
	/// </summary>
	/// <returns>The color texture</returns>
	GLuint GetTexColor() {
		return this->tex_color;
	}


	/// <summary>
	/// Return the depth texture.
	/// </summary>
	/// <returns>The depth texture</returns>
	GLuint GetTexDepth() {
		return this->tex_depth;
	}

	/*GLuint GetTexStencil() {
		return this->tex_stencil;
	}*/

	/// <summary>
	/// Return the framebuffer GPU location
	/// </summary>
	/// <returns>the framebuffer GPU location</returns>
	GLuint GetFramebuffer() {
		return this->framebuffer;
	}

	/// <summary>
	/// Return the renderbuffer GPU location
	/// </summary>
	/// <returns>the renderbuffer GPU location</returns>
	GLuint GetRenderbuffer() {
		return this->renderbuffer;
	}

	/// <summary>
	/// Write the texture to a File
	/// </summary>
	/// <param name="name">The name of the file</param>
	/// <param name="tex">The texture to draw (0 = Color, 1 = Depth)</param>
	void WriteTextureToFile(std::string name, int tex = 0) { //Tex : 0 = Color, 1 = Depth
		if (w <= 0 || h <= 0)
			return;
		
		glBindTexture(GL_TEXTURE_2D, tex == 0 ? this->tex_color : this->tex_depth);

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

	/// <summary>
	/// Return the width
	/// </summary>
	/// <returns>the width</returns>
	int GetWidth() {
		return this->w;
	}

	/// <summary>
	/// return the height
	/// </summary>
	/// <returns>the height</returns>
	int GetHeight() {
		return this->h;
	}
};




#endif