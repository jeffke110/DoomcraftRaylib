#pragma once
#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"
#include "raymath.h"



#define MAX_BULLETS 1000

typedef struct {
    Vector3 position;
    Vector3 velocity;
    float radius;
    bool active;
    struct BoundingBox hitBox;
} Bullet;


class Bullets {

public:
    Bullets();
    void reset();
    void updateBullets();
    void ShootBullet(Camera * camera);
    void drawBullets();
    Bullet bullets[MAX_BULLETS];

private:
    
    int bulletCount = 0;
};

Vector3 Vector3Normalize(Vector3 vector);
Vector3 Vector3Subtract(Vector3 v1, Vector3 v2);




#endif // !BULLET_H
