#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "PlatformHandler.h"
#include "Platform.h"
#include "Player.h"
#include "PressurePlate.h"
#include "PushableObject.h"
#include "Enemy.h"
#include "Background.h"
#include "Button.h"
#include "Label.h"
#include "Lever.h"
#include "Elevator.h"
#include "Sniff.h"

class PlayScene : public Scene {

	private:
	std::string m_guiTitle;

	glm::vec2 m_mousePosition;

	Platform *m_disappearingLongPlatform;
	PlatformHandler *m_pPlatformHandler;
	std::vector<Platform *> m_disappearingShortPlatforms;

	Sniff *m_pSniff;
	Background *m_pBackground;
	Player *m_pPlayer;
	PressurePlate *m_pPressurePlate;
	PushableObject *m_pBox;
	Lever *m_pLever;
	Lever* m_pBlackLever;
	Enemy *m_pEnemy;
	Elevator *m_pElevator;
	
	bool m_playerFacingRight;
	bool m_boxOnPressurePlate;
	bool m_pressurePlateActivate;
	bool blackLeverIsActivated;
	bool m_playerCanActivateLever;
	bool m_appearingPlatformEnabled;

	// UI Items
	Button *m_pBackButton;
	Button *m_pNextButton;
	Label *m_pInstructionsLabel;

	private:
	// IMGUI Function
	void GUI_Function();

	void CreatePlatforms();

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