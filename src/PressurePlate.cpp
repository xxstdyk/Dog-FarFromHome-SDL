#include "PressurePlate.h"
#include "SoundManager.h"
#include "TextureManager.h"

PressurePlate::PressurePlate() {

	TextureManager::Instance()->load("../Assets/textures/pressure plate.png", "pressurePlate");
	auto size = TextureManager::Instance()->getTextureSize("pressurePlate");
	SetWidth(size.x);
	SetHeight(size.y);
	GetTransform()->position = glm::vec2(2100.0f, 1868.0f);
	GetRigidBody()->isColliding = false;
	SetType(PRESSURE_PLATE);
}

PressurePlate::~PressurePlate()
= default;

void PressurePlate::Draw() {
	TextureManager::Instance()->draw("pressurePlate", GetTransform()->drawn_position.x, GetTransform()->drawn_position.y);
}

void PressurePlate::Update() {

}

void PressurePlate::Clean() {

}