#pragma once
#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "raylib.h"
#include "raymath.h"
#include <random>
#include <iostream>

class Zombie {
public:
	Zombie(Camera &camera);
	~Zombie();
	void reset();
	Vector3 getPosition();


	//Update Zombie
	void drawZombie();
	void updateZombie();

	//Health
	float getHealth() const;
	void setHealth(float newHealth);

	//Jump
	void checkJump();
	bool getOnBox();
	void setOnBox(bool input);
	void setJumping(bool input);
	void setJumpSpeed(float input);

	//Collision
	struct BoundingBox getHitBox();
	bool getCollidingZombie();
	void setCollidingZombie(bool input);

	//Activate
	void activate();
	bool isActive() const;
	void setActive(bool newActive);

	//Position
	Vector3 getPosition() const;
	void setPosition(const Vector3& newPosition);
	Vector3 getPrevPosition() const;
	void setPrevPosition(const Vector3& newPrevPosition);
	Vector3 getCollidingPosition() const;
	void setCollidingPosition(const Vector3& newCollidingPosition);

private:

	//Camera and Position
	Camera &playerCamera;
	Model zombieModel;
	Vector3 position;
	Vector3 prevposition;
	Vector3 colllidngPosition;
	struct BoundingBox hitBox;
	bool isCollidingZombie = false;
	float moveSpeed;

	//health and active
	float health = 5.0f;
	bool active = true;


	//Jumping
	bool isJumping = false;
	bool onBox = false;
	float jumpSpeed = 0.0f;
	float gravity = .098f;  
};


#endif // !ZOMBIE_H
