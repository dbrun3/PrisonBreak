#include "Game.h"
#include "TextureManager.h"
#include "RoomManager.h"
#include "Room.h"
#include "Components.h"
#include "Vector2D.h"
#include "KeyboardController.h"
#include "Collision.h"

int screenWidth, screenHeight;
bool paused = false;
bool game_over = false;
bool win = false;
bool escape = false;
bool disguise = false;
SDL_Rect pause_rect;
SDL_Rect win_rect;
SDL_Rect over_rect;
bool preventChange = false;
int timer = 0;
Manager manager;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Rect Game::camera = { 0,0,0,0 };

SDL_Texture* pauseIcon = nullptr;
SDL_Texture* winIcon = nullptr;
SDL_Texture* loseIcon = nullptr;

// variable representing the current item
Entity* currentItem = nullptr;
int currentIndex = -1;
// inventory
std::vector<Entity*> inventory;

//metal door and id scanner. need to reference later

bool metalDoorOpen = false;

//escape wall and barrels n rope. to be referenced later


//******************************TEST YOUR TILESHEETS HERE*************************************//

const char* tilesheet1 = "assets/tileset1.png"; 
const char* tilesheet2 = "assets/tileset2.PNG";

//******************************TEST YOUR TILESHEETS HERE*************************************//

Game::Game() {
	// variable representing the current item
	currentItem = nullptr;
    currentRoom = nullptr;
    currentNavmesh = nullptr;
	currentIndex = -1;
	// inventory
	inventory.clear();
    player = &manager.addEntity();
    metaldoor = &manager.addEntity();
    idscan = &manager.addEntity();
    escapeWall = &manager.addEntity();
    boomWall = &manager.addEntity();
    barrels = &manager.addEntity();
    rope_obs = &manager.addEntity();
    tiles = &manager.getGroup(Game::groupTiles);
    players = &manager.getGroup(Game::groupPlayers);
    npcs = &manager.getGroup(Game::groupNPCs);
    colliders = &manager.getGroup(Game::groupColliders);
    objects = &manager.getGroup(Game::groupObjects);
    
}

