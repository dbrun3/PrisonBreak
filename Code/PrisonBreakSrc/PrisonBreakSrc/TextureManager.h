#pragma once

#include "Game.h"

//Manages loading and drawing SDL_Textures
class TextureManager
{
public:

	//LoadTexture() loads a particular texture from a file and returns it as an SDL_Texture*
	static SDL_Texture* LoadTexture(const char* filename);

	//Draw() renders a texture to a given src and dest SDL_Rect
	static void Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);

};