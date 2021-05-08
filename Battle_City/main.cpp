#include "Game.h"
#include "MainMenu.h"
int main(int args,char **argv) 
{
	Window window;
	window.setToCenter();
	MainMenu menu(&window);
	menu.Run();
}