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

	double ScreenAspectRatio() {
		return ((float)screen_width) / ((float)screen_height);
	}
};

Global global;
#endif