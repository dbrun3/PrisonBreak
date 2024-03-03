#include "StartScreen.h"

StartScreenIcon* startButton;
StartScreenIcon* quitButton;
StartScreenIcon* titleLogo;
StartScreenIcon* howToButton;
StartScreenIcon* howToScreen;
bool showHowTo = false;
SDL_Renderer* renderer = nullptr;

StartScreen::StartScreen() {
    //TODO
}

StartScreen::~StartScreen() {
    //TODO
}

void StartScreen::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    int screenWidth = 0;
    int screenHeight = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    //initialize window
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems Initialized..." << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            std::cout << "Start screen window created!" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "Start screen renderer created!" << std::endl;
        }

        screenWidth = width;
        screenHeight = height;

        isRunning = true;
    }
    else {
        isRunning = false;
    }
    
    titleLogo = new StartScreenIcon("assets/titlescreen.png", renderer, 600, 800, (screenWidth / 2) - 400, 0, 0, false);
    
    startButton = new StartScreenIcon("assets/StartQuitSmaller.png", renderer, 50, 200, (screenWidth / 2) - 100, (screenHeight / 1) - 200, 0, true);
    
    quitButton = new StartScreenIcon("assets/StartQuitSmaller.png", renderer, 50, 200, (screenWidth / 2) - 100, (screenHeight - 50) - 25, 50, true);
    
    howToButton = new StartScreenIcon("assets/StartQuitSmaller.png", renderer, 50, 200, (screenWidth / 2) - 95, (screenHeight - 135), 100, true);
    
    howToScreen = new StartScreenIcon("assets/pbHTPscreen.png", renderer, 600, 800, (screenWidth / 2) - 400, 0, 0, false);
    
    
    
}

void StartScreen::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    
    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (startButton->isSelected) {
                    isRunning = false;
                    startGame = true;
                }
                
                if (howToButton->isSelected) {
                    showHowTo = true;
                }
                
                if (quitButton->isSelected) {
                    isRunning = false;
                }
            }
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                if (showHowTo) {
                    showHowTo = false;
                }
            }

    default:
        break;
    }
}

void StartScreen::update() {
    if (!showHowTo) {
        titleLogo->update();
        startButton->update();
        howToButton->update();
        quitButton->update();
    }
    else {
        howToScreen->update();
    }
    
}

void StartScreen::render() {
    SDL_RenderClear(renderer);
    if (!showHowTo) {
        titleLogo->render();
        startButton->render();
        howToButton->render();
        quitButton->render();
    }
    else {
        howToScreen->render();
    }
    
    SDL_RenderPresent(renderer);
}

void StartScreen::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Start screen cleaned." << std::endl;
}