Game::~Game() {
    player = nullptr;
    metaldoor = nullptr;
    idscan = nullptr;
    escapeWall = nullptr;
    boomWall = nullptr;
    barrels = nullptr;
    rope_obs = nullptr;
    tiles = nullptr;
    players = nullptr;
    npcs = nullptr;
    colliders = nullptr;
    objects = nullptr;
    currentItem = nullptr;
    currentRoom = nullptr;
    currentNavmesh = nullptr;
    paused = false;
    escape = false;
    preventChange = false;
    disguise = false;
    metalDoorOpen = false;
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	//initialize window
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems Initialized..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer){
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created!" << std::endl;
		}

		screenWidth = width;
		screenHeight = height;

		isRunning = true;
        forceQuit = false;
	}
	else {
		isRunning = false;
	}

	roomManager = RoomManager(tilesheet2);

	//***************************vvv ADD ROOMS HERE vvv**************************//

	

	Room* start_room = new Room("cellblock", "assets/cellblockmap.txt", 48, 25);
	Room* hallway_vent = new Room("hallway_with_vent", "assets/hallway_with_vent.txt", 29, 23);
	Room* death_row = new Room("deathrow", "assets/deathrowmap.txt", 44, 23);
	Room* shower_room = new Room("showers", "assets/showerRoom.txt", 26, 28);
	Room* admin_office = new Room("admin_office", "assets/admin_office.txt", 36, 29);
    Room* visitationPrisonerSide = new Room("visitation_ps", "assets/VisitorRoomPrisoner.txt", 26, 18);
	Room* hallwayI1 = new Room("hallwayI1", "assets/hallwayI1.txt", 26, 23);
	Room* hallwayI2v2 = new Room("hallwayI2", "assets/hallwayI2.txt", 26, 20);
    Room* hallwayP1 = new Room("hallwayP1", "assets/hallwayPlus1.txt", 26, 25);
    Room* hallwayT2 = new Room("hallwayT2", "assets/hallwayT2.txt", 26, 25);
    Room* hallwayT4 = new Room("hallwayT4", "assets/hallwayT4.txt", 26, 24);
    Room* library_room = new Room("library", "assets/LibraryRoom.txt", 30, 18);
    Room* laundry_room = new Room("laundry", "assets/LaundryRoom.txt", 28, 18);
	Room* warden = new Room("warden", "assets/warden.txt", 23, 19);
    Room* fsVisitation = new Room("freeSVisitation", "assets/visitationFreeSide.txt", 22, 23);
    Room* hallwayT3_2 = new Room("hallwayT3_2", "assets/hallwayT3.txt", 26, 20);
    Room* hallwayT3_3 = new Room("hallwayT3_3", "assets/hallwayT3.txt", 26, 20);
    Room* hallwayI2_3 = new Room("hallwayI2_3", "assets/hallwayI2_3.txt", 26, 20);
    Room* lockerRoom = new Room("lockerRoom", "assets/lockerRoom.txt", 22, 18);
	Room* ammo = new Room("ammo", "assets/ammoroom.txt", 47, 22);
	Room* end = new Room("end", "assets/end.txt", 30, 20);
	Room* empty1 = new Room("", "", 0, 0);
    Room* empty2 = new Room("", "", 0, 0);
    Room* empty3 = new Room("", "", 0, 0);
    Room* empty4 = new Room("", "", 0, 0);



	map.addRoom(fsVisitation);
	map.addRoom(empty1);
	map.addRoom(warden);
	map.addRoom(ammo);
	map.addRoom(end); //TODO:ESCAPE HERE

	map.addRoom(visitationPrisonerSide);
	map.addRoom(hallwayT4);
	map.addRoom(admin_office);
	map.addRoom(hallwayT2); //add metal door to ammo at north exit
	map.addRoom(empty2);

	map.addRoom(start_room);
	map.addRoom(hallway_vent);
	map.addRoom(hallwayI2v2);
	map.addRoom(hallwayP1);
	map.addRoom(laundry_room);

	map.addRoom(shower_room);
	map.addRoom(hallwayI1);
	map.addRoom(empty3);
	map.addRoom(death_row);
	map.addRoom(empty4);

	map.addRoom(library_room);
	map.addRoom(hallwayT3_3);
	map.addRoom(hallwayI2_3);
	map.addRoom(hallwayT3_2);
	map.addRoom(lockerRoom);


	currentRoom = start_room; //change starting room here for testing

      
	//**************************^^^ ADD ROOMS HERE ^^^**************************//
	/*
	          .     ’     .    :    .  ●  .    *:  . ‘
          ‘  :    *          .     *   .        *        ’     .   :    ’
            ______𐐘𐑀 _____________
	*/
	//*************************vvv ADD OBJECTS HERE vvv*************************//


	//end
	auto& endPic(manager.addEntity());
	endPic.addComponent<ObjectComponent>("assets/end.png", 1460, 1085);
	end->AddObject(&endPic, 1000, 127);
    


    // admin guard 1
	auto& sussybaka(manager.addEntity());
	sussybaka.addComponent<NPCComponent>(currentNavmesh, "assets/pbGuardAnim.png");
	admin_office->AddObject(&sussybaka, 400, 400);
    
    // admin guard 2
    auto& betterRun(manager.addEntity());
    betterRun.addComponent<NPCComponent>(currentNavmesh, "assets/pbGuardAnim.png");
    admin_office->AddObject(&betterRun, 1120, 960);
    
    // library guard
    auto& notALibrarian(manager.addEntity());
    notALibrarian.addComponent<NPCComponent>(currentNavmesh, "assets/pbGuardAnim.png");
    library_room->AddObject(&notALibrarian, 320, 500);
    
    // visitation guard
    auto& youHaveAVisitor(manager.addEntity());
    youHaveAVisitor.addComponent<NPCComponent>(currentNavmesh, "assets/pbGuardAnim.png");
    visitationPrisonerSide->AddObject(&youHaveAVisitor, 864, 384);
    
    // death row guard
    auto& terminator(manager.addEntity());
    terminator.addComponent<NPCComponent>(currentNavmesh, "assets/pbGuardAnim.png");
    death_row->AddObject(&terminator, 1504, 704);
    
    // t2 hallway guard
    auto& goodLuck(manager.addEntity());
    goodLuck.addComponent<NPCComponent>(currentNavmesh, "assets/pbGuardAnim.png");
    hallwayT2->AddObject(&goodLuck, 640, 384);



	auto& startGuard(manager.addEntity());
	startGuard.addComponent<NPCComponent>(currentNavmesh, "assets/pbGuardAnim.png");
	start_room->AddObject(&startGuard, 1700, 900);


	//       vvv   DECORATOR / GENERIC   vvv

	// SUPER GENERICS HERE

	//start_room
	auto& table0(manager.addEntity());
	table0.addComponent<ObjectComponent>("assets/table.png", 170, 130);
	start_room->AddObject(&table0, 750, 700);

	auto& table1(manager.addEntity());
	table1.addComponent<ObjectComponent>(&table0);
	start_room->AddObject(&table1, 1080, 920);

	auto& table2(manager.addEntity());
	table2.addComponent<ObjectComponent>(&table0);
	start_room->AddObject(&table2, 490, 920);

	auto& table3(manager.addEntity());
	table3.addComponent<ObjectComponent>(&table0);
	lockerRoom->AddObject(&table3, 430, 620);

	auto& table4(manager.addEntity());
	table4.addComponent<ObjectComponent>(&table0);
	lockerRoom->AddObject(&table4, 740, 800);

	auto& body0(manager.addEntity());
	body0.addComponent<ObjectComponent>("assets/dead1.png", 32, 32, 3);
	start_room->AddObject(&body0, 600, 600);

	auto& body1(manager.addEntity());
	body1.addComponent<ObjectComponent>("assets/dead2.png", 32, 32, 3);
	start_room->AddObject(&body1, 550, 650);

	auto& body2(manager.addEntity());
	body2.addComponent<ObjectComponent>(&body0);
	start_room->AddObject(&body2, 900, 900);

	auto& body3(manager.addEntity());
	body3.addComponent<ObjectComponent>(&body1);
	hallway_vent->AddObject(&body3, 900, 750);

	//vent
	auto& body4(manager.addEntity());
	body4.addComponent<ObjectComponent>(&body1);
	death_row->AddObject(&body4, 1100, 560);

	//other
	auto& plantv(manager.addEntity());
	plantv.addComponent<ObjectComponent>("assets/potted_plant2.png", 192, 192);
	hallwayI2v2->AddObject(&plantv, 915, 460);

	auto& bench0(manager.addEntity());
	bench0.addComponent<ObjectComponent>("assets/bench.png", 135, 50);
	hallwayT3_2->AddObject(&bench0, 290, 600);

	auto& bench1(manager.addEntity());
	bench1.addComponent<ObjectComponent>(&bench0);
	hallwayT3_3->AddObject(&bench1, 290, 600);

	auto& bench2(manager.addEntity());
	bench2.addComponent<ObjectComponent>(&bench0);
	hallwayT3_2->AddObject(&bench2, 910, 600);

	auto& bench3(manager.addEntity());
	bench3.addComponent<ObjectComponent>(&bench0);
	hallwayT3_3->AddObject(&bench3, 910, 600);

	auto& plant4(manager.addEntity());
	plant4.addComponent<ObjectComponent>(&plantv);
	hallwayT3_3->AddObject(&plant4, 1200, 460);
		
	escapeWall->addComponent<TransformComponent>(0, 0, 1085, 1460, 1);
	escapeWall->addComponent<SpriteComponent>("assets/ammoroom.png", true).playAnim("ammo1");
	escapeWall->addComponent<ObjectComponent>("assets/ammoroom.png", 0, 0);
	ammo->AddObject(escapeWall, 1500, 127);

	//An enviroment background made with a generic object
	auto& cellblock1(manager.addEntity());
	cellblock1.addComponent<ObjectComponent>("assets/cellblock.png", 1280, 448);
	start_room->AddObject(&cellblock1, 191, 128);

	//An environment background reusing an asset
	auto& cellblock2(manager.addEntity());
	cellblock2.addComponent<ObjectComponent>(&cellblock1);
	death_row->AddObject(&cellblock2, 896, 64);

	//toilets in showerroom
	auto& toilets(manager.addEntity());
	toilets.addComponent<ObjectComponent>("assets/toilets.png", 129, 858);
	shower_room->AddObject(&toilets, 1229, 425);
    
    //shower head
    auto& showerHead1(manager.addEntity());
    showerHead1.addComponent<ObjectComponent>("assets/showerHead.png", 128, 64);
    shower_room->AddObject(&showerHead1, 560, 250);
    
    //shower head
    auto& showerHead2(manager.addEntity());
    showerHead2.addComponent<ObjectComponent>("assets/showerHead.png", 128, 64);
    shower_room->AddObject(&showerHead2, 560, 640);
    
    //shower head
    auto& showerHead3(manager.addEntity());
    showerHead3.addComponent<ObjectComponent>("assets/showerHead.png", 128, 64);
    shower_room->AddObject(&showerHead3, 560, 1025);

	//receptionist desk in admin office
	auto& recepdesk(manager.addEntity());
	recepdesk.addComponent<ObjectComponent>("assets/receptiondesk.png", 450, 319);
	admin_office->AddObject(&recepdesk, 832, 662);

	//filing cabinets
	auto& wardenfilecabinet(manager.addEntity());
	wardenfilecabinet.addComponent<ObjectComponent>("assets/wardenfilingcabinet.png", 745, 160);
	admin_office->AddObject(&wardenfilecabinet, 500, 268);

	//corner desk in admin office
	auto& cornerdesk(manager.addEntity());
	cornerdesk.addComponent<ObjectComponent>("assets/cornerdesk.png", 225, 195);
	admin_office->AddObject(&cornerdesk, 1490, 384);
    
    //visitation stalls for prisoner room
    auto& visitation_stalls_p(manager.addEntity());
    visitation_stalls_p.addComponent<ObjectComponent>("assets/VisitorRoomStalls.png", 448, 192);
    visitationPrisonerSide->AddObject(&visitation_stalls_p, 704, 223);
    
    //washers and dryers
    auto& washers(manager.addEntity());
    washers.addComponent<ObjectComponent>("assets/washerDryers.png", 704, 128);
    laundry_room->AddObject(&washers, 672, 288);
    
    //library bookshelf
    auto& shelf(manager.addEntity());
    shelf.addComponent<ObjectComponent>("assets/LibraryBookshelf.png", 896, 128);
    library_room->AddObject(&shelf, 256, 256);
    
    //library plant
    auto& libPlant(manager.addEntity());
    libPlant.addComponent<ObjectComponent>("assets/potted_plant.png", 32, 60);
    libPlant.getComponent<TransformComponent>().scale = 2;
    library_room->AddObject(&libPlant, 256, 310);

	//warden desk
	auto& warden_desk(manager.addEntity());
	warden_desk.addComponent<ObjectComponent>("assets/wardendesk.png", 460, 315);
	warden->AddObject(&warden_desk, 540, 498);

	//turnstyle
	auto& turnstyle(manager.addEntity());
	turnstyle.addComponent<ObjectComponent>("assets/turnstyle.png", 220, 160);
	hallwayP1->AddObject(&turnstyle, 562, 200);

	//turnstyle2
	auto& turnstyle2(manager.addEntity());
	turnstyle2.addComponent<ObjectComponent>("assets/turnstyle.png", 220, 160);
	hallwayT2->AddObject(&turnstyle2, 562, 1200);
    
    // free side gated off hallway
    auto& fsGate(manager.addEntity());
    fsGate.addComponent<ObjectComponent>("assets/FreeSideGate.png", 256, 192);
    fsVisitation->AddObject(&fsGate, 832, 448);
    
    // free side visitor stalls
    auto& fsStalls(manager.addEntity());
    fsStalls.addComponent<ObjectComponent>("assets/freeSideStalls.png", 448, 192);
    fsVisitation->AddObject(&fsStalls, 704, 850);
    
    // free side plant decoration
    auto& fsPlant(manager.addEntity());
    fsPlant.addComponent<ObjectComponent>("assets/potted_plant2.png", 192, 192);
    fsVisitation->AddObject(&fsPlant, 240, 470);
    
    // filled laundry basket
    auto& basket1(manager.addEntity());
    basket1.addComponent<ObjectComponent>("assets/laundryBasket1.png", 192, 96);
    laundry_room->AddObject(&basket1, 1200, 550);
    
    // filled laundry basket
    auto& basket2(manager.addEntity());
    basket2.addComponent<ObjectComponent>("assets/laundryBasket2.png", 192, 96);
    laundry_room->AddObject(&basket2, 1200, 750);
    
    // empty laundry basket
    auto& basket3(manager.addEntity());
    basket3.addComponent<ObjectComponent>("assets/laundryBasketEmpty.png", 192, 96);
    laundry_room->AddObject(&basket3, 1200, 650);
    
    // laundry room table
    auto& laundryTable(manager.addEntity());
    laundryTable.addComponent<ObjectComponent>("assets/laundryTable.png", 288, 288);
    laundry_room->AddObject(&laundryTable, 860, 560);
    
	// lockers (all open)
	auto& lockers_open(manager.addEntity());
	lockers_open.addComponent<ObjectComponent>("assets/lockers_all_open.png", 290, 64);
	lockers_open.getComponent<TransformComponent>().scale = 2;
	lockerRoom->AddObject(&lockers_open, 383, 460);
    
    // hallway wet floor sign
    auto& wetSign(manager.addEntity());
    wetSign.addComponent<ObjectComponent>("assets/wetFloor.png", 96, 96);
    hallwayI1->AddObject(&wetSign, 560, 900);
    
    // hallway wet floor sign
    auto& wetSign2(manager.addEntity());
    wetSign2.addComponent<ObjectComponent>("assets/wetFloor.png", 96, 96);
    hallwayI2_3->AddObject(&wetSign2, 560, 685);
    
    // hallway janitor mop bucket
    auto& jannyMop(manager.addEntity());
    jannyMop.addComponent<ObjectComponent>("assets/janitorMop.png", 96, 96);
    hallwayI2_3->AddObject(&jannyMop, 1000, 590);

	auto& toolbag(manager.addEntity());
	toolbag.addComponent<ObjectComponent>("assets/toolbag.png", 100, 90);
	shower_room->AddObject(&toolbag, 700, 740);

	// ammo room background (default state is image 1)
	// TODOs

	auto& cratestack(manager.addEntity());
	cratestack.addComponent<ObjectComponent>("assets/crate_stack.png", 128, 128);
	hallwayT3_2->AddObject(&cratestack, 1200, 530);

	//       vvv   OBSTACLES / ITEMS   vvv


	//auto& metaldoor(manager.addEntity());
	metaldoor->addComponent<ObstacleComponent>(245, 195, 1, "assets/metaldoor2.PNG", "none");
	metaldoor->getComponent<ColliderComponent>().collider.h = 140;
	hallwayT2->AddObject(metaldoor, 582, 180);

	//auto& idscan(manager.addEntity());
	idscan->addComponent<ObstacleComponent>(48, 64, 1, "assets/idscan0.PNG", "warden_id");
	hallwayT2->AddObject(idscan, 820, 280);

	// fire extinguisher
	auto& fire_ext(manager.addEntity());
	fire_ext.addComponent<ItemComponent>(22, 32, 2, "assets/fire_extinguisher.png", "assets/pbCharAnim_fire_extinguisher.png", "fire_extinguisher");
	start_room->AddObject(&fire_ext, 350, 600);

	// fire
	auto& fire(manager.addEntity());
	fire.addComponent<TransformComponent>(0, 0, 27, 73, 3);  // remember width and height order is flipped for TransformComponent --dylan note: sorry!!!
	fire.addComponent<SpriteComponent>("assets/fire_anim.png", true).playAnim("Fire_Animation");
	fire.addComponent<ObstacleComponent>(73, 27, 3, "assets/fire_anim.png", "fire_extinguisher");
	start_room->AddObject(&fire, 1907, 550);

	// screwdriver
	auto& screwdriver(manager.addEntity());
	screwdriver.addComponent<ItemComponent>(32, 5, 2, "assets/screwdriver.png", "assets/pbCharAnim_screwdriver.png", "screwdriver");
	shower_room->AddObject(&screwdriver, 720, 830); // test line
	// shower_room->AddObject(&screwdriver, 740, 400); correct line

	// vent
	auto& vent(manager.addEntity());
	vent.addComponent<ObstacleComponent>(65, 431, 1, "assets/vent.png", "screwdriver");
	hallway_vent->AddObject(&vent, 1343, 209);

	// crowbar
	auto& crowbar(manager.addEntity());
	crowbar.addComponent<ItemComponent>(29, 32, 2, "assets/crowbar.png", "assets/pbCharAnim_crowbar.png", "crowbar");
	library_room->AddObject(&crowbar, 300, 400);

	// obstacle for crowbar
	auto& crate(manager.addEntity());
	crate.addComponent<ObstacleComponent>(32, 32, 2, "assets/crate.png", "crowbar");
	crate.getComponent<ColliderComponent>().collider.h = 10;
	hallwayT3_2->AddObject(&crate, 1250, 620);
	

	// guard outfit
	auto& guard_outfit(manager.addEntity());
	guard_outfit.addComponent<ItemComponent>(22, 30, 2, "assets/guard_outfit.png", "assets/pbCharAnim_guard_outfit.png", "guard_outfit");
	lockerRoom->AddObject(&guard_outfit, 714, 479);

	// locker door
	auto& locker_door(manager.addEntity());
	locker_door.addComponent<ObstacleComponent>(32, 64, 2, "assets/locker_door.png", "bolt_cutter");
	lockerRoom->AddObject(&locker_door, 707, 460);

	// knife
	auto& knife(manager.addEntity());
	knife.addComponent<ItemComponent>(32, 9, 2, "assets/knife.png", "assets/pbCharAnim_knife.png", "knife");
	death_row->AddObject(&knife, 1100, 550);

	// warden ID
	auto& warden_id(manager.addEntity());
	warden_id.addComponent<ItemComponent>(32, 23, 1, "assets/warden_id.png", "assets/pbCharAnim_warden_id.png", "warden_id");
	warden->AddObject(&warden_id, 900, 575);

	// metal door

	// warden key
	auto& ward_key(manager.addEntity());
	ward_key.addComponent<ItemComponent>(32, 19, 2, "assets/key_gold.png", "assets/pbCharAnim_key_gold.png", "ward_key");
	laundry_room->AddObject(&ward_key, 1020, 720);

	// warden door
	auto& ward_door(manager.addEntity());
	ward_door.addComponent<ObstacleComponent>(245, 1070, 1, "assets/locked_door_gold.png", "ward_key");
	admin_office->AddObject(&ward_door, 1285, -698);

	// visitation key
	auto& visitor_key(manager.addEntity());
	visitor_key.addComponent<ItemComponent>(32, 19, 2, "assets/key_silver.png", "assets/pbCharAnim_key_silver.png", "visitor_key");
	lockerRoom->AddObject(&visitor_key, 860, 920);

	// visitation door
	auto& visit_door(manager.addEntity());
	visit_door.addComponent<ObstacleComponent>(160, 1070, 1, "assets/locked_door_silver.png", "visitor_key");
	visitationPrisonerSide->AddObject(&visit_door, 240, -730);

	// money will add player anim for it later
	auto& money(manager.addEntity());
	money.addComponent<ItemComponent>(31, 19, 1, "assets/money.png", "assets/pbCharAnim_money.png", "money");
	lockerRoom->AddObject(&money, 460, 640);  // will place in right location later, test line

	// vending machine (have to figure out how to not destroy it when using dollar on it)
	auto& vending_machine(manager.addEntity());
	vending_machine.addComponent<ObstacleComponent>(64, 109, 1, "assets/vending_machine.png", "money");
	vending_machine.getComponent<ColliderComponent>().collider.h = 60; //make vending shorter
	hallwayI2v2->AddObject(&vending_machine, 850, 530);

	// lighter
	auto& lighter(manager.addEntity());
	lighter.addComponent<ItemComponent>(23, 32, 2, "assets/lighter.png", "assets/pbCharAnim_lighter.png", "lighter");
	fsVisitation->AddObject(&lighter, 600, 620);

	//rope
	auto& rope(manager.addEntity());
	rope.addComponent<ItemComponent>(27, 32, 2, "assets/rope.png", "assets/pbCharAnim_rope.png", "rope");
	ammo->AddObject(&rope, 460, 640);  // will place in right location later, test line

	//endgame barrels n rope n crates
	//auto& barrels(manager.addEntity());
	boomWall->addComponent<ObjectComponent>("assets/boomwall.png", 544, 200, 2);
	ammo->AddObject(boomWall, 690, 30);
	barrels->addComponent<ObstacleComponent>(413, 510, 1, "assets/explosivebarrels2.png", "rope");
	ammo->AddObject(barrels, 1590, 400);
	//rope
	rope_obs->addComponent<TransformComponent>(0, 0, 32, 600, 1);
	rope_obs->addComponent<SpriteComponent>("assets/rope_obs.png", true).playAnim("rope_idle");
	rope_obs->addComponent<ObstacleComponent>(600, 32, 1, "assets/rope_obs.png", "lighter");
	rope_obs->getComponent<ColliderComponent>().collider.w = 20;

	//*************************^^^ ADD OBJECTS HERE ^^^*************************//


	//ECS PLAYER implementation: Tranform adds ability to transform (scale, move) and Sprite adds visual representation
	player->addComponent<TransformComponent>(700, 700, 32, 32, 3);
	// player.addComponent<SpriteComponent>("assets/sus.png"); // orig sus test player (legacy)
	player->addComponent<SpriteComponent>("assets/pbCharAnim.png", true); // animated prisoner
	player->addComponent<KeyboardController>();
	player->addComponent<ColliderComponent>("player");
	player->addGroup(groupPlayers);

	camera.w = currentRoom->x() * 32;
	camera.h = currentRoom->y() * 32;
	roomManager.LoadRoom(currentRoom);
	currentNavmesh = roomManager.getNavmesh();
    
    pause_rect.x = 300;
    pause_rect.y = 200;
    pause_rect.w = 424;
    pause_rect.h = 176;
    
    win_rect.x = 260;
    win_rect.y = 200;
    win_rect.w = 488;
    win_rect.h = 168;
    
    over_rect.x = 200;
    over_rect.y = 200;
    over_rect.w = 584;
    over_rect.h = 184;
}



