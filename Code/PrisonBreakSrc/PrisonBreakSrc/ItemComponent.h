#pragma once
#include <string>
#include "SDL.h"
#include "ECS.h"
#include "Components.h"

class ItemComponent : public Component
{
private:
	int width, height, scale; // width, height, and scale (for transform component)
	const char* sprite_path_s; // static sprite path where item is on its own (for sprite component)
	const char* sprite_path_a; // animated sprite path where player is holding item (for sprite component)
	std::string item_name; // name of the item
	// required components (wrapper)
	TransformComponent* transform;
	SpriteComponent* sprite;
	ColliderComponent* collider;

public:
	// constructor
	ItemComponent(int w, int h, int s, const char* spath, const char* apath, std::string iname)
	{
		width = w;
		height = h;
		scale = s;
		sprite_path_s = spath;
		sprite_path_a = apath;
		item_name = iname;
	}

	// init function, runs once when object created
	void init() override
	{
		//a transform component is required for positioning
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>(0, 0, height, width, scale);
		}
		transform = &entity->getComponent<TransformComponent>();

		//a sprite component is required for having a sprite
		if (!entity->hasComponent<SpriteComponent>()) {
			entity->addComponent<SpriteComponent>(sprite_path_s, false);
		}
		sprite = &entity->getComponent<SpriteComponent>();

		//a collider component is required for collision
		if (!entity->hasComponent<ColliderComponent>()) {
			entity->addComponent<ColliderComponent>("item");
		}
		collider = &entity->getComponent<ColliderComponent>();
	}

	std::string getName() {
		return item_name;
	}

	const char* getCharacterModel() {
		return sprite_path_a;
	}
};