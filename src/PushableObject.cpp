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
	Util::DrawRect(GetTransform()->drawn_position, GetWidth(), GetHeight());
}

void PushableObject::Update() {

	GetTransform()->position += GetRigidBody()->velocity;

	GetRigidBody()->velocity.x *= 0.975f;
	if (GetRigidBody()->velocity.x <= 0.5f)
		GetRigidBody()->velocity.x = 0;
}

void PushableObject::Clean() {

}