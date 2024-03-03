#pragma once
#include <string>
#include "SDL.h"
#include "ECS.h"
#include "Components.h"

class ObjectComponent : public Component {

private:
	int width, height;
	int scale = 1;
	const char* filepath;
	TransformComponent* transform;
	SpriteComponent* sprite;

public:


	//create a new object
	ObjectComponent(const char* file, int w, int h) {
		filepath = file;
		width = w;
		height = h;
	}

	ObjectComponent(const char* file, int w, int h, int s) {
		filepath = file;
		width = w;
		height = h;
		scale = s;
	}

	//create an object using another as a reference
	ObjectComponent(Entity* o) {
		
		filepath = o->getComponent<SpriteComponent>().file;
		width = o->getComponent<TransformComponent>().width;
		height = o->getComponent<TransformComponent>().height;
		scale = o->getComponent<TransformComponent>().scale;
	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>(0, 0, height, width, scale);
		}
		transform = &entity->getComponent<TransformComponent>();

		if (!entity->hasComponent<SpriteComponent>()) {
			entity->addComponent<SpriteComponent>(filepath);
		}
		sprite = &entity->getComponent<SpriteComponent>();
	}



};
