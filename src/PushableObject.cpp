#include "PushableObject.h"

#include "CollisionManager.h"

PushableObject::PushableObject() {
	TextureManager::Instance()->load("../Assets/textures/box.png", "forestBox");
	glm::vec2 size = { 64, 64 };						//TODO: fix the size of the box because it too smol
	SetWidth(size.x);
	SetHeight(size.y);
	GetRigidBody()->hasGravity = true;	// This gives the PushableObject gravity
	SetType(PUSHABLE_OBJECT);			// This sets the PushableObject to the correct enum
}

void PushableObject::Draw() {
	TextureManager::Instance()->draw("forestBox", GetTransform()->drawn_position.x, GetTransform()->drawn_position.y);
}

void PushableObject::Update() {
	GetTransform()->position += GetRigidBody()->velocity;
}

void PushableObject::Clean() {
	
}
