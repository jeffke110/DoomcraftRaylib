#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "rcamera.h"

#include <iostream>

class Player {

public:
	Player();
	~Player();
	void reset();

	Camera * getCamera();
	void setCamera(Camera * camera);

	Camera getPrevCamera();
	void setPrevCamera(Camera  camera);

	void drawCameraInfo();
	void updateCameraPosition();
	void drawJetPackInfo();
	struct BoundingBox getHitBox();

	void checkJump();
	void jetPack();
	bool getOnBox();
	void setOnBox(bool input);
	void setJumping(bool input);
	void setJumpSpeed(float input);

	void setHealth(float input);
	float getHealth();
	
	

private:

	//General
	float health = 100.0f;
	Vector3 position;
	Camera* camera;
	Camera prevCamera;

	int cameraMode;
	struct BoundingBox hitBox;

	//jet pack
	Sound jetPackSoundEffect;
	bool jetPackOn = false;
	bool fuelRegen = false;
	int fuel = 400;

	//jumping
	float jumpSpeed = 0.0f;
	float gravity = .098f;  
	float jumpForce = .10f;
	bool isJumping = false;
	bool onBox = false;

	//interface
	Texture2D healthBar;
	Texture2D fuelBar;
	Texture2D fuelFont;

};


#endif // PLAYER_H
