
#include "StartScreenIcon.h"

StartScreenIcon::StartScreenIcon(const char* textureSheet, SDL_Renderer* r, int h, int w, int x, int y, int sRY, bool clickable) {
    renderer = r;
    objTexture = loadTexture(textureSheet, r);
    height = h;
    width = w;
    xpos = x;
    ypos = y;
    sourceY = sRY;
    isClickable = clickable;
    
}

SDL_Texture* StartScreenIcon::loadTexture(const char *filename, SDL_Renderer* renderer) {
        SDL_Surface* tempSurface = IMG_Load(filename);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        return texture;
}

void StartScreenIcon::update() {
    srcRect.h = height;
    srcRect.w = width;
    srcRect.x = 0;
    srcRect.y = sourceY;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = width;
    destRect.h = height;
    
    SDL_GetMouseState(&pointerX, &pointerY);
    
    if (isClickable) {
        if ((pointerX >= xpos && pointerX <= (xpos + width)) && (pointerY >= ypos && pointerY <= (ypos + height))) {
            isSelected = true;
            srcRect.x = 200;
        }
        else {
            isSelected = false;
            srcRect.x = 0;
        }
    }
    
    
    
    
}

void StartScreenIcon::render() {
    SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}
