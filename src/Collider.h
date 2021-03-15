#pragma once
#ifndef __COLLIDER__
#define __COLLIDER__

#include "util.h"
#include "GameObject.h"

class Collider : public GameObject {

	private:

	bool m_colliding;
	GameObject *m_pParent;
	glm::vec2 m_offset;

	public:
	Collider();
	~Collider();

	// Inherited via GameObject
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

	void SetParent(GameObject *_parent);
	void SetOffset(glm::vec2 _offset);
	void SetColliding(bool _colliding);
	
	GameObject *GetParent(); 
	glm::vec2 GetOffset() const;

	explicit operator bool() const;
};

#endif