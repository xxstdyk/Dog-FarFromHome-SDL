#include "SettingScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"



SettingScene::SettingScene() // begaining of program constructs everything 
{
	SettingScene::Start();
}

SettingScene::~SettingScene()
= default;

void SettingScene::Draw()  // renders objects
{
	DrawDisplayList();
}

void SettingScene::Update() // updates the image / scenes
{
	UpdateDisplayList();
}

void SettingScene::Clean() // last thing the program does
{
	RemoveAllChildren();
}

void SettingScene::HandleEvents() // handle keyboard inputs 
{
	EventManager::Instance().update();

	// Button Events
	m_backButton->Update();

	// Keyboard Events
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}
}




void SettingScene::Start() // main window  res , controls , sound , back 
{
	DisplayObject* m_pBackground = new Background();
	AddChild(m_pBackground);

	const SDL_Color white = { 255, 255, 255, 0 }; //255
	m_settinglabel = new Label("SETTINGS", "Consolas", 80, white, glm::vec2(800.0f, 100.0f));
	m_settinglabel->SetParent(this);
	AddChild(m_settinglabel);

	// Resolution Button
	m_resolutionButton = new Button("../Assets/textures/resolution_Button.png", "resolutionButton", RESTART_BUTTON);
	m_resolutionButton->GetTransform()->position = glm::vec2(800.0f, 325.0f);
	
	m_resolutionButton->AddEventListener(CLICK, [&]()-> void
	{
		m_resolutionButton->setActive(false);
		//TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_resolutionButton->AddEventListener(MOUSE_OVER, [&]()->void
	{
		m_resolutionButton->setAlpha(128);
	});

	m_resolutionButton->AddEventListener(MOUSE_OUT, [&]()->void
	{
		m_resolutionButton->setAlpha(255);
	});

	AddChild(m_resolutionButton);

	// controls
	m_controlsButton = new Button("../Assets/textures/controlsButton.png", "backButton", RESTART_BUTTON);
	m_controlsButton->GetTransform()->position = glm::vec2(800.0f, 400.0f);

	m_controlsButton->AddEventListener(CLICK, [&]()-> void
	{
		m_controlsButton->setActive(false);
		//TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_controlsButton->AddEventListener(MOUSE_OVER, [&]()->void
	{
		m_controlsButton->setAlpha(128);
	});

	m_controlsButton->AddEventListener(MOUSE_OUT, [&]()->void
	{
		m_controlsButton->setAlpha(255);
	});

	AddChild(m_controlsButton);



	

	// sound changed to back 
	m_soundButton = new Button("../Assets/textures/backButton.png", "soundButton", RESTART_BUTTON);
	m_soundButton->GetTransform()->position = glm::vec2(800.0f, 550.0f);   // 350

	m_soundButton->AddEventListener(CLICK, [&]()-> void
	{
		m_soundButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_soundButton->AddEventListener(MOUSE_OVER, [&]()->void
	{
		m_soundButton->setAlpha(128);
	});

	m_soundButton->AddEventListener(MOUSE_OUT, [&]()->void
	{
		m_soundButton->setAlpha(255);
	});

	AddChild(m_soundButton);

	//c button
	
	// back changed to sound 
	
	m_backButton = new Button("../Assets/textures/soundButton.png", "controlsButton", RESTART_BUTTON);
	m_backButton->GetTransform()->position = glm::vec2(800.0f, 475.0f);

	m_backButton->AddEventListener(CLICK, [&]()-> void
	{
		m_backButton->setActive(false);
		//TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_backButton->AddEventListener(MOUSE_OVER, [&]()->void
	{
		m_backButton->setAlpha(128);
	});

	m_backButton->AddEventListener(MOUSE_OUT, [&]()->void
	{
		m_backButton->setAlpha(255);
	});

	AddChild(m_backButton);


	


}
