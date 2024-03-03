#include "ECS.h"
#include "Room.h"
#include "Components.h"
#include <vector>

Room::Room(const char* id, const char* path, int x, int y) {
	roomId = id;
	tilemap = path;
	sizeX = x;
	sizeY = y;
	
	
}

void Room::AddObject(Entity* e, int x, int y) {

	TransformComponent* transform = &e->getComponent<TransformComponent>();
	transform->position.x = x;
	transform->position.y = y;


	objects.push_back(e);
}

void Room::DeleteObject(Entity* e) {
	
	auto it = std::find(objects.begin(), objects.end(), e);
	if (it != objects.end()) { objects.erase(it); }

}

void Room::Load() {
	for (auto& o : objects) {

		//add npc
		if (o->hasComponent<NPCComponent>()) {
			if(!o->hasGroup(Game::groupNPCs))
			o->addGroup(Game::groupNPCs);
			continue;
		}

		//add objects, items, obstacles
		if (o->hasComponent<ObjectComponent>() || o->hasComponent<ItemComponent>() || o->hasComponent<ObstacleComponent>()) {
			if (o->hasComponent<ColliderComponent>()) {

				if(!o->hasGroup(Game::groupColliders))
				o->addGroup(Game::groupColliders);
			}
			if (!o->hasGroup(Game::groupObjects))
			o->addGroup(Game::groupObjects);
		}
	}
}

const char* Room::getId() { return roomId; }
const char* Room::getRoomFile() { return tilemap; }
int Room::x() {	return sizeX; }
int Room::y() { return sizeY; }