void Game::handleEvents() {

	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
        forceQuit = true;
		break;

	case SDL_KEYUP:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
            if (!game_over && !win) {
                if (!paused) {
                    pauseIcon = IMG_LoadTexture(renderer, "assets/pauseLogo.png");
                    paused = true;


                }
                else {
                    SDL_DestroyTexture(pauseIcon);
                    pauseIcon = nullptr;
                    paused = false;
                }
            }
            else {
                isRunning = false;
            }
            break;
			
		}

	default:
		break;
	}
}

void Game::update() {
	if (!paused && !game_over && !win) {

		//check if "interact" button or "switch item" button pressed
		bool e_pressed = false;
		bool q_pressed = false;
		if (Game::event.type == SDL_KEYDOWN) {

			if (Game::event.key.keysym.sym == SDLK_e)
				e_pressed = true;

			else if (Game::event.key.keysym.sym == SDLK_q)
				q_pressed = true;

		}


		//setup get player's hitbox and position before entity update
		SDL_Rect playerHitbox = player->getComponent<ColliderComponent>().collider;
		Vector2D playerPos = player->getComponent<TransformComponent>().position;

		//entity manager
		manager.refresh();
		manager.update();


		NPCComponent* npc;
		TransformComponent* transform;
		Node* src;
		Node* dest;
		for (auto& n : *npcs) {

			npc = &n->getComponent<NPCComponent>();
			transform = &n->getComponent<TransformComponent>();

			//if player shot, end game
			if (npc->shoot == 2) {


				if (abs(playerPos.y - transform->position.y) < 150 &&
					abs(playerPos.x - transform->position.x) < 150)
				{
					game_over = true;
				}
				else {
					npc->shoot = 0;
				}
			}

			//if npc has no path, provide a random one after a random amount of time
			if (npc->path.size() == 0) {

				if ((rand() % 100) > 90) {

					if (npc->spotted) {
						src = currentNavmesh->closestNode(transform->position.x, transform->position.y);
						dest = currentNavmesh->closestNode(playerPos.x, playerPos.y);
						transform->speed = rand() % 1 + 2;
					}
					else {
						src = currentNavmesh->closestNode(transform->position.x, transform->position.y);
						dest = currentNavmesh->randomNode();
						transform->speed = rand() % 2 + 1;
					}

					npc->path = currentNavmesh->shortestPath(src->id, dest->id);
				}
			}

			std::cout << "NPC DIRECTION: " << npc->last_direction << std::endl;

			//player spotted, update path
			if (!disguise && ((npc->last_direction == 'w' && playerPos.y < transform->position.y && abs(playerPos.x - transform->position.x) < 250 && abs(playerPos.y - transform->position.y) < 500) ||
				(npc->last_direction == 's' && playerPos.y > transform->position.y && abs(playerPos.x - transform->position.x) < 250 && abs(playerPos.y - transform->position.y) < 500) ||
				(npc->last_direction == 'a' && playerPos.x < transform->position.x && abs(playerPos.y - transform->position.y) < 250 && abs(playerPos.x - transform->position.x) < 500) ||
				(npc->last_direction == 'd' && playerPos.x > transform->position.x && abs(playerPos.y - transform->position.y) < 250 && abs(playerPos.x - transform->position.x) < 500)))
			{

				std::cout << "PLAYER DETECTED" << std::endl;

				//shoot player
				if (!disguise && abs(playerPos.y - transform->position.y) < 100 &&
					abs(playerPos.x - transform->position.x) < 100) 
				{
					npc->shoot = 1;
				}

				if (npc->spotted == false && npc->timer >= 50) {
					npc->path.clear();
					std::cout << "NPC PATH RESET TO PLAYER DEST" << std::endl;
					npc->timer = 0;
				}
				else {
					std::cout << "FOLLOWING PLAYER" << std::endl;
				}

				npc->spotted = true;
			}
			else {
				std::cout << "PLAYER NOT FOUND" << std::endl;
				npc->spotted = false;
			}

			if (e_pressed && currentItem != nullptr && 
				!currentItem->getComponent<ItemComponent>().getName().compare("knife") && 
				abs(playerPos.y - transform->position.y) < 100 &&
				abs(playerPos.x - transform->position.x) < 100)
			{

				auto& dead_cop(manager.addEntity());
				dead_cop.addComponent<TransformComponent>(0,0,32,32,3);
				dead_cop.addComponent<SpriteComponent>("assets/deadcop.png");
				dead_cop.addComponent<ObjectComponent>("assets/deadcop.png", 32, 32);
				currentRoom->AddObject(&dead_cop, transform->position.x, transform->position.y);

				currentRoom->DeleteObject(n);
				n->delGroup(Game::groupNPCs);
				n->delGroup(Game::groupObjects);
				n->destroy();

				currentRoom->Load();
			}
			
		}

		for (auto& c : *colliders) {
			std::string tag = c->getComponent<ColliderComponent>().tag;
			SDL_Rect col = c->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(playerHitbox, col)) {


				if (!tag.compare("environment")) {
					if (Collision::CollisionDirection(playerHitbox, col) == 0) {
						player->getComponent<TransformComponent>().position.x = playerPos.x;
					}
					if (Collision::CollisionDirection(playerHitbox, col) == 1) {
						player->getComponent<TransformComponent>().position.y = playerPos.y;
					}
				}

				else if (!tag.compare("obstacle")) {

					//interact with obstacle
					if (e_pressed) {

						//if holding key item..
						if (currentItem != nullptr && !currentItem->getComponent<ItemComponent>().getName().compare(c->getComponent<ObstacleComponent>().getKey())) {

							// VENT AFTER USING SCREWDRIVER ON IT
							if (c->getComponent<ObstacleComponent>().getKey().compare("screwdriver") == 0) {
								//delete obstacle
								currentRoom->DeleteObject(c);
								c->delGroup(Game::groupColliders);
								c->delGroup(Game::groupObjects);
								c->destroy();

								// spawn in detached vent lying down
								auto& vent_detached(manager.addEntity());
								vent_detached.addComponent<ObjectComponent>("assets/vent_detached.png", 65, 582);
								currentRoom->AddObject(&vent_detached, 1343, 209);
								currentRoom->Load(); // reload new object into room
							}
							//MONEY FOR VENDING MACHINE
							else if (c->getComponent<ObstacleComponent>().getKey().compare("money") == 0) {
								// spawn in energy drink
								auto& energy_drink(manager.addEntity());
								energy_drink.addComponent<ItemComponent>(20, 32, 1, "assets/energy_drink.png", "assets/pbCharAnim_energy_drink.png", "energy_drink");
								currentRoom->AddObject(&energy_drink, 950, 620);
								currentRoom->Load(); // reload new item into room
							}
							//KEY CARD SCANNER
							else if (c->getComponent<ObstacleComponent>().getKey().compare("warden_id") == 0) {
								c->getComponent<SpriteComponent>().setTexture("assets/idscan2.png");
								metalDoorOpen = true;
							}
							//CRATE
							else if (c->getComponent<ObstacleComponent>().getKey().compare("crowbar") == 0) {
								//delete obstacle
								currentRoom->DeleteObject(c);
								c->delGroup(Game::groupColliders);
								c->delGroup(Game::groupObjects);
								c->destroy();
								
								auto& bolt_cutter(manager.addEntity());
								bolt_cutter.addComponent<ItemComponent>(14, 31, 2, "assets/bolt_cutter.png", "assets/pbCharAnim_bolt_cutter.png", "bolt_cutter");
								currentRoom->AddObject(&bolt_cutter, 1250, 620);
								currentRoom->Load(); // reload new item into room
							}
							
							//SOME OTHER EXAMPLE
							else if (c->getComponent<ObstacleComponent>().getKey().compare("rope") == 0) {
								currentRoom->AddObject(rope_obs, 1000, 650);
								currentRoom->Load();
							}

							else if (c->getComponent<ObstacleComponent>().getKey().compare("lighter") == 0) {
								
								escape = true;

							}
							//DEFAULT: DESTROY OBSTACLE
							else {
								//delete obstacle
								currentRoom->DeleteObject(c);
								c->delGroup(Game::groupColliders);
								c->delGroup(Game::groupObjects);
								c->destroy();
							}

							//delete current item

							auto it = std::find(inventory.begin(), inventory.end(), currentItem);
							if (it != inventory.end()) { inventory.erase(it); }

							currentRoom->DeleteObject(currentItem);
							currentItem->delGroup(Game::groupColliders);
							currentItem->delGroup(Game::groupObjects);
							currentItem->destroy();

							//switch to another item in inventory if available
							currentIndex--;
							if (inventory.size() > 0 && currentIndex < 0) currentIndex = inventory.size() - 1;
							if (currentIndex >= 0) currentItem = inventory.at(currentIndex);
							else currentItem = nullptr;
								
						  
						e_pressed = false;

							
						}
						else {
							if (!c->getComponent<ObstacleComponent>().getKey().compare("warden_id")) {
								idscan->getComponent<SpriteComponent>().setTexture("assets/idscan1.png");
							}
						}
				  }
				
				  player->getComponent<TransformComponent>().position = playerPos;
			  }

			  else if (!tag.compare("item")) {

				  //interact with item
				  if (e_pressed) {
					  currentItem = c; //set current item
					  currentIndex = inventory.size(); //get its index
					  inventory.push_back(currentItem); //add to inventory

					  currentRoom->DeleteObject(currentItem);//remove object from room
					  currentItem->delGroup(Game::groupObjects);//remove from render group
					  currentItem->delGroup(Game::groupColliders);

					  //c->getComponent<TransformComponent>().position = Vector2D(-100, -100); //remove from screen

					  e_pressed = false;
				  }
			  }


			  else if (!preventChange) {
				  currentRoom = map.changeRoom(currentRoom, tag.at(0)); //get current room after a room change from the map manager
                  preventChange = true;
				  roomManager.LoadRoom(currentRoom); //load and process the room data into the game loop
				  currentNavmesh = roomManager.getNavmesh(); //get the newly created navmesh the room manager loaded from the room
				  camera.w = (currentRoom->x() * 32);
				  camera.h = (currentRoom->y() * 32);
				  switch (tag.at(0)) {
				  case 'e':
					  player->getComponent<TransformComponent>().position = currentRoom->west;
					  player->getComponent<TransformComponent>().position.x += TILE_SIZE * 4 + 16;
					  player->getComponent<TransformComponent>().position.y += TILE_SIZE * 1 + 16;
					  break;
				  case 'n':
					  player->getComponent<TransformComponent>().position = currentRoom->south;
					  player->getComponent<TransformComponent>().position.y -= TILE_SIZE * 4 + 16;
					  player->getComponent<TransformComponent>().position.x += TILE_SIZE * 1 + 16;
					  break;
				  case 'w':
					  player->getComponent<TransformComponent>().position = currentRoom->east;
					  player->getComponent<TransformComponent>().position.x -= TILE_SIZE * 4 + 16;
					  player->getComponent<TransformComponent>().position.y += TILE_SIZE * 1 + 16;
					  break;
				  case 's':
					  player->getComponent<TransformComponent>().position = currentRoom->north;
					  player->getComponent<TransformComponent>().position.y += TILE_SIZE * 4 + 16;
					  player->getComponent<TransformComponent>().position.x += TILE_SIZE * 1 + 16;
					  break;
				  default:
					  //none
					  break;
				  }
				  break;
			  }
              else {
                  preventChange = false;
              }
            

		  }
	  }

	  //switch item
	  if (q_pressed && inventory.size() >= 2) {
		  currentIndex++;
		  if (currentIndex == inventory.size()) currentIndex = 0;

		  currentItem = inventory.at(currentIndex);
		  q_pressed = false;
	  }

	  //drop item with e (REMOVED)
	  //if (e_pressed && currentIndex > -1) {
		//  currentRoom->AddObject(currentItem, 0, 0);
		//  currentRoom->Load();
		//  currentItem->getComponent<TransformComponent>().position = playerPos;
		  
		//  auto it = std::find(inventory.begin(), inventory.end(), currentItem);
		//  if (it != inventory.end()) { inventory.erase(it); }

		//  currentIndex--;
		//  if (inventory.size() > 0 && currentIndex < 0) currentIndex = inventory.size() - 1;
		//  if (currentIndex >= 0) currentItem = inventory.at(currentIndex);
		//  else currentItem = nullptr;
	  //}
      
	  //energy
	  if (e_pressed && currentItem != nullptr && !currentItem->getComponent<ItemComponent>().getName().compare("energy_drink")) {
		  player->getComponent<TransformComponent>().speed = 5;
		  //delete current item
		  auto it = std::find(inventory.begin(), inventory.end(), currentItem);
		  if (it != inventory.end()) { inventory.erase(it); }

		  currentRoom->DeleteObject(currentItem);
		  currentItem->delGroup(Game::groupColliders);
		  currentItem->delGroup(Game::groupObjects);
		  currentItem->destroy();

		  //switch to another item in inventory if available
		  currentIndex--;
		  if (inventory.size() > 0 && currentIndex < 0) currentIndex = inventory.size() - 1;
		  if (currentIndex >= 0) currentItem = inventory.at(currentIndex);
		  else currentItem = nullptr;
	  }

	  //show item player is holding
	  if (inventory.size() == 0) {
		  player->getComponent<SpriteComponent>().setTexture("assets/pbCharAnim.png");
	  }
	  else {
		  player->getComponent<SpriteComponent>().setTexture(currentItem->getComponent<ItemComponent>().getCharacterModel());
	  }

	  //update id scanner
	  if (strcmp(idscan->getComponent<SpriteComponent>().file, "assets/idscan0.png")) {
		  timer++;
		  if (timer > 100) {
			  idscan->getComponent<SpriteComponent>().setTexture("assets/idscan0.png");
			  timer = 0;
		  }
	  }
	  //update metal door if opened
	  if (metalDoorOpen && metaldoor->getComponent<TransformComponent>().position.y > -100) {
		  metaldoor->getComponent<TransformComponent>().velocity.y = -1;
	  }

	  //is disguised
	  if (currentItem != nullptr && !currentItem->getComponent<ItemComponent>().getName().compare("guard_outfit")) {
		  disguise = true;
	  }
	  else {
		  disguise = false;
	  }

	  //if blow up barrels
	  if (escape) {
		  rope_obs->getComponent<SpriteComponent>().playAnim("rope_burn");
		  rope_obs->getComponent<TransformComponent>().velocity.x = 1;
		  rope_obs->getComponent<TransformComponent>().speed = 10;
		  escapeWall->getComponent<SpriteComponent>().playAnim("ammo2");
		  barrels->getComponent<TransformComponent>().position.y = 2000;
		  currentRoom->Load();
	  }

	  if (!strcmp(currentRoom->getId(), "end") && playerPos.x > 1620) {
		  win = true;
	  }
      
    //update camera position
    camera.x = player->getComponent<TransformComponent>().position.x - 450;
    camera.y = player->getComponent<TransformComponent>().position.y - 280;
	if (camera.x < (TILE_SIZE * 3)) camera.x = (TILE_SIZE * 3);
    if (camera.y < (TILE_SIZE * 3)) camera.y = (TILE_SIZE * 3);
    if (camera.x > camera.w - (TILE_SIZE * 5)) camera.x = camera.w - (TILE_SIZE * 5);
    if (camera.y > camera.h - (TILE_SIZE * 1)) camera.y = camera.h - (TILE_SIZE * 1);

  }
    else if (game_over) {
        if (loseIcon == nullptr) {
            loseIcon = IMG_LoadTexture(renderer, "assets/gameOverLogo.png");
        }
    }
    else {
        if (winIcon == nullptr && win) {
            winIcon = IMG_LoadTexture(renderer, "assets/winLogo.png");
        }
    }
}

