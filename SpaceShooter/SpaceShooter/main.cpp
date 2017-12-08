#include "main.h"

int main(int argc, char** argv) {
	Game game = Game();
	if (game.init() > 0) {
		cout << "Error failed to initialize!" << endl;
		return 1;
	}
	game.gs = GS_MAIN;

	//main loop
	while (game.gs != GS_DONE) {
		if (game.gs == GS_MAIN) {
			game.run();
		}
	}
	// end main

	printf("Exited\n");
	return 0;
}