#include "Game.h"
#include "Res.h"

CRes g_res;

int WINAPI WinMain (__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	CGame game;
	while (game.state()) {
		game.draw();
		game.go();
	}
	return 0;
}