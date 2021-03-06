#pragma once
#ifndef __PRESSUREPLATE__
#define __PRESSUREPLATE__

#include "InteractableObject.h"
#include "Sprite.h"

class PressurePlate final : public InteractableObject {
	bool m_timerOn;
	
	public:
	PressurePlate();   // Constructor
	~PressurePlate();  // Destructor

	void Draw() override;
	void Update() override;
	void Clean() override;

	void DisableTimer();

};
#endif

