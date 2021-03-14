#pragma once
#ifndef __PRESSURE_PLATE_HANDLER__
#define __PRESSURE_PLATE_HANDLER__

#include <vector>

#include "CollisionManager.h"
#include "Platform.h"
#include "Collider.h"

class PlatformHandler {

	// Member Variables
	private:

	Collider *m_pPlayerGroundCheck;
	std::vector<Platform *> m_pPlatforms;

	// Public Functions
	public:
	void SetGCCollider(Collider *_gc);
	void AddPlatform(Platform *_plat);

	std::vector<Platform *> GetPlatforms();

	void Update();
	void Draw();
};

#endif