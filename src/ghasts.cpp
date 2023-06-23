#include "ghasts.h"


Ghasts::Ghasts(Camera& camera) : playerCamera(camera){
	ghastDeathSound = LoadSound("resources/sounds/GhastDeathSound.wav");
	for (int i = 0; i < MAX_GHASTS; i++) {
		ghastArray.push_back(new Ghast(playerCamera));
		if (i > 2) {
			ghastArray[i]->setActive(false);
		}
	}
}

Ghasts::~Ghasts() {
	UnloadSound(ghastDeathSound);
	for (int i = 0; i < MAX_GHASTS; i++) {
		delete ghastArray[i];
	}
}

//reset ghasts
void Ghasts::reset() {
	ghastSpawnTimer = 0;
	activGhastCount = 10;
	for (int i = 0; i < MAX_GHASTS; i++) {
		ghastArray[i]->reset();
		if (i > 2) {
			ghastArray[i]->setActive(false);
		}
	}
}

//draw ghasts
void Ghasts::drawGhasts() {
	for (int i = 0; i < MAX_GHASTS; i++) {
		if(ghastArray[i]->isActive())
			ghastArray[i]->drawGhast();
	}
}

//update ghasts
void Ghasts::updateGhasts() {
	for (int i = 0; i < MAX_GHASTS; i++) {
		if (ghastArray[i]->isActive()) {
			ghastCollision(i);
			ghastArray[i]->updateGhast();

			if (ghastArray[i]->getHealth() == 0.0f) {
				PlaySound(ghastDeathSound);
				ghastArray[i]->setActive(false);
				activGhastCount--;
			}
		}
	}
}

//ghasts collision with other ghasts
void Ghasts::ghastCollision(int i) {
	for (int j = 0; j < MAX_GHASTS; j++) {
		if (CheckCollisionBoxes(ghastArray[i]->getHitBox(), ghastArray[j]->getHitBox()) && i != j && ghastArray[j]->getCollidingGhast() != true && ghastArray[j]->isActive()) {
			ghastArray[i]->setCollidingGhast(true);
			Vector3 direction = Vector3Subtract(ghastArray[i]->getPosition(), ghastArray[j]->getPosition());
			direction = Vector3Normalize(direction);
			Vector3 newPosition = Vector3Add(ghastArray[i]->getPosition(), direction);
			ghastArray[i]->setCollidingPosition(newPosition);
			break;
		}
		else {
			ghastArray[i]->setCollidingGhast(false);
		}

	}
}

//spawn ghasts
void Ghasts::spawnGhast() {
	auto findInactiveGhast = [](Ghast* ghast) {
		return !ghast->isActive();
	};
	if ((int)GetTime() % 7 == 0 && ghastSpawnTimer != (int)GetTime()) {
		ghastSpawnTimer = (int)GetTime();
		auto it = std::find_if(ghastArray.begin(), ghastArray.end(), findInactiveGhast);
		if (it != ghastArray.end()) {
			size_t index = std::distance(ghastArray.begin(), it);
			ghastArray[index]->activate();
			activGhastCount++;
		}
	}
}
std::vector<Ghast*> Ghasts::getGhastArray() {
	return ghastArray;
}