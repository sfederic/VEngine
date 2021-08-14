#include "Engine.h"

int main(int argc, char *argv[])
{
	engine.Init(argc, argv);
	engine.MainLoop();
	engine.Cleanup();
	 
	return 0;
}