void Game::render() {
	SDL_RenderClear(renderer);

	// vvv STUFF TO RENDER (IN ORDER DEEPEST TO SHALLOWEST) vvv //
    
	//Room tiles
	for (auto& t : *tiles) {
		t->draw();
	}

	//objects
	for (auto& o : *objects) {
		o->draw();
	}

	//NPCs
	for (auto& n : *npcs) {
		n->draw();
	}

	//Players
	for (auto& p : *players) {
		p->draw();
	}
    
    if (paused) {
        SDL_RenderCopy(renderer, pauseIcon, NULL, &pause_rect);
    }
    else if (game_over) {
        SDL_RenderCopy(renderer, loseIcon, NULL, &over_rect);
    }
    else if (win) {
        SDL_RenderCopy(renderer, winIcon, NULL, &win_rect);
	}
	else {

	}
    
	// ^^^ STUFF TO RENDER (IN ORDER DEEPEST TO SHALLOWEST) ^^^ //

	SDL_RenderPresent(renderer);
}

void Game::clean() {
    if (win) {
        SDL_DestroyTexture(winIcon);
        winIcon = nullptr;
        win = false;
    }
    if (game_over) {
        SDL_DestroyTexture(loseIcon);
        loseIcon = nullptr;
        game_over = false;
    }
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
    manager.reset();
	std::cout << "Game cleaned." << std::endl;
}



