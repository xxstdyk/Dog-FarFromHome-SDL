#pragma once
#ifndef ___DRAWBRIDGE___
#define ___DRAWBRIDGE___
#include "InteractableObject.h"
#include "TextureManager.h"
#include "Platform.h"

class Drawbridge : public InteractableObject {
	Platform* m_pPlatformTop, * m_pPlatformBottom;
public:
	Drawbridge();
	~Drawbridge();

	void Draw() override;
	void Update() override;
	void Clean() override;
};
#endif // !___DRAWBRIDGE___