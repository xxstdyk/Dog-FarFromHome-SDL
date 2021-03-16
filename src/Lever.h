#pragma once
#ifndef __LEVER__
#define __LEVER__

#include "InteractableObject.h"
#include "Sprite.h"
#include "SoundManager.h"
#include "TextureManager.h"

class Lever : public InteractableObject {
public: 
	Lever();
	~Lever();

	void Draw() override;
	void Update() override;
	void Clean() override;

};



#endif