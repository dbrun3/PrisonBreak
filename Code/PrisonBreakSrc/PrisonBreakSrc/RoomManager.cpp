#include "RoomManager.h"
#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include <fstream>

extern Manager manager;

RoomManager::RoomManager(const char* mfp) {
	roomFilepath = mfp;
}

//removes current tiles and colliders from their respective groups so new ones can be loaded in
void RoomManager::UnloadRoom() {
	auto& npcs(manager.getGroup(Game::groupNPCs));
	auto& tiles(manager.getGroup(Game::groupTiles));
	auto& colliders(manager.getGroup(Game::groupColliders));
	auto& objects(manager.getGroup(Game::groupObjects));

	for (auto& t : tiles) {
		t->delGroup(Game::groupTiles);
		t->destroy();
	}

	for (auto& c : colliders) {
		c->delGroup(Game::groupColliders);
	}

	for (auto& o : objects) {
		o->delGroup(Game::groupObjects);
	}
	
	for (auto& n : npcs) {
		n->delGroup(Game::groupNPCs);
	}
}

void RoomManager::LoadRoom(Room* room) {

	UnloadRoom();
	//if (nav != nullptr) delete(nav); //delete old navmesh from memory
	nav = new Navmesh(); //create a new navmesh for the current room

	int sizeX = room->x();
	int sizeY = room->y();
	x = sizeX;
	y = sizeY;
	
	char c;
	std::fstream roomFile;
	roomFile.open(room->getRoomFile());

	int srcX, srcY;

	//Loads tiles from tileset to be displayed as room background
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			roomFile.get(c);
			std::cout << "Loaded tile: " << c;
			srcY = atoi(&c) * 32;
			roomFile.get(c);
			std::cout  << c;
			srcX = atoi(&c) * 32;
			AddTile(srcX, srcY, x * TILE_SIZE, y * TILE_SIZE);
			roomFile.ignore();  //skip comma

			std::cout << " from (" << srcX << ", " << srcY << ")" << std::endl;
		}
	}
	roomFile.ignore();  //skip comma

	//loads environment colliders to act as walls
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			roomFile.get(c);
			if (c == '1') {
				auto& tcol(manager.addEntity());									
				tcol.addComponent<ColliderComponent>("environment", x * TILE_SIZE + (TILE_SIZE/2), y * TILE_SIZE + (TILE_SIZE / 2)); 
				tcol.addGroup(Game::groupColliders);
				std::cout << "collider added" << std::endl;
			}

			if (c == '2') {
				nav->addNode(x * TILE_SIZE, y * TILE_SIZE);
			}

			if (c == 'n') {
				auto& north(manager.addEntity());	
				north.addComponent<TransformComponent>(x * TILE_SIZE - 80 , y * TILE_SIZE, 30, 360, 1);
				north.addComponent<ColliderComponent>("north"); 
				north.addGroup(Game::groupColliders);
				room->north = north.getComponent<TransformComponent>().position;
				
			}
			if (c == 'e') {
				auto& east(manager.addEntity());			
				east.addComponent<TransformComponent>(x * TILE_SIZE, y * TILE_SIZE - 80, 360, 30, 1);
				east.addComponent<ColliderComponent>("east");
				east.addGroup(Game::groupColliders);
				room->east = east.getComponent<TransformComponent>().position;
			}
			if (c == 's') {
				auto& south(manager.addEntity());		
				south.addComponent<TransformComponent>(x * TILE_SIZE - 80, y * TILE_SIZE, 30, 360, 1);
				south.addComponent<ColliderComponent>("south");
				south.addGroup(Game::groupColliders);
				room->south = south.getComponent<TransformComponent>().position;
			}
			if (c == 'w') {
				auto& west(manager.addEntity());			
				west.addComponent<TransformComponent>(x * TILE_SIZE, y * TILE_SIZE - 80, 360, 30, 1);
				west.addComponent<ColliderComponent>("west");
				west.addGroup(Game::groupColliders);
				room->west = west.getComponent<TransformComponent>().position;
			}

			roomFile.ignore();  //skip comma
		}
	}

	nav->generateGraph(70); //generate adjencency graph with max distance between nodes 140
	roomFile.close();
	room->Load();
}

void RoomManager::AddTile(int srcX, int srcY, int xpos, int ypos) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, roomFilepath);
	tile.addGroup(Game::groupTiles);
}
