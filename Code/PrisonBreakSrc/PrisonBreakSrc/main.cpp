//TODO: Add documentation



// ADD DESTRUCTORS TO ALL CLASSES THAT LACK THEM !!!!!!!



#include "Game.h"
#include "StartScreen.h"

Game* game = nullptr;
StartScreen* start = nullptr;
bool startGame = false;
bool done = false;

int main(int argc, char* argv[]) {
    while (!done) {
        start = new StartScreen();
        

        start->init("Prison Break", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, false);
        
        while (start->running()) {
            start->handleEvents();
            start->update();
            start->render();
        }
        startGame = start->getSelection();
        
        start->clean();
        delete start;
        
        if (startGame) {
            const int FPS = 60;
            const int frameDelay = 1000 / FPS;

            Uint32 frameStart;
            int frameTime;

            game = new Game();
            game->init("GAME WINDOW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 640, false);


            while (game->running()) {

                frameStart = SDL_GetTicks();

                game->handleEvents();
                game->update();
                game->render();

                frameTime = SDL_GetTicks() - frameStart;

                if (frameDelay > frameTime) {
                    SDL_Delay(frameDelay - frameTime);
                }
            }
            if (game->forceClosed()) {
                done = true;
            }

            game->clean();
            delete game;
            game = nullptr;
        }
        else {
            done = true;
        }
    }
    
	

	return 0;

}
