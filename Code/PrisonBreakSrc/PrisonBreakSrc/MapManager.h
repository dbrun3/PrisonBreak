#include <stdio.h>
#include "Room.h"

class MapManager {
    public:
        MapManager();
        ~MapManager();
        void addRoom(Room* room);
        Room* getRoom(int roomNum);
        Room* changeRoom(Room* currentRoom, char dir);
    private:
        Room* MapArray[5][5];
};
