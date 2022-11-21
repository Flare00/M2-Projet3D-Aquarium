#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#define STB_IMAGE_IMPLEMENTATION

#include <GL/gl.h>
#include <string>
#include <Engine/Tools.hpp>
#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <iostream>


class Texture{
protected :
    std::string path;
    glm::vec4 fallback;
    int width, height;

	GLuint texture_index = -1;
	unsigned char *texture_data;

    bool loaded;

public :

    Texture(std::string path = "", glm::vec4 fallback = glm::vec4(1.0))
	{
        this->path = path;
        this->fallback = fallback;
        this->LoadData();
    }
    Texture(glm::vec4 fallback)
	{
        this->path = "";
        this->fallback = fallback;
        this->LoadData();
    }

    void SetPath(std::string path){
        if(this->path.compare(path) != 0){
            this->path = path;
            LoadData();
        }
    }

    void SetFallback(glm::vec4 fallback){
        if(this->fallback != fallback){
            this->fallback = fallback;
            LoadData();
        }
    }

    //Load the texture data to the GPU
    void GenerateTexture(){
        if(this->texture_index == -1){
            glGenTextures(1, &this->texture_index);
        }
        if(this->texture_data){
            glBindTexture(GL_TEXTURE_2D, this->texture_index);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->texture_data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            this->loaded = true;
        }   else {
            this->loaded = false;
        }
    }

    //Try to load texture file, if not possible, generate a texture with the fallback color. then Load the data to the GPU
    void LoadData(){
        if(this->path.size() > 0){
            int nbC;
		    this->texture_data = stbi_load(path.c_str(), &this->width, &this->height, &nbC, 4);
            /*int max = sizeof(this->texture_data);
            std::cout << "Max : " << max << std::endl;
            for(int i = 0; i < max; i++){
                printf("%d, %02x\n", i, this->texture_data[i]);
            }*/
        } else {
            GenerateColorData(this->fallback);
        }

        this->GenerateTexture();
    }

    //Generate a texture data 1 by 1 with the given color;
    void GenerateColorData(glm::vec4 color){
        this->width = 1;
        this->height = 1;
        this->texture_data = new unsigned char[4];
        for(int i = 0; i < 4; i++){
            this->texture_data[i] = (int)(color[i]*0xff);
        }
    }

    GLuint GetIndex(){
        return this->texture_index;
    }

    void Bind(){
        glBindTexture(GL_TEXTURE_2D, this->texture_index);
    }

};

#endif