#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "raylib.h"
#include "rlgl.h" 
#include "rcamera.h"

#include <iostream>


class Textures {

public:
	Textures();
	~Textures();
	
	//Draw
	void drawBackgroundTextures();
	void drawTrees(Camera camera);

	//Getters and setters
	Texture2D getTreeTexture();
	Texture2D getWallTexture();
	Texture2D getDirtTexture();
	Texture2D getGunTexture();
	Texture2D getCrossHairTexture();
	Texture2D getPlatformTexture();
	Model getCastleModel();

	

private:
	//Textures and model
	Texture2D treeTexture;
	Texture2D wallTexture;
	Texture2D dirtTexture;
	Texture2D gunTexture;
	Texture2D crosshairTexture;
	Texture2D platformTexture;
	Model castleModel;


};

void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color);

#endif // !TEXTURE_H