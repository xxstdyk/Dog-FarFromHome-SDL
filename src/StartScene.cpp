#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "SettingScene.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"
#include "PlayScene.h"

StartScene::StartScene() {
	StartScene::Start();
}

StartScene::~StartScene()
= default;

void StartScene::Draw() {
	DrawDisplayList();
}

void StartScene::Update() {
	UpdateDisplayList();
	UpdateGlobalPositions();
}

void StartScene::Clean() {
	RemoveAllChildren();
}

void StartScene::HandleEvents() {
	EventManager::Instance().update();

	// Keyboard Events
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE)) {
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(SETTING_SCENE);
	}
}

void StartScene::Start() {

	//Background
	DisplayObject *m_pBackground = new Background();
	AddChild(m_pBackground);
	//const SDL_Color blue = { 0, 0, 255, 255 };
	const SDL_Color white = { 255, 255, 255, 0 };
	m_pStartLabel = new Label("Dog: Far From Home", "Consolas", 95, white, glm::vec2(800.0f, 150.0f));
	m_pStartLabel->SetParent(this);
	AddChild(m_pStartLabel);

	// StartButton
	m_pStartButton = new Button();
	m_pStartButton->GetTransform()->position = glm::vec2(800.0f, 425.0f);

	m_pStartButton->AddEventListener(CLICK, [&]()-> void {
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});

	m_pStartButton->AddEventListener(MOUSE_OVER, [&]()->void {
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->AddEventListener(MOUSE_OUT, [&]()->void {
		m_pStartButton->setAlpha(255);
	});
	AddChild(m_pStartButton);

	// Settings Button 
	m_settingButton = new Button("../Assets/textures/settingsButton.png","settingsButton",SETTINGS_BUTTON);
	m_settingButton->GetTransform()->position = glm::vec2(800.0f, 525.0f);

	m_settingButton->AddEventListener(CLICK, [&]()-> void {
		m_settingButton->setActive(false);
		TheGame::Instance()->changeSceneState(SETTING_SCENE);
	});

	m_settingButton->AddEventListener(MOUSE_OVER, [&]()->void {
		m_settingButton->setAlpha(128);
	});

	m_settingButton->AddEventListener(MOUSE_OUT, [&]()->void {
		m_settingButton->setAlpha(255);
	});
	AddChild(m_settingButton);

	SoundManager::Instance().setMusicVolume(20);
	SoundManager::Instance().load("../Assets/audio/menuSongReal.mp3", "mainMenuSong", SOUND_MUSIC);
	SoundManager::Instance().playMusic("mainMenuSong", -1);
}