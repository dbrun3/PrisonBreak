#pragma once

#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"


class StartScreenIcon
{
public:
    StartScreenIcon(const char* textureSheet, SDL_Renderer* r, int h, int w, int x, int y, int sRY, bool clickable);
    ~StartScreenIcon();
    static SDL_Texture* loadTexture(const char* filename, SDL_Renderer* r);
    void update();
    void render();
    bool isSelected = false;
    
private:
    int xpos;
    int ypos;
    int height;
    int width;
    
    int pointerX;
    int pointerY;
    
    
    int sourceY;

    SDL_Texture* objTexture;
    SDL_Rect srcRect, destRect;
    SDL_Renderer* renderer;
    
    bool isClickable = false;
};
