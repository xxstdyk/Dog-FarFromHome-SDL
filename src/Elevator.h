#pragma once
#ifndef __ELEVATOR__
#define __ELEVATOR__
#include "InteractableObject.h"
#include "Platform.h"
#include "Sprite.h"
#include "SoundManager.h"
#include "TextureManager.h"

class Elevator : public InteractableObject {
	Platform* m_pPlatformFloor, *m_pPlatformRoof;
public:
	Elevator();
	~Elevator();

	int m_timer;
	bool m_direction;

	void Draw() override;
	void Update() override;
	void Clean() override;

	bool TimerReady();

	void setRoof(Platform* _roof) { m_pPlatformRoof = _roof; }
	void setFloor(Platform* _floor) { m_pPlatformFloor = _floor; }
	bool getDirection() { return m_direction; }
	Platform* GetRoof() { return m_pPlatformRoof; }
	Platform* GetFloor() { return m_pPlatformFloor; }
};
#endif // !__ELEVATOR__