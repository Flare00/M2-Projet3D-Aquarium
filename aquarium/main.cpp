#include <Engine/Engine.hpp>

/// <summary>
/// Main function, initialize the Engine, and start it.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns>If engine stop normally.</returns>
int main(int argc, char* argv[]){
	Engine engine;
	//Init the engine.
	if (engine.Init() == -1) {
		return 1;
	}
	//Init the scenes.
	engine.InitScenes();
	//Launch the main loop.
	engine.MainLoop();
	//Close the program.
	engine.Terminate();
	return 0;
}