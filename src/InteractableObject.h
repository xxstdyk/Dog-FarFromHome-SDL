#pragma once
#ifndef __INTERACTABLE_OBJECT__
#define __INTERACTABLE_OBJECT__

#include "DisplayObject.h"

class InteractableObject : public DisplayObject {

	private:
	bool m_enabled;

	public:

	virtual void Draw() override = 0;
	virtual void Update() override = 0;
	virtual void Clean() override = 0;

	void SetEnabled(const bool _status);
	bool GetEnabled() const;

	operator bool();
};

#endif