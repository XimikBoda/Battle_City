#include "Game.h"

int main() 
{
	Window window;
	window.setToCenter();
	Game game(&window);
	game.run();
}