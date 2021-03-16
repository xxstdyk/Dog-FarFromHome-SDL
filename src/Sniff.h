#pragma once
#ifndef __SNIFF__
#define __SNIFF__

//TODO: not actually sniff yet

#include "DisplayObject.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Sprite.h"

class Sniff final : public DisplayObject {
public:
	Sniff();
	~Sniff();

	// Inherited via GameObject
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

private:
	
};
#endif