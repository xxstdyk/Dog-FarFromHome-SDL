#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Player.h"
#include "PressurePlate.h"
#include "PushableObject.h"
#include "Enemy.h"
#include "Background.h"
#include "Button.h"
#include "Label.h"

class PlayScene : public Scene {

	private:
	std::string m_guiTitle;

	glm::vec2 m_mousePosition;

	Background *m_pBackground;
	Player *m_pPlayer;
	PressurePlate *m_pPressurePlate;
	PushableObject*m_pBox;
	Enemy *m_pEnemy;
	bool m_playerFacingRight;

	// UI Items
	Button *m_pBackButton;
	Button *m_pNextButton;
	Label *m_pInstructionsLabel;

	private:
	// IMGUI Function
	void GUI_Function();

	public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void Start() override;
	virtual void Update() override;
	virtual void Draw() override;
	void CollisionHandler();
	virtual void HandleEvents() override;
	virtual void Clean() override;

};

#endif /* defined (__PLAY_SCENE__) */