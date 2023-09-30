#include "Framework.h"
#include "MyGame.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Framework* game = new MyGame();

	game->Execute();

	delete game;
	
	return 0;
}
