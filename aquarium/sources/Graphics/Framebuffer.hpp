#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

#include <glm/glm.hpp>
#include <GL/glut.h>

class Framebuffer
{
protected:
    GLuint framebuffer;
    GLuint renderbuffer;
    GLuint tex_color;
public:
    Framebuffer(){
    }

    ~Framebuffer(){
		glDeleteRenderbuffers(1, &this->renderbuffer);
		glDeleteFramebuffers(1, &this->framebuffer);
        glDeleteTextures(1, &this->tex_color);
    }

    void Generate(){
        //create the framebuffer
		glGenFramebuffers(1, &this->framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

		// create a color attachment texture
		glGenTextures(1, &this->tex_color);
		glBindTexture(GL_TEXTURE_2D, this->tex_color);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, global.screen_width, global.screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->tex_color, 0);

		//create the renderbuffer
		glGenRenderbuffers(1, &this->renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, this->renderbuffer);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, global.screen_width, global.screen_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->renderbuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint GetTexColor(){
        return this->tex_color;
    }

    GLuint GetFramebuffer(){
        return this->framebuffer;
    }

    GLuint GetRenderbuffer(){
        return this->renderbuffer;
    }
};




#endif