#pragma once
#ifndef __BACKGROUND__
#define __BACKGROUND__

#include "DisplayObject.h"

class Background final : public DisplayObject {
public:
	Background();
	~Background();

	// Inherited via GameObject
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

private:
	void m_move();
	void m_checkBounds();
	void m_reset();
};
#endif