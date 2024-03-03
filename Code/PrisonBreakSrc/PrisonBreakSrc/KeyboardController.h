#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

/*	<KeyboardController> 
 
* Component gives an entity the property of having its velocity affected
  the user's keyboard. 
* Intended for the player entity.

* Requires that an entity also has a <TransformComponent>

*/
class KeyboardController : public Component
{
private:
	int wasd[4] = { 0, 0, 0, 0 };
	char last_looked = 'd';
	bool activate = false;
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = -1;
				sprite->playAnim("Walk_Up");
				wasd[0] = 1;
				last_looked = 'u';
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				sprite->playAnim("Walk_Left");
				wasd[1] = 1;
				last_looked = 'l';
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				sprite->playAnim("Walk_Down");
				wasd[2] = 1;
				last_looked = 'd';
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				sprite->playAnim("Walk_Right");
				wasd[3] = 1;
				last_looked = 'r';
				break;
			case SDLK_e:
				transform->velocity.x = 0;
				transform->velocity.y = 0;
				activate = true;
				switch (last_looked) {
				case 'u':
					sprite->playAnim("Walk_Up_Use");
					break;
				case 'd':
					sprite->playAnim("Walk_Down_Use");
					break;
				case 'l':
					sprite->playAnim("Walk_Left_Use");
					break;
				case 'r':
					sprite->playAnim("Walk_Right_Use");
					break;
				}
			case SDLK_q:
				activate = true;
				switch (last_looked) {
				case 'u':
					sprite->playAnim("Walk_Up_Use");
					break;
				case 'd':
					sprite->playAnim("Walk_Down_Use");
					break;
				case 'l':
					sprite->playAnim("Walk_Left_Use");
					break;
				case 'r':
					sprite->playAnim("Walk_Right_Use");
					break;
				}
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = 0;
				wasd[0] = 0;
				if (wasd[0] == 1)
					sprite->playAnim("Walk_Up");
				else if (wasd[1] == 1)
					sprite->playAnim("Walk_Left");
				else if (wasd[2] == 1)
					sprite->playAnim("Walk_Down");
				else if (wasd[3] == 1)
					sprite->playAnim("Walk_Right");
				else if (!activate)
					sprite->playAnim("Walk_Up_Idle");
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				wasd[1] = 0;
				if (wasd[0] == 1)
					sprite->playAnim("Walk_Up");
				else if (wasd[1] == 1)
					sprite->playAnim("Walk_Left");
				else if (wasd[2] == 1)
					sprite->playAnim("Walk_Down");
				else if (wasd[3] == 1)
					sprite->playAnim("Walk_Right");
				else if (!activate)
					sprite->playAnim("Walk_Left_Idle");
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				wasd[2] = 0;
				if (wasd[0] == 1)
					sprite->playAnim("Walk_Up");
				else if (wasd[1] == 1)
					sprite->playAnim("Walk_Left");
				else if (wasd[2] == 1)
					sprite->playAnim("Walk_Down");
				else if (wasd[3] == 1)
					sprite->playAnim("Walk_Right");
				else if (!activate)
					sprite->playAnim("Walk_Down_Idle");
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				wasd[3] = 0;
				if (wasd[0] == 1)
					sprite->playAnim("Walk_Up");
				else if (wasd[1] == 1)
					sprite->playAnim("Walk_Left");
				else if (wasd[2] == 1)
					sprite->playAnim("Walk_Down");
				else if (wasd[3] == 1)
					sprite->playAnim("Walk_Right");
				else if (!activate)
					sprite->playAnim("Walk_Right_Idle");
				break;
			case SDLK_e:
				activate = false;
				switch (last_looked) {
				case 'u':
					sprite->playAnim("Walk_Up_Idle");
					break;
				case 'd':
					sprite->playAnim("Walk_Down_Idle");
					break;
				case 'l':
					sprite->playAnim("Walk_Left_Idle");
					break;
				case 'r':
					sprite->playAnim("Walk_Right_Idle");
					break;
				}
				break;
			case SDLK_q:
				activate = false;
				switch (last_looked) {
				case 'u':
					sprite->playAnim("Walk_Up_Idle");
					break;
				case 'd':
					sprite->playAnim("Walk_Down_Idle");
					break;
				case 'l':
					sprite->playAnim("Walk_Left_Idle");
					break;
				case 'r':
					sprite->playAnim("Walk_Right_Idle");
					break;
				}
				break;
			default:
				break;
			}
		}
		/* ORIGINAL CODE IN CASE MY "FIX" actually causes problems
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = -1;
				sprite->playAnim("Walk_Up");
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				sprite->playAnim("Walk_Left");
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				sprite->playAnim("Walk_Down");
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				sprite->playAnim("Walk_Right");
				break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = 0;
				sprite->playAnim("Walk_Up_Idle");
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				sprite->playAnim("Walk_Left_Idle");
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->playAnim("Walk_Down_Idle");
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->playAnim("Walk_Right_Idle");
				break;
			default:
				break;
			}
		}
		*/
	}
};
