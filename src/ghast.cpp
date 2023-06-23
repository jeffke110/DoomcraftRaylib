#include "ghast.h"


Ghast:: Ghast(Camera &Camera) : playerCamera(Camera){
	ghastModel = LoadModel("resources/textures/ghast.glb");
	fireballTexture = LoadTexture("resources/textures/Fireball.png");

	sphereMesh = GenMeshSphere(.5f, 32, 32);
	fireballModel = LoadModelFromMesh(sphereMesh);
	fireballModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = fireballTexture;

	ghastBulletSound = LoadSound("resources/sounds/GhastBulletSound.wav");
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

	position = Vector3{ x, 5.0f, z };

	colllidngPosition = position;
	hitBox = {
	position.x - 1.0f, position.y - 1.0f, position.z - 1.0f,
		position.x + 1.0f, position.y + 1.0f, position.z + 1.0f
	};
	initBullets();
}

Ghast::~Ghast() {
	UnloadModel(ghastModel);
	UnloadSound(ghastBulletSound);
	UnloadTexture(fireballTexture);
}

void Ghast::reset() {

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
	position = Vector3{ x, 5.0f, z };
	colllidngPosition = position;
	hitBox = {
	position.x - 1.0f, position.y - 1.0f, position.z - 1.0f,
		position.x + 1.0f, position.y + 1.0f, position.z + 1.0f
	};
	bulletShootTimer = 0;
	bulletCount = 0;

	isCollidingGhast = false;
	health = 10.0f;
	active = true;
	initBullets();
}

// Calculate the direction from the Ghast's position to the camera position
void Ghast::drawGhast() {
	Vector3 directionToCamera = Vector3Subtract(playerCamera.position, position);
	float rotationAngle = atan2f(directionToCamera.x, directionToCamera.z) * RAD2DEG;
	Vector3 modelScale = { .10f, .10f, .10f };
	DrawModelEx(ghastModel, position, { 0.0f, 1.0f, 0.0f }, rotationAngle, modelScale, WHITE);
	drawBullets();
}
int getRandomNumber(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}
//Update Ghast
void Ghast::updateGhast() {
	//move away from another ghast when colliding with another ghast
	if (isCollidingGhast) {
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
		//move towards player
		Vector3 directionToCamera = Vector3Normalize(Vector3Subtract(playerCamera.position, position));
		position = Vector3Add(position, Vector3Scale(directionToCamera, moveSpeed));
		hitBox = {
			position.x - 1.0f, position.y - 1.0f, position.z - 1.0f,
				position.x + 1.0f, position.y + 1.0f, position.z + 1.0f
		};
	}
	//shoot bullet randomly
	if (bulletShootTimer != (int)GetTime()) {
		bulletShootTimer = (int)GetTime(); 
		int shootChance = getRandomNumber(1, 100);
		if (shootChance <= 30) {  
			PlaySound(ghastBulletSound);
			ShootBullet();
		}
	}
	//update bullets when shooting
	updateBullets();
}

//make a ghast active
void Ghast::activate() {
	float x, z;
	// load position away from player
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
	// load variables
	position = Vector3{ x, 5.0f, z };
	health = 10.0f;
	colllidngPosition = position;
	active = true;
	hitBox = {
		position.x - 1.0f, position.y - 1.0f, position.z - 1.0f,
			position.x + 1.0f, position.y + 1.0f, position.z + 1.0f
	};
}

//initialize bullets
void Ghast::initBullets() {
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bullets[i].position = Vector3{ 0 };
		bullets[i].velocity = Vector3{ 0 };
		bullets[i].radius = 0.25f;
		bullets[i].active = false;
		bullets[i].hitBox = { Vector3{ 0 }, Vector3{ 0 } };
	}
}

//update bullets
void Ghast::updateBullets()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bullets[i].active)
		{
			bullets[i].position.x += bullets[i].velocity.x * GetFrameTime();
			bullets[i].position.y += bullets[i].velocity.y * GetFrameTime();
			bullets[i].position.z += bullets[i].velocity.z * GetFrameTime();
			bullets[i].hitBox = {
				bullets[i].position.x - 0.25f, bullets[i].position.y - 0.25f, bullets[i].position.z - 0.25f,
					bullets[i].position.x + 0.25f, bullets[i].position.y + 0.25f, bullets[i].position.z + 0.25f
			};
			if (bullets[i].position.x < -81.0f || bullets[i].position.x > 81.0f || bullets[i].position.z < -91.0f || bullets[i].position.x > 91.0f)
			{
				bullets[i].active = false;
				bulletCount--;
			}
		}
	}
}

//shoot a bullet
void Ghast::ShootBullet()
{
	if (bulletCount < MAX_BULLETS)
	{
		bullets[bulletCount].position = position;
		Vector3 direction = Vector3Normalize(Vector3Subtract(playerCamera.position, position));
		bullets[bulletCount].velocity.x = direction.x * 10.0f;
		bullets[bulletCount].velocity.y = direction.y * 10.0f;
		bullets[bulletCount].velocity.z = direction.z * 10.0f;

		bullets[bulletCount].active = true;
		bulletCount++;
	}
}
//draw bullet
void Ghast::drawBullets() {
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bullets[i].active)
		{
			DrawModel(fireballModel, bullets[i].position, 0.50f, WHITE);
		}
	}
}

//Getter and Settters
int Ghast::getBulletCount() {
	return bulletCount;
}
Bullet* Ghast::getBullets() {
	return bullets;
}
bool Ghast::getCollidingGhast() {
	return isCollidingGhast;
}
void Ghast::setCollidingGhast(bool input) {
	isCollidingGhast = input;
}
struct BoundingBox Ghast::getHitBox() {
	return hitBox;
}
Vector3 Ghast::getPosition() {
	return position;
}
float Ghast::getHealth() const {
	return health;
}
void Ghast::setHealth(float newHealth) {
	health = newHealth;
}
bool Ghast::isActive() const {
	return active;
}
void Ghast::setActive(bool newActive) {
	active = newActive;
}
Vector3 Ghast::getCollidingPosition() const {
	return colllidngPosition;
}
void Ghast::setCollidingPosition(const Vector3& newCollidingPosition) {
	colllidngPosition = newCollidingPosition;
}
