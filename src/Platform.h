#pragma once
#ifndef __PLATFORM__
#define __PLATFORM__

#include "DisplayObject.h"
#include "TextureManager.h"

class Platform final : public DisplayObject { 

	private:

	public:
	Platform(glm::vec2, int, int);
	~Platform();

	void Draw() override;
	void Update() override;
	void Clean() override;

	//Player X and Y
	float PlayerPosX;
	float PlayerPosY;
	float PlayerVelX;
	float PlayerVelY;
	bool OnGround;
	bool leverIsActivated;
	
};

#endif