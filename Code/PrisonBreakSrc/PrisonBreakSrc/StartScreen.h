#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "StartScreenIcon.h"
#include <stdio.h>
#include <iostream>

class StartScreen
{
public:
    StartScreen();
    ~StartScreen();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() { return isRunning; }
    bool getSelection() { return startGame; }

private:
    bool isRunning;
    bool startGame = false;
    SDL_Window* window;
    SDL_Renderer* renderer;
};
