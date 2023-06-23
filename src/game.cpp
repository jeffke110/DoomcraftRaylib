#include "game.h"


Game::Game() {
    InitWindow(screenWidth, screenHeight, "Jeffrey Kedda - First Person Jump Game");
    InitAudioDevice();
    DisableCursor();
    SetTargetFPS(60);

    //Loads Characters and Platforms
    player = new Player();
    ghasts = new Ghasts(*player->getCamera());
    zombies = new Zombies(*player->getCamera());
    textureControl = new Textures();
    bulletControl = new Bullets();
    setPlatforms();

    //Loads sounds
    playerSoundEffect = LoadSound("resources/sounds/PlayerSoundEffect.wav");
    gunShotSoundEffect = LoadSound("resources/sounds/Gunshot.wav");
    zombieSoundEffect = LoadSound("resources/sounds/ZombieSoundEffect.wav");
    ghastSoundEffect = LoadSound("resources/sounds/GhastSoundEffect.wav");
    backgroundMusic = LoadSound("resources/sounds/BackgroundMusic.wav");

    //Loads Textures for menus
    mainMenuTexture = LoadTexture("resources/textures/Doomcraft!.png");
    instructionsMenuTexture = LoadTexture("resources/textures/InstructionsMenu.png");
   
}

//Reset Game
void Game::reset() {
    zombieTimeCollision = 0;
    ghastTimeCollision = 0;
    ghastTimeBulletCollision = 0;
    player->reset();
    ghasts->reset();
    zombies->reset();
    bulletControl->reset();
}


//Delete Games
Game::~Game() {
    delete player;
    delete textureControl;
    delete bulletControl;
    delete zombies;
    delete ghasts;
}


//unload raylib objects
void Game::unload() {
    UnloadTexture(mainMenuTexture);
    UnloadTexture(instructionsMenuTexture);
    UnloadSound(playerSoundEffect);
    UnloadSound(gunShotSoundEffect);
    UnloadSound(zombieSoundEffect);
    UnloadSound(ghastSoundEffect);
    UnloadSound(backgroundMusic);
}

//set corner platforms 
void Game::setPlatforms() {
    platforms.push_back(Platform{ {10.5f, 1.0f, 25.0f} });
    platforms.push_back(Platform{ {-10.5f, 1.0f, 25.0f} });
    platforms.push_back(Platform{ {10.5f, 1.0f, -10.5f} });
    platforms.push_back(Platform{ {-10.5f, 1.0f, -10.5f} });
    for (auto& platform : platforms) {
        platform.hitBox = {
            platform.position.x - 5.1f, platform.position.y - 1.0f, platform.position.z - 5.1f,
            platform.position.x + 5.1f, platform.position.y + 1.0f, platform.position.z + 5.1f
        };
    }
}

//check wall collision for players
void Game::checkWallCollision() {
    if (player->getCamera()->position.x >= 15.3f || player->getCamera()->position.x <= -15.3f) {
        player->getCamera()->position.x = player->getPrevCamera().position.x;
    }
    if (player->getCamera()->position.z >= 29.3f || player->getCamera()->position.z <= -15.3f) {
        player->getCamera()->position.z = player->getPrevCamera().position.z;
    }
}

// player collision with platform
void Game::playerPlatformCollision() {

    bool isColliding = false;
    for (const auto& platform : platforms) {
        if (CheckCollisionBoxes(player->getHitBox(), platform.hitBox)) {

            if (player->getCamera()->position.y - 2.0f <= platform.position.y + 1.0f && !player->getOnBox() && player->getCamera()->position.y > 3.5) {
                // Player is on top of the box
                player->getCamera()->position.y = 4.0f;
                player->setJumping(false);
                player->setOnBox(true);
                player->setJumpSpeed(0.0f);
            }
            else if(player->getOnBox() == false) {
                // Player collided with the side of the box
                player->getCamera()->position = player->getPrevCamera().position;
                player->setOnBox(false);
            }
            else {
                player->setOnBox(false);
            }
            isColliding = true;
            break;
        }
    }

    if (!isColliding && player->getCamera()->position.y == 4.0f) {
        player->setOnBox(false);
        player->setJumping(true);
    }

}

