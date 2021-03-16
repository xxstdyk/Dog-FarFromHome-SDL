#include "Player.h"
#include <iostream>
#include "EventManager.h"
#include "Game.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "EventManager.h"
#include "Collider.h"

Player::Player() : m_currentAnimationState(PLAYER_IDLE_RIGHT) {
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/dog-spritesheet.png",
		"dogsprite");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("dogsprite"));

	// set frame width
	SetWidth(87);

	// set frame height
	SetHeight(58);

	GetTransform()->position = glm::vec2(540.0f, 300.0f);
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;
	GetRigidBody()->hasGravity = true;

	SetMovementEnabled(true);
	SetAccelerationRate(0.4f);
	SetMaxSpeed(8.5f);

	Collider *gc = new Collider();
	gc->SetParent(this);
	gc->SetWidth(GetWidth());
	gc->SetHeight(GetHeight() / 4);
	gc->SetOffset(glm::vec2(0, GetHeight() * 3 / 4));

	AddCollider(gc, "groundCheck");

	SetType(PLAYER);

	m_buildAnimations();
	BuildSoundIndex();
}

Player::~Player()
= default;

void Player::Draw() {
	// alias for x and y
	const auto x = GetTransform()->drawn_position.x;
	const auto y = GetTransform()->drawn_position.y;

	// draw the player according to animation state
	switch (m_currentAnimationState) {
		case PLAYER_IDLE_RIGHT:
			TextureManager::Instance()->playAnimation("dogsprite", GetAnimation("idle"),
													  x, y, 0.12f, 0, 255, false, SDL_FLIP_HORIZONTAL);
			break;
		case PLAYER_IDLE_LEFT:
			TextureManager::Instance()->playAnimation("dogsprite", GetAnimation("idle"),
													  x, y, 0.12f, 0, 255, false);
			break;
		case PLAYER_RUN_RIGHT:
			TextureManager::Instance()->playAnimation("dogsprite", GetAnimation("run"),
													  x, y, 0.25f, 0, 255, false, SDL_FLIP_HORIZONTAL);
			break;
		case PLAYER_RUN_LEFT:
			TextureManager::Instance()->playAnimation("dogsprite", GetAnimation("run"),
													  x, y, 0.25f, 0, 255, false);
			break;
		default:
			break;
	}

}
void Player::BuildSoundIndex() {
	SoundManager::Instance().load("../Assets/audio/plateSound1.wav", "pressurePlateCollision", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/dogWhine1.mp3", "enemyCollision", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/arf.wav", "defaultSound", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/jumpSound1.wav", "jumpSound", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/landFromJump1.mp3", "landSound", SOUND_SFX); // do this later

	SoundManager::Instance().setSoundVolume(32);
}
bool Player::GetInteracting(){
	return m_interacting;
}

void Player::SetInteracting(bool _interacting) {
	m_interacting = _interacting;
}

void Player::Update() {

	bool colliding = false;
	for (auto &it : m_colliders) {
		it.second->Update();
		colliding = colliding || it.second->operator bool();
	}
	GetRigidBody()->isColliding = colliding;

	Jump();

	EventManager::Instance().update();

	SetInteracting(EventManager::Instance().isKeyDown(SDL_SCANCODE_E));
	
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_Q) && m_canBark) {
		m_barking = true;
		m_canBark = false;
	}
	
	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_Q)) {
		m_canBark = true;
	}

	if (m_barking) {
		SoundManager::Instance().load("../Assets/audio/arf.wav", "barkSound1", SOUND_SFX);
		SoundManager::Instance().playSound("barkSound1", 0, -1);
		std::cout << "arf" << std::endl;
		m_barking = false;
		m_canBark = false;
	}

	// Lets us pause the movement
	if (m_movementEnabled) {
		int moveVal = 0;

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A)) moveVal = -1;
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D)) moveVal =  1;
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A)
			== EventManager::Instance().isKeyDown(SDL_SCANCODE_D)) moveVal = 0;

		m_move(moveVal);
		if (!moveVal) Decel();
	}

	if (GetCollider("groundCheck")->operator bool()) {
		if (GetIsJumping()) {
			SoundManager::Instance().playSound("landSound", 0);
		}
		SetIsJumping(false);
	}

	ApplyMovement();
}

void Player::Clean() { }

// Setters

void Player::AddCollider(Collider * _col, std::string _key) {

	if (!m_colliders.count(_key)) {

		std::cout << "Collider: " << _key << " added.\n";
		m_colliders.insert({ _key, _col });
	}
}

void Player::setAnimationState(const PlayerAnimationState new_state) {
	m_currentAnimationState = new_state;
}


void Player::m_buildAnimations() {

	Animation idleAnimation = Animation();
	idleAnimation.m_name = "idle";

	std::string tmp_str = "dog-idle-";
	for (int i = 0; i < 7; i++) 
		idleAnimation.m_frames.push_back(GetSpriteSheet()->GetFrame(tmp_str + std::to_string(i)));
	setAnimation(idleAnimation);

	Animation runAnimation = Animation();
	runAnimation.m_name = "run";

	tmp_str = "dog-dash-";
	for (int i = 0; i < 5; ++i)
		runAnimation.m_frames.push_back(GetSpriteSheet()->GetFrame(tmp_str + std::to_string(i)));
	setAnimation(runAnimation);

}

void Player::Jump() {

	float deltaTime = TheGame::Instance()->GetDeltaTime();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE) && !GetIsJumping()) {
		GetRigidBody()->velocity.y = -15.0f;
		SetIsJumping(true);
		std::cout << "Jump" << std::endl;
		SoundManager::Instance().playSound("jumpSound", 0);
	}

}

void Player::AddAcceleration(glm::vec2 _accelRate) {
	this->GetRigidBody()->acceleration += _accelRate;
}

void Player::m_move(int _dir) {

	auto accel = m_accelerationRate * _dir;
	if (abs(this->GetRigidBody()->velocity.x) <= GetMaxSpeed()) {
		AddAcceleration(glm::vec2(accel, 0));
	}
}

void Player::Decel() {

	float decelRate = 0.4f;
	auto pushbackForce = this->GetRigidBody()->velocity.x * decelRate * -1.0f;
	
	AddAcceleration(glm::vec2(pushbackForce, 0));

	if (abs(this->GetRigidBody()->velocity.x) <= 0.5f) {
		this->GetRigidBody()->acceleration.x = 0;
		this->GetRigidBody()->velocity.x = 0;
	}
}

void Player::ApplyMovement() {

	auto rb = this->GetRigidBody();
	auto transform = this->GetTransform();

	rb->acceleration.x = Util::clamp(rb->acceleration.x, -0.8f, 0.8f);
	rb->velocity += rb->acceleration;

	rb->velocity.x = Util::clamp(rb->velocity.x, -GetMaxSpeed(), GetMaxSpeed());

	if (rb->velocity.y > 0 && GetCollider("groundCheck")->operator bool()) rb->velocity.y = 0;

	transform->position += rb->velocity;

}

// Setters
void Player::SetAccelerationRate(float _accel) { m_accelerationRate = _accel; }
void Player::SetMaxSpeed(float _speed) { m_maxSpeed = _speed; }
void Player::SetMovementEnabled(bool _val) { m_movementEnabled = _val; }
void Player::SetIsJumping(bool _jump) { m_isJumping = _jump; }

// Getters
float Player::GetAcceleration() { return m_accelerationRate; }
float Player::GetMaxSpeed() { return m_maxSpeed; }
bool Player::GetIsJumping() { return m_isJumping; }
Collider *Player::GetCollider(const std::string _key) const { return m_colliders.at(_key); }