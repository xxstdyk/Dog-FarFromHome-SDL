#pragma once
#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "PlayerAnimationState.h"
#include "Collider.h"
#include "Sprite.h"

class Player final : public Sprite {

	private:

	std::map<std::string, Collider *> m_colliders;

	float m_accelerationRate;
	float m_maxSpeed;

	bool m_movementEnabled;
	bool m_isJumping; // This will track whether or not the player is jumping 

	private:

	void BuildSoundIndex();

	public:

	Player();
	~Player();

	// Life Cycle Methods
	virtual void Draw() override;
	void HandleEvents();
	virtual void Update() override;
	virtual void Clean() override;

	void AddCollider(Collider *_col, const std::string _key);

	// setters
	void setAnimationState(PlayerAnimationState new_state);
	void SetAccelerationRate(float _accel);
	void SetMaxSpeed(float _speed);
	void SetMovementEnabled(bool _val);
	void SetIsJumping(bool _jump);

	// Getters
	Collider *GetCollider(const std::string _key) const;

	float GetAcceleration();
	float GetMaxSpeed();

	bool GetIsJumping();

	private:

	void Jump();
	void Decelerate();
	void m_buildAnimations();
	void Move(bool _direction);

	bool m_barking, m_canBark;

	PlayerAnimationState m_currentAnimationState;
};

#endif /* defined (__PLAYER__) */