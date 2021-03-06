#include "Lever.h"

Lever::Lever() {
	TextureManager::Instance()->load("../Assets/textures/leverOff.png", "leverOff");
	TextureManager::Instance()->load("../Assets/textures/leverOn.png", "leverOn");
	TextureManager::Instance()->load("../Assets/textures/leverOffBlack.png", "blackLever");
	auto size = TextureManager::Instance()->getTextureSize("leverOff");
	SetWidth(size.x);
	SetHeight(size.y);
	GetTransform()->position = glm::vec2(0.0f, 0.0f); // default location
	GetRigidBody()->isColliding = false;
	SetType(LEVER);
}

Lever::~Lever()
= default;

void Lever::Draw() {
	if (GetEnabled()) TextureManager::Instance()->draw("leverOn", GetTransform()->drawn_position.x, GetTransform()->drawn_position.y);
	else TextureManager::Instance()->draw("leverOff", GetTransform()->drawn_position.x, GetTransform()->drawn_position.y);
}

void Lever::Update() {
}

void Lever::Clean() {

}