//zombie collision with platform
void Game::zombiePlatformCollision(int i) {
    Vector3 zombiePosition = zombies->getZombieArray()[i]->getPosition();
    Zombie * zombieTemp = zombies->getZombieArray()[i];

    bool isColliding = false;
    for (const auto& platform : platforms) {

        if (CheckCollisionBoxes(zombieTemp->getHitBox(), platform.hitBox)) {
            if (zombiePosition.y  <= platform.position.y + 1.0f && !zombieTemp->getOnBox() && zombiePosition.y > 1.5) {
                // Player is on top of the box
                zombieTemp->setPosition(Vector3{ zombiePosition.x, 2.0f, zombiePosition.z });
                zombieTemp->setJumping(false);
                zombieTemp->setOnBox(true);
                zombieTemp->setJumpSpeed(0.0f);
            }
            else if (zombieTemp->getOnBox() == false) {
                zombieTemp->setOnBox(false);
                zombieTemp->setJumping(true);
                zombieTemp->setJumpSpeed(.10f);
            }
            else {
                zombieTemp->setOnBox(false);
            }
            isColliding = true;
            break;
        }
    }
    if (!isColliding && zombieTemp->getPosition().y == 2.0f) {
        zombieTemp->setOnBox(false);
        zombieTemp->setJumping(true);
    }

}

//zombie collision with bullet from player and player collision
void Game::zombieBulletCollision(int i) {
    Zombie* zombieTemp = zombies->getZombieArray()[i];
    for (auto &bullet : bulletControl->bullets) {
        //collision with bullet
        if (CheckCollisionBoxes(zombieTemp->getHitBox(), bullet.hitBox) && bullet.active) {
            PlaySound(zombieSoundEffect);
            float zombieHealth = zombieTemp->getHealth();
            zombieTemp->setHealth(--zombieHealth);
            bullet.active = false;
        }
        //also checks collision with player
        if (CheckCollisionBoxes(zombieTemp->getHitBox(), player->getHitBox())) {
            float playerHealth = player->getHealth() - 0.0001f;
            player->setHealth(playerHealth);
        }
        // so that the sound effect isn't played repeatedly
        if (CheckCollisionBoxes(zombieTemp->getHitBox(), player->getHitBox()) && zombieTimeCollision != (int)GetTime()) {
            zombieTimeCollision++;
            PlaySound(playerSoundEffect);
        }
    }
}

//update all zombie attributes
void Game::zombiesUpdate() {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (zombies->getZombieArray()[i]->isActive()) {
            zombiePlatformCollision(i);
            zombieBulletCollision(i);
        }
    }
    zombies->updateZombies();
    zombies->spawnZombie();
}


//ghast collision with bullet and player
void Game::ghastBulletCollision(int i) {
    Ghast* ghastTemp = ghasts->getGhastArray()[i];
    for (auto& bullet : bulletControl->bullets) {
        //collision with bullet
        if (CheckCollisionBoxes(ghastTemp->getHitBox(), bullet.hitBox) && bullet.active) {
            PlaySound(ghastSoundEffect);
            float ghastHealth = ghastTemp->getHealth();
            ghastTemp->setHealth(--ghastHealth);
            bullet.active = false;
        }
        //also checks collision with player
        if (CheckCollisionBoxes(ghastTemp->getHitBox(), player->getHitBox())) {
            float playerHealth = player->getHealth() - 0.0001f;
            player->setHealth(playerHealth);
        }
        //so that the sound effect isn't played repeatedly
        if (CheckCollisionBoxes(ghastTemp->getHitBox(), player->getHitBox()) && ghastTimeCollision != (int)GetTime()) {
            PlaySound(playerSoundEffect);
            ghastTimeCollision++;
        }
    }
    //check ghast bullet collision with player
    Bullet* bullets = ghastTemp->getBullets();
    for (int i = 0; i < ghastTemp->getBulletCount(); i++) {
        Bullet bullet = bullets[i];
        if (CheckCollisionBoxes(player->getHitBox(), bullet.hitBox) && bullet.active && ghastTimeBulletCollision != (int)GetTime()) {
            PlaySound(playerSoundEffect);
            ghastTimeBulletCollision++;
            bullet.active = false;
            float playerHealth = player->getHealth() - 3.0f;
            player->setHealth(playerHealth);
        }
    }
}

//update all ghast attributes
void Game::ghastsUpdate() {

    for (int i = 0; i < MAX_GHASTS; i++) {
        if (ghasts->getGhastArray()[i]->isActive()) {
            ghastBulletCollision(i);
        }
    }
    ghasts->updateGhasts();
    ghasts->spawnGhast();

}

//update player attributes
void Game::playerUpdate() {
    playerPlatformCollision();
    checkWallCollision();
    player->jetPack();
    player->checkJump();
    player->updateCameraPosition();
}


