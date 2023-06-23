#pragma once
#ifndef ZOMBIES_H 
#define ZOMBIES_H

#include "raylib.h"
#include "raymath.h"
#include "zombie.h"

#include <iostream>
#include <vector>


#define MAX_ZOMBIES 50


class Zombies {

public:
	Zombies(Camera& camera);
	~Zombies();
	void reset();
	void drawZombies();
	void updateZombies();
	void zombieCollision(int i);
	void spawnZombie();
	std::vector<Zombie*> getZombieArray();
private:
	int zombieSpawnTimer = 0;
	int activeZombieCount = 10;
	Camera &playerCamera;
	std::vector<Zombie*> zombieArray;
	
	Sound zombieDeathSound;
};


#endif // !ZOMBIES_H 
