#include "MapManager.h"

MapManager::MapManager() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            MapArray[i][j] = NULL;
        }
    }
    
}

MapManager::~MapManager() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Room* temp = MapArray[i][j];
            MapArray[i][j] = NULL;
            delete temp;
            temp = NULL;
            
        }
    }
}

void MapManager::addRoom(Room* room) {

    int placed = 0;
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (MapArray[i][j] == NULL) {
                MapArray[i][j] = room;
                placed = 1;
                break;
            }
        }
        if (placed) break;
    }
}

Room* MapManager::getRoom(int roomNum) {
    int n = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (n == roomNum) {
                return MapArray[i][j];
            }
            n++;
        }
    }
    return NULL;
}

Room* MapManager::changeRoom(Room* currentRoom, char dir) {
    int x = 0;
    int y = 0;
    int found = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (MapArray[i][j] == currentRoom) { // gets current grid position of room
                x = j;
                y = i;
                found = 1;
            }
        }
    }
    
    if (found) {
        switch (dir) {
            case 'n':
                if (y >= 1) {
                    return MapArray[--y][x];
                }
                break;
                
            case 'e':
                if (x <= 3) {
                    return MapArray[y][++x];
                }
                break;
            case 'w':
                if (x >= 1) {
                    return MapArray[y][--x];
                }
                break;
            case 's':
                if (y <= 3) {
                    return MapArray[++y][x];
                }
                break;
            default:
                break;
        }
    }
    
    return NULL;
}
