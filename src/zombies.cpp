#include "zombies.h"


Zombies::Zombies(Camera & camera) : playerCamera(camera){

	zombieDeathSound = LoadSound("resources/sounds/ZombieDeathSound.wav");
	for (int i = 0; i < MAX_ZOMBIES; i++){
		zombieArray.push_back(new Zombie(playerCamera));
		if (i > 10) {
			zombieArray[i]->setActive(false);
		}
	}
} 
Zombies::~Zombies() {
	UnloadSound(zombieDeathSound);
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		delete zombieArray[i];
	}
}

//reset zombies
void Zombies::reset() {
	zombieSpawnTimer = 0;
	activeZombieCount = 10;
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		zombieArray[i]->reset();
		if (i > 10) {
			zombieArray[i]->setActive(false);
		}
	}
}

//draw zombies
void Zombies::drawZombies() {
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		if(zombieArray[i]->isActive())
			zombieArray[i]->drawZombie();
	}
}

//update zombies
void Zombies::updateZombies() {
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		if (zombieArray[i]->isActive()) {
			zombieCollision(i);
			zombieArray[i]->updateZombie();
			if (zombieArray[i]->getHealth() == 0.0f) {
				PlaySound(zombieDeathSound);
				zombieArray[i]->setActive(false);
				activeZombieCount--;
			}
		}
	}
}

//check zombie collision with other zombies
void Zombies::zombieCollision(int i) {
    for (int j = 0; j < MAX_ZOMBIES; j++) {
        if (CheckCollisionBoxes(zombieArray[i]->getHitBox(), zombieArray[j]->getHitBox()) && i != j && zombieArray[j]->getCollidingZombie() != true && zombieArray[j]->isActive()) {

            zombieArray[i]->setCollidingZombie(true);
            Vector3 direction = Vector3Subtract(zombieArray[i]->getPosition(), zombieArray[j]->getPosition());
            direction = Vector3Normalize(direction);
            Vector3 newPosition = Vector3Add(zombieArray[i]->getPosition(), direction);
			zombieArray[i]->setCollidingPosition(newPosition);
			break;
        }
        else {
            zombieArray[i]->setCollidingZombie(false);
        }
    }
}


//spawn zombies
void Zombies::spawnZombie() {
	auto findInactiveZombie = [](Zombie* zombie) {
		return !zombie->isActive();
	};
	if ((int)GetTime() % 2 == 0 && zombieSpawnTimer != (int)GetTime()) {
		zombieSpawnTimer = (int)GetTime();
		auto it = std::find_if(zombieArray.begin(), zombieArray.end(), findInactiveZombie);
		if (it != zombieArray.end()) {
			size_t index = std::distance(zombieArray.begin(), it);
			zombieArray[index]->activate();
			activeZombieCount++;
		}
	}
}


std::vector<Zombie*> Zombies::getZombieArray() {
	return zombieArray;
}