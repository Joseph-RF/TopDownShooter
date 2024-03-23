#include "Classes.h"


int main()
{
	srand(time(NULL));

	Game game;

	while (game.getWindowIsOpen()) {
		game.update();

		game.render();
	}

	return 0;
}