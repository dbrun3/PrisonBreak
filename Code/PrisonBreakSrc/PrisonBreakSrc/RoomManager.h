#pragma once
#include <string>
#include "Room.h"
#include "Navmesh.h"

class RoomManager
{
private:
	const char* roomFilepath;
	Navmesh* nav = nullptr;

	//Removes current room's objects, tiles and colliders) from their render groups
	void UnloadRoom();

public:
	int x;
	int y;

	RoomManager() {}
	RoomManager(const char* mfp);
	~RoomManager() {
		delete(nav);
	}

	//Loads room data (dimensions, tiles and colliders) from a room object.
	void LoadRoom(Room* room);

	//Adds tiles to game manager's collider group
	void AddTile(int srcX, int srcY, int xpos, int ypos);

	//Self-explanatory
	Navmesh* getNavmesh() { return nav; }
};

