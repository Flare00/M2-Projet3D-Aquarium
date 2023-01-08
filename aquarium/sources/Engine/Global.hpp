#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/// <summary>
/// Global datas, sush as the windows, resolution or mouse informations.
/// </summary>
struct Global {
	//window
	GLFWwindow* global_window;
	//settings
	unsigned int screen_width = 900;
	unsigned int screen_height = 600;
	bool close_asked = false;
	bool wireframe = false;
	bool debug = true;
	bool debug_shader = false;
	bool debug_file_load = false;
	double fps_limit = 60.0;
	double mouseX = 0.0;
	double mouseY = 0.0;

	double ScreenAspectRatio() {
		return ((float)screen_width) / ((float)screen_height);
	}
};

Global global;

/// <summary>
/// Handle the screen resolution change.
/// </summary>
/// <param name="window">The concerned windows.</param>
/// <param name="width">New Width</param>
/// <param name="height">New Height</param>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	global.screen_width = width;
	global.screen_height = height;
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

/// <summary>
/// Callback of the mouse position change.
/// </summary>
/// <param name="window">The concerned window</param>
/// <param name="xpos">The new X position</param>
/// <param name="ypos">The new Y position</param>
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	global.mouseX = xpos;
	global.mouseY = ypos;
}

#endif