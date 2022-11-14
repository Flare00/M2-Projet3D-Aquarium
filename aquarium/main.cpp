#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>

//GL Libs
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Engine/Global.hpp>
#include <Engine/Engine.hpp>

double avance = 0.0;
void compute(double deltaTime){
    avance += deltaTime;
}

void mainLoop(){
	Engine engine;

    double lastFrame = 0.0f;
    double currentFrame = glfwGetTime();
    double deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    double sumDelta=  0.0;

    do{
        if(glfwGetKey(global.global_window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
			global.close_asked = true;
        }

        double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        sumDelta += deltaTime;

        if(sumDelta >= 1.0 / (global.fps_limit) ){

            std::string titre = "Moteur - ";
            titre += std::to_string((int)((1.0/sumDelta) +0.1));
            glfwSetWindowTitle(global.global_window, titre.c_str());

			engine.loop(sumDelta);

            glfwSwapBuffers(global.global_window);
			glfwPollEvents();
            sumDelta = 0.0f;
        }

    } while(!global.close_asked && glfwWindowShouldClose(global.global_window) == 0);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	global.screen_width = width;
	global.screen_height = height;
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

int main(int argc, char* argv[]){
    

    // Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Open a global.global_window and create its OpenGL context
    global.global_window = glfwCreateWindow(global.screen_width, global.screen_height, "Moteur", NULL, NULL);

	if (global.global_window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW global.global_window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(global.global_window);
	glfwSetFramebufferSizeCallback(global.global_window, framebuffer_size_callback);


	printf("OpenGL version supported by this platform : %s\n", glGetString(GL_VERSION));

    // Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
    // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(global.global_window, GLFW_STICKY_KEYS, GL_TRUE);

	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(global.global_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(global.global_window, global.screen_width / 2.0, global.screen_height / 2.0);

    //Clear background to clear grey
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);


    // Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//for Tesselation
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_FRAMEBUFFER_SRGB);

	// Launch main loop
	mainLoop();

	// Close OpenGL global_window and terminate GLFW
	glfwTerminate();

	return 0;
}