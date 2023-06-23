#pragma once
#ifndef GHAST_H
#define GHAST_H

#include "raylib.h"
#include "raymath.h"
#include "bullet.h"
#include <random>


class Ghast {

public:
	Ghast(Camera &camera);
	~Ghast();
	void reset();

	//update and draw
	void drawGhast();
	void updateGhast();

	//health
	float getHealth() const;
	void setHealth(float newHealth);

	//collision
	struct BoundingBox getHitBox();
	bool getCollidingGhast();
	void setCollidingGhast(bool input);
	
	//active
	void activate();
	bool isActive() const;
	void setActive(bool newActive);

	//bullet methods
	void initBullets();
	void updateBullets();
	void ShootBullet();
	void drawBullets();
	int getBulletCount();
	Bullet* getBullets();

	//position methods
	Vector3 getPosition();
	Vector3 getCollidingPosition() const;
	void setCollidingPosition(const Vector3& newCollidingPosition);


private:
	
	//Camera and position
	Camera & playerCamera;
	Model ghastModel;
	Vector3 position;
	Vector3 colllidngPosition;
	struct BoundingBox hitBox;
	bool isCollidingGhast = false;
	float moveSpeed = 0.025f;

	//health and active
	float health = 10.0f;
	bool active = true;

	//Bullets
	Bullet bullets[MAX_BULLETS];
	Texture2D fireballTexture;
	Model fireballModel;
	Mesh sphereMesh;
	int bulletShootTimer = 0;
	int bulletCount = 0;

	//Sound
	Sound ghastBulletSound;

};



#endif // !GHAST_H
