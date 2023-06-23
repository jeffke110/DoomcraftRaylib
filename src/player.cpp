#include "player.h"


Player::Player() {
    //init the camera
    camera = new Camera();
    *camera = { 0 };
    camera->position = Vector3{ 0.0f, 2.0f, 10.0f };
    camera->target = Vector3{ 0.0f, 2.0f, 0.0f };
    camera->up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera->fovy = 60.0f;
    camera->projection = CAMERA_PERSPECTIVE;
    cameraMode = CAMERA_FIRST_PERSON;
    position = camera->position;
    prevCamera = *camera;

    //hitbox
    hitBox = {
        camera->position.x, camera->position.y - 2.0f, camera->position.z,
            camera->position.x, camera->position.y, camera->position.z
    };

    //Load Textures and sounds
    healthBar = LoadTexture("resources/textures/HealthBar.png");
    fuelBar = LoadTexture("resources/textures/FuelBar.png");
    fuelFont = LoadTexture("resources/textures/FuelFont.png");
    jetPackSoundEffect = LoadSound("resources/sounds/JetpackSoundEffect.wav");
}

//reset variables
void Player::reset() {
    camera->position = Vector3{ 0.0f, 2.0f, 10.0f };
    camera->target = Vector3{ 0.0f, 2.0f, 0.0f };
    health = 100.0f;
    prevCamera = *camera;

    hitBox = {
    camera->position.x, camera->position.y - 2.0f, camera->position.z,
        camera->position.x, camera->position.y, camera->position.z
    };

    //jumping
    jetPackOn = false;
    isJumping = false;
    onBox = false;
    fuelRegen = false;
    fuel = 400;
    jumpSpeed = 0.0f;
    jumpForce = .10f;
}

//delete and unload
Player::~Player() {
    UnloadTexture(healthBar);
    UnloadTexture(fuelBar);
    UnloadTexture(fuelFont);
    UnloadSound(jetPackSoundEffect);
    delete camera;
}

//camera info for debugging
void Player::drawCameraInfo() {

    DrawRectangle(600, 5, 195, 100, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(600, 5, 195, 100, BLUE);
    DrawText("Camera status:", 610, 15, 10, BLACK);
    DrawText(TextFormat("- Projection: %s", (camera->projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
        (camera->projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"), 610, 45, 10, BLACK);
    DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera->position.x, camera->position.y, camera->position.z), 610, 60, 10, BLACK);
    DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera->target.x, camera->target.y, camera->target.z), 610, 75, 10, BLACK);
    DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera->up.x, camera->up.y, camera->up.z), 610, 90, 10, BLACK);

}

//draws jet pack info for interface
void Player::drawJetPackInfo() {
    //fuel info
    int barY = 502.9 + (float)(100 - fuel) / 4;
    int barHeight = fuel / 4;
   
    DrawRectangle(75, barY, 30, barHeight, ORANGE);
    DrawTexture(fuelBar, 75, 424, WHITE);
    DrawTexture(fuelFont, 60, 540, WHITE);
    //DrawText(TextFormat("- fuel: (%06d)", fuel), 0, 0, 10, BLACK);
    DrawRectangle(71, 29, health * 1.64, 38, GREEN);
    DrawTexture(healthBar, 30, 20, WHITE);
   

}

//Updates the position of the player
void Player::updateCameraPosition() {
    prevCamera = *camera;
    UpdateCamera(camera, cameraMode);         
    position = camera->position;
    hitBox = {
    camera->position.x, camera->position.y - 2.0f, camera->position.z,
        camera->position.x, camera->position.y, camera->position.z
    };

}

//jet pack controls
void Player::jetPack() {
    if (IsKeyDown(KEY_Z) && !fuelRegen) {
        if (!IsSoundPlaying(jetPackSoundEffect)) {
            PlaySound(jetPackSoundEffect);
        }
        if (fuel > 0 && (int)GetFrameTime() % 60 == 0) {
            fuel--;
            camera->position.y += GetFrameTime() * 6.0f;
            camera->target.y += GetFrameTime() * 6.0f;
            isJumping = true;
            jumpSpeed = 0.0f;
        }
        else if (fuel == 0) {
            fuelRegen = true;
        }
    }
    else if (fuelRegen) {
        PauseSound(jetPackSoundEffect);
        if (fuel < 400 && (int)GetFrameTime() % 60 == 0) {
            fuel++;
        }
        if (fuel == 400) {
            fuelRegen = false;
        }
    }
    else {
        PauseSound(jetPackSoundEffect);
    }
}

//checks the jump function
void Player::checkJump() {
    if (isJumping)
    {
        camera->position.y += jumpSpeed;
        camera->target.y += jumpSpeed;
        // Apply gravity over time
        jumpSpeed -= gravity * GetFrameTime(); 

        if (camera->position.y <= 2.0f)
        {
            camera->position.y = 2.0f;
            isJumping = false;
        }
    }
    else
    {
        if (IsKeyDown(KEY_SPACE))
        {   
            isJumping = true;
            jumpSpeed = jumpForce;
        }

    }
}


//Getters and Setters
bool Player::getOnBox() {
    return onBox;
}
void Player::setOnBox(bool input) {
    onBox = input;
}
void Player::setJumping(bool input) {
    isJumping = input;
}
void Player::setJumpSpeed(float input) {
    jumpSpeed = input;
}
struct BoundingBox Player::getHitBox() {
    return hitBox;
}
void Player::setHealth(float input) {
    health = input;
}
float Player::getHealth() {
    return health;
}
Camera * Player::getCamera() {
    return camera;
}
void Player::setCamera(Camera * camera) {
    this->camera = camera;
}
Camera Player::getPrevCamera() {
    return prevCamera;
}

void Player::setPrevCamera(Camera camera) {
    prevCamera = camera;
}