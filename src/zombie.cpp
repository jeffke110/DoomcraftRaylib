#include "zombie.h"

Zombie::Zombie(Camera& camera)
	: playerCamera(camera),
	zombieModel(LoadModel("resources/textures/zombie.glb"))
{
	float x, z;
	if (playerCamera.position.x > 0) {
		x = (float)GetRandomValue(-14, 0);
	}
	else {
		x = (float)GetRandomValue(0, 14);
	}
	if (playerCamera.position.z > 0) {
		z = (float)GetRandomValue(-4, 0);
	}
	else {
		z = (float)GetRandomValue(0, 20);
	}

	position = Vector3{ x, 0.0f, z };


	moveSpeed = 0.01f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (0.05f - 0.01f));
	prevposition = position;
	colllidngPosition = position;

	hitBox = {
		position.x - .5f, position.y, position.z - .5f,
			position.x + .5f, position.y + 1.7f, position.z + .5f
	};
}

//reset zombies for game reset
void Zombie::reset() {
	float x, z;
	if (playerCamera.position.x > 0) {
		x = (float)GetRandomValue(-14, 0);
	}
	else {
		x = (float)GetRandomValue(0, 14);
	}
	if (playerCamera.position.z > 0) {
		z = (float)GetRandomValue(-4, 0);
	}
	else {
		z = (float)GetRandomValue(0, 20);
	}

	position = Vector3{ x, 0.0f, z };


	moveSpeed = 0.01f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (0.05f - 0.01f));
	prevposition = position;
	colllidngPosition = position;

	hitBox = {
		position.x - .5f, position.y, position.z - .5f,
			position.x + .5f, position.y + 1.7f, position.z + .5f
	};
	isCollidingZombie = false;
    health = 5.0f;
	active = true;
	isJumping = false;
	onBox = false;
	jumpSpeed = 0.0f;
}


//active zombies
void Zombie::activate() {
	float x, z;
	if (playerCamera.position.x > 0) {
		x = (float)GetRandomValue(-14, 0);
	}
	else {
		x = (float)GetRandomValue(0, 14);
	}
	if (playerCamera.position.z > 0) {
		z = (float)GetRandomValue(-4, 0);
	}
	else {
		z = (float)GetRandomValue(0, 20);
	}

	position = Vector3{ x, 0.0f, z };
	health = 5.0f;
	moveSpeed = 0.01f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (0.05f - 0.01f));
	prevposition = position;
	colllidngPosition = position;
	active = true;
	hitBox = {
		position.x - .5f, position.y, position.z - .5f,
			position.x + .5f, position.y + 1.7f, position.z + .5f
	};
}


Zombie::~Zombie() {
	UnloadModel(zombieModel);
}

// Calculate the direction from the zombie's position to the camera position
void Zombie::drawZombie() {
	Vector3 directionToCamera = Vector3Normalize(Vector3Subtract(playerCamera.position, position));
	float rotationAngle = atan2f(directionToCamera.x, directionToCamera.z) * RAD2DEG + 180;
	Vector3 modelScale = { 1.0f, 1.0f, 1.0f };
	DrawModelEx(zombieModel, position, { 0.0f, 1.0f, 0.0f }, rotationAngle, modelScale, WHITE);
}


//update the zombie's position
void Zombie::updateZombie() {
		prevposition = position;
		//is zombie colliding with other zombie
		if (isCollidingZombie) {
			if (position.x < colllidngPosition.x) {
				position.x += moveSpeed;
			}
			if (position.x > colllidngPosition.x) {
				position.x -= moveSpeed;
			}
			if (position.z < colllidngPosition.z) {
				position.z += moveSpeed;
			}
			if (position.z > colllidngPosition.z) {
				position.z -= moveSpeed;
			}
		}
		else {
			//check for jumping and change position if jumping
			if (!isJumping) {
				if (position.x < playerCamera.position.x) {
					position.x += moveSpeed;
				}
				if (position.x > playerCamera.position.x) {
					position.x -= moveSpeed;
				}
				if (position.z < playerCamera.position.z) {
					position.z += moveSpeed;
				}
				if (position.z > playerCamera.position.z) {
					position.z -= moveSpeed;
				}
			}
			else {
				if (position.x < playerCamera.position.x) {
					position.x += moveSpeed;
				}
				if (position.x > playerCamera.position.x) {
					position.x -= moveSpeed;
				}
				if (position.z < playerCamera.position.z) {
					position.z += moveSpeed;
				}
				if (position.z > playerCamera.position.z) {
					position.z -= moveSpeed;
				}
			}
		}
		hitBox = {
			position.x - .5f, position.y, position.z - .5f,
				position.x + .5f, position.y + 1.7f, position.z + .5f
		};
		checkJump();
}
//have the zombie jump on to platforms
void Zombie::checkJump() {
	if (isJumping)
	{
		position.y += jumpSpeed;
		// Apply gravity over time
		jumpSpeed -= gravity * GetFrameTime();
		if (position.y <= 0.0f)
		{
			position.y = 0.0f;
			isJumping = false;
		}
	}

}


//getters and setters
bool Zombie::getOnBox() {
	return onBox;
}
void Zombie::setOnBox(bool input) {
	onBox = input;
}
void Zombie::setJumping(bool input) {
	isJumping = input;
}
void Zombie::setJumpSpeed(float input) {
	jumpSpeed = input;
}
Vector3 Zombie::getPosition() {
	return position;
}
struct BoundingBox Zombie::getHitBox() {
	return hitBox;
}
void Zombie::setCollidingZombie(bool input) {
	isCollidingZombie = input;
}
bool Zombie::getCollidingZombie() {
	return isCollidingZombie;
}
float Zombie::getHealth() const {
	return health;
}
void Zombie::setHealth(float newHealth) {
	health = newHealth;
}
bool Zombie::isActive() const {
	return active;
}
void Zombie::setActive(bool newActive) {
	active = newActive;
}
Vector3 Zombie::getPosition() const {
	return position;
}
void Zombie::setPosition(const Vector3& newPosition) {
	position = newPosition;
}
Vector3 Zombie::getPrevPosition() const {
	return prevposition;
}
void Zombie::setPrevPosition(const Vector3& newPrevPosition) {
	prevposition = newPrevPosition;
}
Vector3 Zombie::getCollidingPosition() const {
	return colllidngPosition;
}
void Zombie::setCollidingPosition(const Vector3& newCollidingPosition) {
	colllidngPosition = newCollidingPosition;
}
