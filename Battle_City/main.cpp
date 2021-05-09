#include "Game.h"
#include "MainMenu.h"
int main(int args,char **argv) 
{
	srand(time(0));
	Window window;
	window.setToCenter();
	MainMenu menu(&window);
	menu.Run();
}