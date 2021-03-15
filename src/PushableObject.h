#pragma once
#ifndef __PushableObject_H__
#define __PushableObject_H__
#include "InteractableObject.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "Player.h"

class PushableObject : public InteractableObject
{
private:
	Player* m_player;
	
public:
	PushableObject();
	
	void Draw() override;
	void Update() override;
	void Clean() override;
};


#endif