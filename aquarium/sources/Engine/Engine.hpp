#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Engine/Scene/SceneAquarium.hpp>

#include <Engine/Global.hpp>
#include <Engine/GameObject.hpp>
#include <Graphics/Graphics.hpp>
#include <Physics/Physics.hpp>

#include <Engine/Tools/Tools.hpp>

/// <summary>
/// The 3D Engine with graphics and Simple Physics.
/// </summary>
class Engine {

protected:
	// Graphics system.
	Graphics graphics;

	// Physics system.
	Physics physics;

	//Scene list with cursor and number.
	size_t activeScene = -1;
	std::vector<Scene> scenes;
	size_t nbScene = 0; 

	//Waiting timer to avoid too fast cycling when pressing a button.
	double reloadWait = 0.0;
	double wireframeWait = 0.0;

public:

	Engine() {
	}

	/// <summary>
	/// Destructor calling terminate automatically.
	/// </summary>
	~Engine() { Terminate(); }

	/// <summary>
	/// The Engine Initialisation (With OpenGL);
	/// </summary>
	/// <returns></returns>
	int Init() {
		// Initialise GLFW
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			getchar();
			return -1;
		}
		// GLFW Parameters
		glfwWindowHint(GLFW_SAMPLES, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		// Open a global.global_window and create its OpenGL context
		global.global_window = glfwCreateWindow(global.screen_width, global.screen_height, "Aquarium 3D", NULL, NULL);
		if (global.global_window == NULL)
		{
			fprintf(stderr, "Failed to open GLFW global.global_window.\n");
			getchar();
			glfwTerminate();
			return -1;
		}

		//Set Size Callback
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

		// Set the mouse to normal
		glfwSetInputMode(global.global_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// Set the mouse at the center of the screen
		glfwPollEvents();
		//glfwSetCursorPos(global.global_window, global.screen_width / 2.0, global.screen_height / 2.0);

		//Clear background to grey
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

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
		glDisable(GL_FRAMEBUFFER_SRGB);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
		// Setup mouse callback
		glfwSetCursorPosCallback(global.global_window, cursorPositionCallback);

		//Init Modules.
		graphics.Init();
		physics.Init();

		return 0;
	}

	/// <summary>
	/// The Scenes Initialization.
	/// </summary>
	void InitScenes() {
		scenes.push_back(SceneAquarium("Aquarium 3D"));
		activeScene = 0;

		nbScene = scenes.size();
		if (activeScene >= 0 && activeScene < nbScene) {
			scenes[activeScene].start();
			//physics.AddGameObjectToOctreeRecursive(scenes[activeScene].GetRoot());
		}
	}

	/// <summary>
	/// The Main Loop using GLFW events and calling Loop.
	/// </summary>
	void MainLoop() {
		double lastFrame = 0.0f;
		double currentFrame = glfwGetTime();
		double deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		double sumDelta = 0.0;

		do {
			if (glfwGetKey(global.global_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				global.close_asked = true;
			}

			double currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			sumDelta += deltaTime;

			if (sumDelta >= 1.0 / (global.fps_limit)) {

				std::string titre = "Moteur - ";
				titre += std::to_string((int)((1.0 / sumDelta) + 0.1));
				glfwSetWindowTitle(global.global_window, titre.c_str());

				Loop(sumDelta);

				glfwSwapBuffers(global.global_window);
				glfwPollEvents();
				sumDelta = 0.0f;
			}

		} while (!global.close_asked && glfwWindowShouldClose(global.global_window) == 0);
	}

	/// <summary>
	/// The Active Scene loop, called by MainLoop.
	/// </summary>
	/// <param name="deltaT">Time since last frame.</param>
	void Loop(double deltaT) {
		if (activeScene >= 0 && activeScene < nbScene) {
			reloadWait -= deltaT;
			wireframeWait -= deltaT;
			if (glfwGetKey(global.global_window, GLFW_KEY_R) == GLFW_PRESS && reloadWait < 0) {
				scenes[activeScene].ReloadShaders();
				reloadWait = 0.5;
			}

			if (glfwGetKey(global.global_window, GLFW_KEY_Z) == GLFW_PRESS && wireframeWait < 0) {
				global.wireframe = !global.wireframe;
				wireframeWait = 0.5;
			}

			scenes[activeScene].loop(deltaT);

			GameObject* root = scenes[activeScene].GetRoot();
			physics.Compute(deltaT, root);
			graphics.Compute(root);
		}
	}

	/// <summary>
	/// Terminate the engine.
	/// </summary>
	void Terminate() {
		glfwTerminate();
	}

	/// <summary>
	/// Change the active scene by is index position.
	/// </summary>
	/// <param name="scene_number">The index position of the wanted scene.</param>
	void ChangeScene(size_t scene_number) {
		if (scene_number >= 0 && scene_number < nbScene) {
			scenes[activeScene].stop();
			activeScene = scene_number;
			scenes[activeScene].start();
		}
	}

	/// <summary>
	/// Change the active scene, by it's ID.
	/// </summary>
	/// <param name="scene_id">The id of the wanted scene.</param>
	void ChangeScene(std::string scene_id) {
		size_t index = -1;
		for (size_t i = 0, max = scenes.size(); i < max && index == -1; i++) {
			if (scenes[i].id == scene_id) {
				index = i;
			}
		}
		this->ChangeScene(index);
	}

	/// <summary>
	/// Return the Active Scene Root Gameobject.
	/// </summary>
	/// <returns>The Active Scene Root Gameobject</returns>
	GameObject* GetActiveSceneRoot() {
		return this->scenes[activeScene].GetRoot();
	}


	/// <summary>
	/// Return the Graphics system.
	/// </summary>
	/// <returns>The Graphics system.</returns>
	Graphics* GetGraphics() {
		return &this->graphics;
	}

	/// <summary>
	/// Return the Physics system.
	/// </summary>
	/// <returns>The Physics system.</returns>
	Physics* GetPhysics() {
		return &this->physics;
	}
};

#endif