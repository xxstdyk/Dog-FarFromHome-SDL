#include "PressurePlate.h"
#include "SoundManager.h"
#include "TextureManager.h"

PressurePlate::PressurePlate() {

	TextureManager::Instance()->load("../Assets/textures/pressurePlate.png", "pressurePlate");
	auto size = TextureManager::Instance()->getTextureSize("pressurePlate");
	SetWidth(size.x);
	SetHeight(size.y);
	GetTransform()->position = glm::vec2(2576.0f, 2325.0);
	GetRigidBody()->isColliding = false;
	m_timerOn = true;
	SetType(PRESSURE_PLATE);
}

PressurePlate::~PressurePlate()
= default;
void PressurePlate::Draw() {
	TextureManager::Instance()->draw("pressurePlate", GetTransform()->drawn_position.x, GetTransform()->drawn_position.y);
}

void PressurePlate::Update() {
	if (m_timerOn && this->GetEnabled())
		this->IncrementTimer();
	std::cout << this->GetTimer() << std::endl;
}

void PressurePlate::Clean() {

}

void PressurePlate::DisableTimer() { m_timerOn = false; }
