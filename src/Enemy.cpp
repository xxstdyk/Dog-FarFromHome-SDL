#include "Enemy.h"

#include "Util.h"

#include "SoundManager.h"
#include "TextureManager.h"

Enemy::Enemy() : m_currentAnimationState(ENEMY_IDLE_RIGHT) {

	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/CatAtlas.txt",
		"../Assets/sprites/Cat-spriteSheet.png",
		"catsprite");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("catsprite"));

	// Set frame width
	SetWidth(81);

	// Set frame height
	SetHeight(64);

	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;
	GetRigidBody()->hasGravity = true;
	SetType(ENEMY);

	m_facingRight = rand() % 2;

	m_buildAnimations();
}

Enemy::~Enemy()
= default;

void Enemy::Draw() {
	// alias for x and y
	const auto x = GetTransform()->drawn_position.x;
	const auto y = GetTransform()->drawn_position.y;

	// draw the player according to animation state
	switch (m_currentAnimationState) {
		case ENEMY_IDLE_RIGHT:
			TextureManager::Instance()->playAnimation("catsprite", GetAnimation("idle"),
													  x, y, 0.12f, 0, 255, false, SDL_FLIP_HORIZONTAL);
			break;
		case ENEMY_IDLE_LEFT:
			TextureManager::Instance()->playAnimation("catsprite", GetAnimation("idle"),
													  x, y, 0.12f, 0, 255);
			break;
		case ENEMY_ROAM_RIGHT:
			TextureManager::Instance()->playAnimation("catsprite", GetAnimation("run"),
													  x, y, 0.25f, 0, 255, false, SDL_FLIP_HORIZONTAL);
			break;
		case ENEMY_ROAM_LEFT:
			TextureManager::Instance()->playAnimation("catsprite", GetAnimation("run"),
													  x, y, 0.25f, 0, 255);
			break;
		default:
			break;
	}
}

void Enemy::Update() {


	if (m_stunned) ++m_stunTimer;
	if (m_stunned && m_stunTimer % (60 * 4) == 0) {
		m_stunned = false;
		m_stunTimer = 0;
	}

	if (m_stunned) return;

	int max_dist_from_home = 150;

	// check distance from home
	auto dist = Util::distance(GetTransform()->position, m_home);

	if (dist > max_dist_from_home)
		m_facingRight = !m_facingRight;

	int moveDir = m_facingRight ? 1 : -1;
	glm::vec2 moveVec = { moveDir, 0 };

	GetRigidBody()->velocity = moveVec;

	GetTransform()->position += GetRigidBody()->velocity;

	m_currentAnimationState = m_facingRight ? ENEMY_ROAM_RIGHT : ENEMY_ROAM_LEFT;
}

void Enemy::Clean() { }

void Enemy::Stun() {
	m_stunned = true;
}

void Enemy::setAnimationState(const EnemyAnimationState new_state) {
	m_currentAnimationState = new_state;
}

void Enemy::m_buildAnimations() {
	Animation idleAnimation = Animation();

	idleAnimation.m_name = "idle";
	idleAnimation.m_frames.push_back(GetSpriteSheet()->GetFrame("cat-idle-0"));
	idleAnimation.m_frames.push_back(GetSpriteSheet()->GetFrame("cat-idle-1"));
	idleAnimation.m_frames.push_back(GetSpriteSheet()->GetFrame("cat-idle-2"));
	idleAnimation.m_frames.push_back(GetSpriteSheet()->GetFrame("cat-idle-3"));
	idleAnimation.m_frames.push_back(GetSpriteSheet()->GetFrame("cat-idle-4"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.m_name = "run";
	runAnimation.m_frames.push_back(GetSpriteSheet()->GetFrame("cat-walk-0"));
	runAnimation.m_frames.push_back(GetSpriteSheet()->GetFrame("cat-walk-1"));
	runAnimation.m_frames.push_back(GetSpriteSheet()->GetFrame("cat-walk-2"));
	runAnimation.m_frames.push_back(GetSpriteSheet()->GetFrame("cat-walk-3"));
	runAnimation.m_frames.push_back(GetSpriteSheet()->GetFrame("cat-walk-4"));

	setAnimation(runAnimation);
}