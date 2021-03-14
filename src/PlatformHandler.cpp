#include "PlatformHandler.h"

void PlatformHandler::SetGCCollider(Collider *_gc) { m_pPlayerGroundCheck = _gc; }

void PlatformHandler::AddPlatform(Platform *_plat) { m_pPlatforms.push_back(_plat); }

std::vector<Platform *> PlatformHandler::GetPlatforms() { return m_pPlatforms; }

void PlatformHandler::Update() {

	bool colliding = false;
	Platform *collidedPlat = nullptr;


	for (auto platform : m_pPlatforms) {

		if (CollisionManager::AABBCheck(m_pPlayerGroundCheck, platform)) {
			collidedPlat = platform;
			colliding = true;
			break;
		}
	}

	if (colliding && collidedPlat != nullptr) {
		
		m_pPlayerGroundCheck->GetRigidBody()->isColliding = true;

		if (m_pPlayerGroundCheck->GetTransform()->position.y + m_pPlayerGroundCheck->GetHeight() > collidedPlat->GetTransform()->position.y) {
			std::cout << "moving player" << std::endl;
			m_pPlayerGroundCheck->GetParent()->GetTransform()->position.y = collidedPlat->GetTransform()->position.y;
		}
	}

}

void PlatformHandler::Draw() {

	for (auto platform : m_pPlatforms) {
		platform->Draw();
	}
}