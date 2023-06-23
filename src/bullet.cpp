

#include "bullet.h"

Bullets::Bullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].position = Vector3{ 0 };
        bullets[i].velocity = Vector3{ 0 };
        bullets[i].radius = 0.05f;
        bullets[i].active = false;
        bullets[i].hitBox = { Vector3{ 0 }, Vector3{ 0 }};
    }
}

//reset bullets when game reset
void Bullets::reset() {
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].position = Vector3{ 0 };
        bullets[i].velocity = Vector3{ 0 };
        bullets[i].radius = 0.05f;
        bullets[i].active = false;
        bullets[i].hitBox = { Vector3{ 0 }, Vector3{ 0 } };
    }
}
//update bullets
void Bullets::updateBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].position.x += bullets[i].velocity.x * GetFrameTime();
            bullets[i].position.y += bullets[i].velocity.y * GetFrameTime();
            bullets[i].position.z += bullets[i].velocity.z * GetFrameTime();
            bullets[i].hitBox = {
                bullets[i].position.x - 0.05f, bullets[i].position.y - 0.05f, bullets[i].position.z - 0.05f,
                    bullets[i].position.x + 0.05f, bullets[i].position.y + 0.05f, bullets[i].position.z + 0.05f
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
void Bullets::ShootBullet(Camera * camera)
{
    if (bulletCount < MAX_BULLETS)
    {
        bullets[bulletCount].position = camera->position;
        Vector3 direction = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
        bullets[bulletCount].velocity.x = direction.x * 50.0f;
        bullets[bulletCount].velocity.y = direction.y * 50.0f;
        bullets[bulletCount].velocity.z = direction.z * 50.0f;

        bullets[bulletCount].active = true;
        bulletCount++;
    }
}

//draw the bullets
void Bullets::drawBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            DrawSphere(bullets[i].position, bullets[i].radius, BLACK);
        }
    }
}