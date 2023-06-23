#pragma once
#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "rcamera.h"
#include "rlgl.h" 


#include "texture.h"
#include "player.h"
#include "bullet.h"
#include "ghast.h"
#include "ghasts.h"
#include "zombie.h"
#include "zombies.h"

#include <vector>
#include <algorithm>
#include <iostream>


struct Platform {
	Vector3 position;
	const float width = 10.0f;
	const float height = 2.0f;
	const float length = 10.0f;
	struct BoundingBox hitBox;
};

enum class GameState
{
	MainMenu,
	PlayGame,
	Instructions,
	GameOver
};

class Game {

public:
	Game();
	~Game();

	//setup methods
	void draw();
	void setPlatforms();
	void run();
	void reset();
	void unload();

	//menu control methods
	void gameOver();
	void playGame();
	void drawMainMenu();
	void drawInstructionsMenu();

	//collision methods
	void checkWallCollision();
	void playerPlatformCollision();
	void zombiePlatformCollision(int i);
	void zombieBulletCollision(int i);
	void ghastBulletCollision(int i);

	//update methods
	void ghastsUpdate();
	void zombiesUpdate();
	void playerUpdate();

private:

	const int screenWidth = 800;
	const int screenHeight = 600;

	//character objects and platforms
	Player* player;
	Ghasts* ghasts;
	Zombies* zombies;
	Textures* textureControl;
	Bullets * bulletControl;
	std::vector<Platform> platforms;

	//Sounds and textures
	Sound playerSoundEffect;
	Sound gunShotSoundEffect;
	Sound ghastSoundEffect;
	Sound zombieSoundEffect;
	Sound backgroundMusic;
	Texture2D mainMenuTexture;
	Texture2D instructionsMenuTexture;

	//Menu state
	GameState currentState = GameState::MainMenu;

	//Timing variables
	int zombieTimeCollision = 0;
	int ghastTimeCollision = 0;
	int ghastTimeBulletCollision = 0;


};
#endif // !GAME_H
