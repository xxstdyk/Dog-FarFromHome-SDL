#include <iostream>
#include "Platform.h"
#include "Util.h"

Platform::Platform(glm::vec2 _position, int _width, int _height) {
	
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

	Util::DrawRect(GetTransform()->drawn_position, GetWidth(), GetHeight());
}

void Platform::Update() { }

void Platform::Clean() { }
