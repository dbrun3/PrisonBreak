#pragma once
#include <string>
#include "SDL.h"
#include "ECS.h"
#include "Components.h"

class ObstacleComponent : public Component
{
private:
	int width, height, scale; // width, height, and scale (for transform component)
	const char* sprite_path; // sprite path of obstacle (for sprite component)
	std::string key_name; // name of the key used to overcome the obstacle
	// required components (wrapper)
	TransformComponent* transform;
	SpriteComponent* sprite;
	ColliderComponent* collider;

public:
	// constructor
	ObstacleComponent(int w, int h, int s, const char* spath, std::string key)
	{
		width = w;
		height = h;
		scale = s;
		sprite_path = spath;
		key_name = key;
	}

	// init function, runs once when object created
	void init() override
	{
		//a transform component is required for positioning
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>(0, 0, height, width, scale);
		}
		//transform = &entity->getComponent<TransformComponent>();

		//a sprite component is required for having a sprite
		if (!entity->hasComponent<SpriteComponent>()) {
			entity->addComponent<SpriteComponent>(sprite_path, false);
		}
		//sprite = &entity->getComponent<SpriteComponent>();

		//a collider component is required for collision
		if (!entity->hasComponent<ColliderComponent>()) {
			entity->addComponent<ColliderComponent>("obstacle");
		}
		//collider = &entity->getComponent<ColliderComponent>();
	}

	std::string getKey() {
		return key_name;
	}
};