#pragma once

#include "Components.h"
#include "Vector2D.h"
#include "Navmesh.h"

/*	<TransformComponent>

* Component gives an entity the properties of position and velocity.
* Required for all sprites.

*/
class NPCComponent : public Component {

private:
	//none for now

public:

	bool spotted = false;
	int shoot = 0;
	int timer = 0;
	Navmesh* nav = nullptr;
	std::vector<Node*> path;
	char last_direction = 's'; // 'w' = up, 'a' = left, 's' = down, 'd' = right

	TransformComponent* transform;
	SpriteComponent* sprite;
	const char* filepath;

	NPCComponent(Navmesh* n, const char* file) {
		nav = n;
		filepath = file;
	}


	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>(0, 0, 32, 32, 3);
		}
		transform = &entity->getComponent<TransformComponent>();

		if (!entity->hasComponent<SpriteComponent>()) {
			entity->addComponent<SpriteComponent>(filepath, true);
		}
		sprite = &entity->getComponent<SpriteComponent>();

	}

	void update() override {

		timer++;

		//if has path
		if (path.size() > 0) {
			Node* curr = path.front();

			//if currently standing on top of the front node...
			if (abs(transform->position.x - curr->x) < 5 && abs(transform->position.y - curr->y) < 5) {
				if (path.size() > 0) {
					path.erase(path.begin()); //pop front node
				}
			}


			if (abs(transform->position.x - curr->x) > abs(transform->position.y - curr->y)) {
				transform->velocity.y = 0;
				//walk left
				if (transform->position.x > curr->x) {

					transform->velocity.x = -1;
					last_direction = 'a';
				}
				//walk right
				else if (transform->position.x < curr->x) {

					transform->velocity.x = 1;
					last_direction = 'd';
				}
			}
			else {
				transform->velocity.x = 0;
				//walk up
				if (transform->position.y > curr->y) {

					transform->velocity.y = -1;
					last_direction = 'w';
				}
				//walk down
				else if (transform->position.y < curr->y) {

					transform->velocity.y = 1;
					last_direction = 's';
				}
			}

			switch (last_direction) {
			case 'w':
				sprite->playAnim("Walk_Up");
				break;
			case 'a':
				sprite->playAnim("Walk_Left");
				break;
			case 's':
				sprite->playAnim("Walk_Down");
				break;
			case 'd':
				sprite->playAnim("Walk_Right");
				break;
			default:
				sprite->playAnim("Walk_Down");
			}

		}
		else {


			if (shoot == 0) {
				transform->velocity.x = 0;
				transform->velocity.y = 0;
				switch (last_direction) {
				case 'w':
					sprite->playAnim("Walk_Up_Idle");
					break;
				case 'a':
					sprite->playAnim("Walk_Left_Idle");
					break;
				case 's':
					sprite->playAnim("Walk_Down_Idle");
					break;
				case 'd':
					sprite->playAnim("Walk_Right_Idle");
					break;
				default:
					sprite->playAnim("Walk_Down_Idle");
				}
			}
			else {
				shoot++;
				switch (last_direction) {
				case 'w':
					sprite->playAnim("Walk_Up_Use");
					break;
				case 'a':
					sprite->playAnim("Walk_Left_Use");
					break;
				case 's':
					sprite->playAnim("Walk_Down_Use");
					break;
				case 'd':
					sprite->playAnim("Walk_Right_Use");
					break;
				default:
					sprite->playAnim("Walk_Down_Use");
				}
			}
		}
	}
};