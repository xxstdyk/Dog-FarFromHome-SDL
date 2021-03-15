#include "PlatformHandler.h"

void PlatformHandler::SetGCCollider(Collider *_gc) { m_pGroundCheck = _gc; }

void PlatformHandler::AddPlatform(Platform *_plat) { m_pPlatforms.push_back(_plat); }

std::vector<Platform *> PlatformHandler::GetPlatforms() { return m_pPlatforms; }

void PlatformHandler::Update() {

	Platform *collidedPlat = nullptr;

	for (auto platform : m_pPlatforms) {
		platform->GetRigidBody()->isColliding = false;
		if (CollisionManager::AABBCheck(m_pGroundCheck, platform)) {
			m_pGroundCheck->GetRigidBody()->isColliding = true;
			m_pGroundCheck->SetColliding(true);
			collidedPlat = platform;
			break;
		}
	}

	if (collidedPlat != nullptr) {
		if (m_pGroundCheck->GetTransform()->position.y + m_pGroundCheck->GetHeight() > collidedPlat->GetTransform()->position.y) {
			auto parent = m_pGroundCheck->GetParent();
			if (parent->GetRigidBody()->velocity.y > 0) { 
			
				parent->GetRigidBody()->acceleration.y = 0;
				parent->GetRigidBody()->velocity.y = 0;
				
				//parent->GetRigidBody()->velocity.y = collidedPlat->GetTransform()->position.y 
				//	- (parent->GetTransform()->position.y + parent->GetHeight());
			}
		}
	}
}

void PlatformHandler::Draw() {

	for (auto platform : m_pPlatforms) {
		platform->Draw();
	}
}