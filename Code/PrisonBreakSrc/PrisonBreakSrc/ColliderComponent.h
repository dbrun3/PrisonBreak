#pragma once

#include <string>
#include "SDL.h"
#include "ECS.h"
#include "Components.h"

/*
* <ColliderComponent>
*
* Allows an object to possess a hitbox, that can be be checked for overlapping
* with other hitboxes by the Game::update() function
*
*/
class ColliderComponent : public Component {


public:
	SDL_Rect collider; //basically a hitbox
	std::string tag;   //used to identify a collision

	TransformComponent* transform;

	ColliderComponent(std::string t) {
		tag = t;
	}

	ColliderComponent(std::string t, int x, int y) {
		tag = t;
		collider.x = x;
		collider.y = y;
		collider.w = TILE_SIZE;
		collider.h = TILE_SIZE;
	}
	
	void init() override {

		//a transform component is required for collision
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		if (tag != "environment") {
			collider.h = transform->height * transform->scale;
			collider.w = transform->width * transform->scale;
		}
	}

	void update() override {
		//set hitbox position to that of the transform component for sprites
		if (tag != "environment") {
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
		}
	}

};
