#pragma once
#ifndef GHASTS_H
#define GHASTS_H

#include "raylib.h"
#include "raymath.h"
#include "ghast.h"

#include <iostream>
#include <vector>


#define MAX_GHASTS 20


class Ghasts {

public:
	Ghasts(Camera& camera);
	~Ghasts();

	void reset();
	void drawGhasts();
	void updateGhasts();
	void ghastCollision(int i);
	void spawnGhast();
	std::vector<Ghast*> getGhastArray();

	
private:
	int ghastSpawnTimer = 0;
	int activGhastCount = 10;
	Camera& playerCamera;
	std::vector<Ghast*> ghastArray;
	Sound ghastDeathSound;






};



#endif // !GHASTS_H
