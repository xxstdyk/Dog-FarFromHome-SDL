#include "Elevator.h"

Elevator::Elevator() {
	TextureManager::Instance()->load("../Assets/textures/elevator.png", "Elevator");
	auto size = TextureManager::Instance()->getTextureSize("Elevator");
	SetWidth(size.x);
	SetHeight(size.y);
	GetTransform()->position = glm::vec2(800.0f, 2255.0f); // default location
	GetRigidBody()->isColliding = false;
	m_direction = true;
	SetType(ELEVATOR);
}

Elevator::~Elevator()
= default;

void Elevator::Draw() {
	TextureManager::Instance()->draw("Elevator", GetTransform()->drawn_position.x, GetTransform()->drawn_position.y);
}

void Elevator::Update() {
	m_timer++; //1640
	if (GetTransform()->position.y == 1640)
		m_direction = true;
	else if (GetTransform()->position.y == 1250)
		m_direction = false;

	if (m_direction && m_timer > 1)
	{
		GetTransform()->position.y -= 10;
		m_pPlatformRoof->GetTransform()->position.y -= 10;
		m_pPlatformFloor->GetTransform()->position.y -= 10;
		m_timer = 0;
	}
	else if (!m_direction && m_timer > 1)
	{
		GetTransform()->position.y += 10;
		m_pPlatformRoof->GetTransform()->position.y += 10;
		m_pPlatformFloor->GetTransform()->position.y += 10;
		m_timer = 0;
	}	
}

void Elevator::Clean() {

}

bool Elevator::TimerReady() {
	if (m_timer > 1)
		return true;
	else
		return false;
}