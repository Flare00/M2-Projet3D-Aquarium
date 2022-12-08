#include <Engine/Global.hpp>

int main(int argc, char* argv[]){

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