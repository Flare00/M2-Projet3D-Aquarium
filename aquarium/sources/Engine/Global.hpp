#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	global.screen_width = width;
	global.screen_height = height;
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	global.mouseX = xpos;
	global.mouseY = ypos;
}


#endif