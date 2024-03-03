#pragma once
#include "ECS.h"
#include "Vector2D.h"
#include <vector>
class Room
{

private:
	//the name of the room, in case it needs to be referenced later
	const char* roomId;
	//tilemap of the room
	const char* tilemap;
	//dimensions of room
	int sizeX,sizeY;
	

public:

	//list of objects in the room
	std::vector<Entity*> objects;

	//room doors
	Vector2D west;
	Vector2D east;
	Vector2D north;
	Vector2D south;

	//Create room instance with associated room data file
	Room(const char* name, const char* path, int x, int y);

	~Room() {
		
	}

	//remove object from  vector
	void DeleteObject(Entity* e);

	//Add a generic object to room at location
	void AddObject(Entity* e, int x, int y);

	//Load a room's objects into the Game::objectGroup to be rendered
	void Load();

	//Remove a room's objects from objectGroup
	void Unload();

	const char* getId();
	const char* getRoomFile();
	int x();
	int y();
};

