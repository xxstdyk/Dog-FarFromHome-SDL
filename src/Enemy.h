#pragma once
#include "EnemyAnimationState.h"
#include "Sprite.h"

#include "CollisionManager.h"

class Enemy final : public Sprite {

	private:
	void m_buildAnimations();

	glm::vec2 m_home;
	bool m_facingRight;

	EnemyAnimationState m_currentAnimationState;

	int m_stunTimer;
	bool m_stunned;

	public:
	Enemy();
	~Enemy();

	void Stun();

	void SetHome(glm::vec2 _pos) { m_home = _pos; }

	// Life Cycle Methods
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

	// setters
	void setAnimationState(EnemyAnimationState new_state);
};