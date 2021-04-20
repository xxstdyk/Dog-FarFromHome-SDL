#include <iostream>
#include "Platform.h"
#include "Util.h"
#include "EventManager.h"

Platform::Platform(glm::vec2 _position, int _width, int _height) {
	TextureManager::Instance()->load("../Assets/textures/shortPlatform.png", "appearingShortPlatform");
	TextureManager::Instance()->load("../Assets/textures/appearingLongPlatform.png", "appearingLongPlatform");
	GetTransform()->position = _position;

	SetWidth(_width);
	SetHeight(_height);

	GetRigidBody()->isColliding = false;
	GetRigidBody()->hasGravity = false;

	SetType(PLATFORM);
}

Platform::~Platform()
= default;

void Platform::Draw() {
	//Debug - Hold H for platform outline
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
		Util::DrawRect(GetTransform()->drawn_position, GetWidth(), GetHeight());

	if (leverIsActivated)
		TextureManager::Instance()->draw("appearingShortPlatform", GetTransform()->drawn_position.x, GetTransform()->drawn_position.y);

	if (pressurePlateActivated)
		TextureManager::Instance()->draw("appearingLongPlatform", GetTransform()->drawn_position.x, GetTransform()->drawn_position.y);
}

void Platform::Update() { }

void Platform::Clean() { }
