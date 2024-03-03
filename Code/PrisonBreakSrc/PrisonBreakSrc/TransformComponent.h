#pragma once

#include "Components.h"
#include "Vector2D.h"

/*	<TransformComponent>

* Component gives an entity the properties of position and velocity.
* Required for all sprites.

*/
class TransformComponent : public Component {

public:

	Vector2D position;
	Vector2D velocity;
	const char* room; //room id entity is currently in

	int height = 32;
	int width = 32;
	int scale = 1;

	float speed = 3; //default speed

	TransformComponent() {
		position.Zero();
	}

	TransformComponent(int s) {
		position.Zero();
		scale = s;
	}

	TransformComponent(int x, int y) {
		position.x = (float) x;
		position.y = (float) y;
	}

	TransformComponent(int x, int y, int h, int w, int s) {
		position.x = (float)x;
		position.y = (float)y;
		height = h;
		width = w;
		scale = s;
	}

	void init() override {
		velocity.Zero();
	}

	void update() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

};