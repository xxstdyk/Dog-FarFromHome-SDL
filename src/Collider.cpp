#include "Collider.h"

Collider::Collider() :
	m_pParent(nullptr),
	m_offset(glm::vec2(0, 0)),
	m_colliding(false) {
	GetRigidBody()->isColliding = false;
}

Collider::~Collider() = default;

void Collider::Update() {

	GetTransform()->position = m_pParent->GetTransform()->position + m_offset;
	GetTransform()->drawn_position = m_pParent->GetTransform()->drawn_position + m_offset;
}

void Collider::Draw() {

	Util::DrawRect(GetTransform()->drawn_position, GetWidth(), GetHeight());
}

void Collider::Clean() { }

void Collider::SetParent(GameObject * _parent) { this->m_pParent = _parent; }

void Collider::SetOffset(glm::vec2 _offset) { this->m_offset = _offset; }

void Collider::SetColliding(bool _colliding) { this->m_colliding = _colliding; }

GameObject *Collider::GetParent() { return this->m_pParent; }

glm::vec2 Collider::GetOffset() const { return this->m_offset; }

Collider::operator bool() const { return this->m_colliding; }