//draw necessary scenery/textures/bullets
void Game::draw() {

    BeginDrawing();
    ClearBackground(RED);
    BeginMode3D(*player->getCamera());
        //draw characters
        zombies->drawZombies();
        bulletControl->drawBullets();
        ghasts->drawGhasts();
       
        // Draw Platforms
        for (const auto& platform : platforms) {
            DrawCubeTextureRec(textureControl->getPlatformTexture(), Rectangle{0, (float)textureControl->getPlatformTexture().height, (float)textureControl->getPlatformTexture().width, (float)textureControl->getPlatformTexture().height},
                platform.position, platform.width, platform.height, platform.length, WHITE);
        }
        //draw background 3d textures
        textureControl->drawBackgroundTextures();
        textureControl->drawTrees(*player->getCamera());

    EndMode3D();

    //draw gun related textures
    DrawTexture(textureControl->getGunTexture(), 300, 400, WHITE);
    DrawTexture(textureControl->getCrossHairTexture(), 385, 285, WHITE);

    //player info
    player->drawJetPackInfo();

    Rectangle resetGameButton = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 20, 200, 80 };
    Rectangle menuGameButton = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 100, 200, 80 };
    if (currentState == GameState::GameOver) {
        DrawText("You Died", (float)screenWidth / 2 - 100, (float)screenHeight / 2 - 100, 50, RED);

        DrawRectangleRec(resetGameButton, LIGHTGRAY);
        DrawText("Play Again?", resetGameButton.x + 25, resetGameButton.y + 30, 20, DARKGRAY);

        DrawRectangleRec(menuGameButton, LIGHTGRAY);
        DrawText("Main Menu", menuGameButton.x + 25, menuGameButton.y + 30, 20, DARKGRAY);
    }

    EndDrawing();

}

//Draw main menu
void Game::drawMainMenu() {
    ShowCursor();
    Rectangle playGameButton = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 20, 200, 80 };
    Rectangle instructionsButton = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 100, 200, 80 };
    // Check if "Play Game" button is pressed
    if (CheckCollisionPointRec(GetMousePosition(), playGameButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        currentState = GameState::PlayGame;
        DisableCursor();
    }

    // Check if "Instructions" button is pressed
    if (CheckCollisionPointRec(GetMousePosition(), instructionsButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        currentState = GameState::Instructions;
    }
    // Draw
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawTexture(mainMenuTexture, 0, 0, WHITE);

    DrawRectangleRec(playGameButton, LIGHTGRAY);
    DrawText("Play Game", playGameButton.x + 25, playGameButton.y + 30, 20, DARKGRAY);

    DrawRectangleRec(instructionsButton, LIGHTGRAY);
    DrawText("Instructions", instructionsButton.x + 25, instructionsButton.y + 30, 20, DARKGRAY);

    EndDrawing();
}

//Draw instruction menu
void Game::drawInstructionsMenu() {
    ShowCursor();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Check if the mouse click is within the button area
        Rectangle buttonBounds = { (float)screenWidth / 2 - 50, (float)screenHeight - 75, 100, 50 };
        Vector2 mousePosition = GetMousePosition();

        if (CheckCollisionPointRec(mousePosition, buttonBounds))
        {
            // Transition back to the Main Menu state
            currentState = GameState::MainMenu;
        }
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw the Instructions UI
    DrawTexture(instructionsMenuTexture, 0, 0, WHITE);
    DrawRectangle((float)screenWidth / 2 - 50, (float)screenHeight - 75, 100, 50, BLUE);
    DrawText("Back", (float)screenWidth / 2 - MeasureText("Back", 20) / 2, (float)screenHeight - 60, 20, WHITE);

    EndDrawing();

}

//Draw game
void Game::playGame() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        PlaySound(gunShotSoundEffect);
        bulletControl->ShootBullet(player->getCamera());
    }
    bulletControl->updateBullets();
    ghastsUpdate();
    zombiesUpdate();

    playerUpdate();
    draw();
    if (player->getHealth() <= 0.0f){
        currentState = GameState::GameOver;
    }
}

//Draw game over screen
void Game::gameOver() {
    ShowCursor();
    Rectangle resetGameButton = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 20, 200, 80 };
    Rectangle menuGameButton = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 100, 200, 80 };
    if (CheckCollisionPointRec(GetMousePosition(), resetGameButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        reset();
        DisableCursor();
        currentState = GameState::PlayGame;
    }
    if (CheckCollisionPointRec(GetMousePosition(), menuGameButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        reset();
        currentState = GameState::MainMenu;
    }
    draw();
}

//Run game
void Game::run() {
    while (!WindowShouldClose())
        
    {   //Prevent rotation of camera (q and e originally rotate camera
        if (IsKeyDown(KEY_Q) || IsKeyDown(KEY_E))
        {
            player->getCamera()->up = Vector3{0.0f, 1.0f, 0.0f};
            player->getCamera()->fovy = 60.0f;
        }

        if (!IsSoundPlaying(backgroundMusic)) {
            PlaySound(backgroundMusic);
        }
        if (currentState == GameState::MainMenu) {
            drawMainMenu();
        }
        else if (currentState == GameState::Instructions) {
            drawInstructionsMenu();
        }
        else if (currentState == GameState::PlayGame) {
            playGame();
        }
        else if (currentState == GameState::GameOver) {
            gameOver();
        }
    }
    unload();
    CloseAudioDevice();
    CloseWindow();
}


