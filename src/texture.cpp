
#include "texture.h"

Textures::Textures() {

    treeTexture = LoadTexture("resources/textures/Tree.png");
    wallTexture = LoadTexture("resources/textures/Cobblestone.png");
    dirtTexture = LoadTexture("resources/textures/Dirt.png");
    gunTexture = LoadTexture("resources/textures/Gun.png");
    crosshairTexture = LoadTexture("resources/textures/Crosshair.png");
    platformTexture = LoadTexture("resources/textures/Platform.png");
    castleModel = LoadModel("resources/textures/abounded-castle.glb");                         

}

Textures::~Textures() {

    UnloadTexture(treeTexture);
    UnloadTexture(dirtTexture);
    UnloadTexture(gunTexture);
    UnloadTexture(crosshairTexture);
    UnloadTexture(treeTexture);
    UnloadTexture(platformTexture);
    UnloadModel(castleModel);   
}

void Textures::drawBackgroundTextures() {

    //draw dirt ground
    DrawCubeTextureRec(dirtTexture, Rectangle{ 0, (float)dirtTexture.height, (float)dirtTexture.width, (float)dirtTexture.height },
        Vector3{ 0.0f, 0.0f, 0.0f }, 80.0f, .10f, 90.0f, WHITE);

    //right walls
    DrawCubeTextureRec(wallTexture, Rectangle{ 0, (float)wallTexture.height, (float)wallTexture.width, (float)wallTexture.height },
        Vector3{ -16.0f, 2.5f, 18.5f }, 1.0f, 5.0f, 23.0f, WHITE);
    DrawCubeTextureRec(wallTexture, Rectangle{ 0, (float)wallTexture.height, (float)wallTexture.width, (float)wallTexture.height },
        Vector3{ -16.0f, 2.5f, -4.5f }, 1.0f, 5.0f, 23.0f, WHITE);

    //left walls
    DrawCubeTextureRec(wallTexture, Rectangle{ 0, (float)wallTexture.height, (float)wallTexture.width, (float)wallTexture.height },
        Vector3{ 16.0f, 2.5f, 18.5f }, 1.0f, 5.0f, 23.0f, WHITE);
    DrawCubeTextureRec(wallTexture, Rectangle{ 0, (float)wallTexture.height, (float)wallTexture.width, (float)wallTexture.height },
        Vector3{ 16.0f, 2.5f, -4.5f }, 1.0f, 5.0f, 23.0f, WHITE);

    //back walls
    DrawCubeTextureRec(wallTexture, Rectangle{ 0, (float)wallTexture.height, (float)wallTexture.width, (float)wallTexture.height },
        Vector3{ 8.0f, 2.5f, 30.0f }, 16.0f, 5.0f, 1.0f, WHITE);
    DrawCubeTextureRec(wallTexture, Rectangle{ 0, (float)wallTexture.height, (float)wallTexture.width, (float)wallTexture.height },
        Vector3{ -8.0f, 2.5f, 30.0f }, 16.0f, 5.0f, 1.0f, WHITE);

    //front walls
    DrawCubeTextureRec(wallTexture, Rectangle{ 0, (float)wallTexture.height, (float)wallTexture.width, (float)wallTexture.height },
        Vector3{ 8.0f, 2.5f, -16.0f }, 16.0f, 5.0f, 1.0f, WHITE);
    DrawCubeTextureRec(wallTexture, Rectangle{ 0, (float)wallTexture.height, (float)wallTexture.width, (float)wallTexture.height },
        Vector3{ -8.0f, 2.5f, -16.0f }, 16.0f, 5.0f, 1.0f, WHITE);



    DrawModelEx(castleModel, { -13.0f, 0.0f, -15.0f }, { 1.0f, 0.0f, 0.0f }, -90.0f, { 50.0f, 50.0f, 50.0f }, WHITE);

   
}

//Draw Trees
void Textures::drawTrees(Camera camera) {
    if (camera.position.z > 0) {
        DrawBillboard(camera, treeTexture, Vector3{ -25.0f, 6.0f, -8.0f }, 12.0f, WHITE);
        DrawBillboard(camera, treeTexture, Vector3{ -25.0f, 6.0f, 4.0f }, 12.0f, WHITE);
        DrawBillboard(camera, treeTexture, Vector3{ -25.0f, 6.0f, 16.0f }, 12.0f, WHITE);
        DrawBillboard(camera, treeTexture, Vector3{ -25.0f, 6.0f, 28.0f }, 12.0f, WHITE);
    }
    else {
        DrawBillboard(camera, treeTexture, Vector3{ -25.0f, 6.0f, 28.0f }, 12.0f, WHITE);
        DrawBillboard(camera, treeTexture, Vector3{ -25.0f, 6.0f, 16.0f }, 12.0f, WHITE);
        DrawBillboard(camera, treeTexture, Vector3{ -25.0f, 6.0f, 4.0f }, 12.0f, WHITE);
        DrawBillboard(camera, treeTexture, Vector3{ -25.0f, 6.0f, -8.0f }, 12.0f, WHITE);
    }
}

//Getters and Setters
Texture2D Textures::getTreeTexture() {
    return treeTexture;
}
Texture2D Textures::getWallTexture() {
    return wallTexture;
}
Texture2D Textures::getDirtTexture() {
    return dirtTexture;
}
Texture2D Textures::getGunTexture() {
    return gunTexture;
}
Texture2D Textures::getCrossHairTexture() {
    return crosshairTexture;
}

Texture2D Textures::getPlatformTexture() {
    return platformTexture;
}
Model Textures::getCastleModel() {
    return castleModel;
}



// Draw cube with texture piece applied to all faces
void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    float texWidth = (float)texture.width;
    float texHeight = (float)texture.height;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    // We calculate the normalized texture coordinates for the desired texture-source-rectangle
    // It means converting from (tex.width, tex.height) coordinates to [0.0f, 1.0f] equivalent 
    rlBegin(RL_QUADS);
    rlColor4ub(color.r, color.g, color.b, color.a);

    // Front face
    rlNormal3f(0.0f, 0.0f, 1.0f);
    rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
    rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f(source.x / texWidth, source.y / texHeight);
    rlVertex3f(x - width / 2, y + height / 2, z + length / 2);

    // Back face
    rlNormal3f(0.0f, 0.0f, -1.0f);
    rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
    rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
    rlTexCoord2f(source.x / texWidth, source.y / texHeight);
    rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
    rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x + width / 2, y - height / 2, z - length / 2);

    // Top face
    rlNormal3f(0.0f, 1.0f, 0.0f);
    rlTexCoord2f(source.x / texWidth, source.y / texHeight);
    rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
    rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
    rlVertex3f(x + width / 2, y + height / 2, z - length / 2);

    // Bottom face
    rlNormal3f(0.0f, -1.0f, 0.0f);
    rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
    rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f(source.x / texWidth, source.y / texHeight);
    rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x - width / 2, y - height / 2, z + length / 2);

    // Right face
    rlNormal3f(1.0f, 0.0f, 0.0f);
    rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
    rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
    rlTexCoord2f(source.x / texWidth, source.y / texHeight);
    rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x + width / 2, y - height / 2, z + length / 2);

    // Left face
    rlNormal3f(-1.0f, 0.0f, 0.0f);
    rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
    rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f(source.x / texWidth, source.y / texHeight);
    rlVertex3f(x - width / 2, y + height / 2, z - length / 2);

    rlEnd();

    rlSetTexture(0);
}