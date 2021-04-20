#include "Drawbridge.h"


Drawbridge::Drawbridge(){
	TextureManager::Instance()->load("../Assets/textures/drawBridgeStatic.png", "Drawbridge");
	auto size = TextureManager::Instance()->getTextureSize("Drawbridge");
	SetWidth(size.x);
	SetHeight(size.y);
	GetTransform()->position = glm::vec2(800.0f, 2255.0f);
	GetRigidBody()->isColliding = false;
	SetType(DRAWBRIDGE);
}

Drawbridge::~Drawbridge()
= default;

void Drawbridge::Draw(){
	TextureManager::Instance()->draw("Drawbridge", GetTransform()->drawn_position.x, GetTransform()->drawn_position.y);
}

void Drawbridge::Update() {}
void Drawbridge::Clean(){}