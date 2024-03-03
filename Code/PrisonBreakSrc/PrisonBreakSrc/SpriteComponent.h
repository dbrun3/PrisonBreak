#pragma once

#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>

/*	<SpriteComponent>

* Component produces a visual representation of an entity with an SDL_Texture
  from a filepath upon initial construction. 
* Textures may also be updated later with the setTexture() function. 
 
* Requires an entity possesses a <TransformComponent>

*/
class SpriteComponent : public Component {
private:
	TransformComponent* transform;
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect, destRect;

	bool isAnimated = false; // sprite is not animated by default
	int frames = 0; // the amount of frames the animated sprite contains
	int speed = 100; // delay between frames in milliseconds
	int col = 0; // first frame by default

public:
	int animIndex = 0;
	std::map<const char*, Animation> animations;
	const char* file;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE; // render flipping (not used yet but may come in handy) 

	SpriteComponent() = default;

	SpriteComponent(const char* filepath) {
		file = filepath;
		setTexture(filepath);
	}

	SpriteComponent(const char* filepath, bool animated) {
		file = filepath;

		isAnimated = animated;

		// create animations
		Animation walk_up = Animation(2, 0, 3, 100);
		Animation walk_down = Animation(0, 0, 3, 100);
		Animation walk_left = Animation(3, 0, 3, 100);
		Animation walk_right = Animation(1, 0, 3, 100);
		// add animations to map
		animations.emplace("Walk_Up", walk_up);
		animations.emplace("Walk_Down", walk_down);
		animations.emplace("Walk_Left", walk_left);
		animations.emplace("Walk_Right", walk_right);
		// add idle "animations" (which are just 1 frame animations, like a static image)
		animations.emplace("Walk_Up_Idle", Animation(2, 0, 1, INT_MAX));
		animations.emplace("Walk_Down_Idle", Animation(0, 0, 1, INT_MAX));
		animations.emplace("Walk_Left_Idle", Animation(3, 0, 1, INT_MAX));
		animations.emplace("Walk_Right_Idle", Animation(1, 0, 1, INT_MAX));
		// add use animations (last frame)
		animations.emplace("Walk_Up_Use", Animation(2, 3, 1, INT_MAX));
		animations.emplace("Walk_Down_Use", Animation(0, 3, 1, INT_MAX));
		animations.emplace("Walk_Left_Use", Animation(3, 3, 1, INT_MAX));
		animations.emplace("Walk_Right_Use", Animation(1, 3, 1, INT_MAX));

		playAnim("Walk_Down_Idle"); // default state when game booted is Walk_Down_Idle

		// fire animation
		animations.emplace("Fire_Animation", Animation(0, 0, 2, 150));

		//rope animation
		animations.emplace("rope_idle", Animation(0, 0, 1, INT_MAX));
		animations.emplace("rope_burn", Animation(0, 1, 2, 100));

		//ammo transformation
		animations.emplace("ammo1", Animation(0, 0, 1, INT_MAX));
		animations.emplace("ammo2", Animation(0, 1, 1, INT_MAX));

		setTexture(filepath);
	}

	~SpriteComponent() {
		SDL_DestroyTexture(texture);
	}
    
	void setTexture(const char* filepath) {
        if (texture != NULL) {
            SDL_DestroyTexture(texture);
        }
		texture = TextureManager::LoadTexture(filepath);
	}

	void init() override {

		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override {
		if (isAnimated) {
			srcRect.x = srcRect.w * static_cast<int>(((SDL_GetTicks() / speed) % frames) + col); // sets what column of the spritesheet gets used
		}

		srcRect.y = animIndex * transform->height; // sets what row of the spritesheet gets used

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.h = transform->height * transform->scale;
		destRect.w = transform->width * transform->scale;
	}

	void draw() override {
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	// function to play sprite animations
	void playAnim(const char* animName) {
		animIndex = animations[animName].index;
		frames = animations[animName].frames;
		speed = animations[animName].speed;
		col = animations[animName].col;
	}
};
