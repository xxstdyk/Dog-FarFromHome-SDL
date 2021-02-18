#include "PressurePlate.h"
#include "../src/SoundManager.h"
#include "../src/TextureManager.h"

PressurePlate::PressurePlate() {

	TextureManager::Instance()->load("../Assets/textures/pressure_plate.png", "pressurePlate");
	TextureManager::Instance()->getTextureSize("pressurePlate");
	SetWidth(140.0f);
	SetHeight(146.0f);
	GetTransform()->position = glm::vec2(167.0f, 146.0f);
	GetRigidBody()->isColliding = false;
	SetType(PRESSURE_PLATE);

	//For Joshi
	//SoundManager::Instance().load();
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