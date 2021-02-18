#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene() {
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Draw() {

	DrawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);

	if (EventManager::Instance().isIMGUIActive()) {

		GUI_Function();
	}

}

void PlayScene::Update() {
	
	UpdateDisplayList();
	TickGravity();
	UpdateGlobalPositions();

	GetTransform()->local_position =  m_pPlayer->GetTransform()->local_position - glm::vec2(150.0f, 475.0f);

	CollisionManager::AABBCheck(m_pPlayer, m_pEnemy);
	if (CollisionManager::AABBCheck(m_pPlayer, m_pPressurePlate))
	{
		m_pPressurePlate->GetTransform()->local_position = glm::vec2(625, 477.0f);
	}

	if (!CollisionManager::AABBCheck(m_pPlayer, m_pPressurePlate))
	{
		m_pPressurePlate->GetTransform()->local_position = glm::vec2(625, 470.0f);
	}
}

void PlayScene::Clean() {
	RemoveAllChildren();
}

void PlayScene::HandleEvents() {
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0) {
		if (EventManager::Instance().getGameController(0) != nullptr) {
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone) {
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			} else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone) {
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			} else {
				if (m_playerFacingRight) {
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				} else {
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}
	
	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1) {
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A)) {
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		} else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D)) {
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		} else {
			if (m_playerFacingRight) {
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			} else {
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}


	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE)) {
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1)) {
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2)) {
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::Start() {
	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	//Background
	m_pBackground = new Background();
	AddChild(m_pBackground);

	//Pressure plate Sprite
	m_pPressurePlate = new PressurePlate();
	m_pPressurePlate->GetTransform()->local_position = glm::vec2(625, 470.0f);
	AddChild(m_pPressurePlate);

	// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->SetMovementEnabled(true); 
	m_pPlayer->GetTransform()->local_position = glm::vec2(150.0f, 475.0f);
	AddChild(m_pPlayer);
	m_playerFacingRight = true;

	//Enemy Sprite (cat)
	m_pEnemy = new Enemy();
	m_pEnemy->GetTransform()->local_position = glm::vec2(700.0f, 285.0f);
	AddChild(m_pEnemy);
	
	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->GetTransform()->local_position = glm::vec2(5000.0f, 2000.0f);
	m_pBackButton->AddEventListener(CLICK, [&]()-> void {
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->AddEventListener(MOUSE_OVER, [&]()->void {
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->AddEventListener(MOUSE_OUT, [&]()->void {
		m_pBackButton->setAlpha(255);
	});
	AddChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->GetTransform()->local_position = glm::vec2(5000.0f, 4000.0f);
	m_pNextButton->AddEventListener(CLICK, [&]()-> void {
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->AddEventListener(MOUSE_OVER, [&]()->void {
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->AddEventListener(MOUSE_OUT, [&]()->void {
		m_pNextButton->setAlpha(255);
	});
	SoundManager::Instance().setMusicVolume(5);
	SoundManager::Instance().load("../Assets/audio/menuSongTest.mp3", "forestSong", SOUND_MUSIC);
	SoundManager::Instance().playMusic("forestSong", -1);
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Dog - Far From Home", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float cameraPos[2] = { GetTransform()->local_position.x, GetTransform()->local_position.y };
	if (ImGui::SliderFloat2("Camera XY", cameraPos, -1000.0f, 1000.0f)) {
		GetTransform()->local_position = glm::vec2(cameraPos[0], cameraPos[1]);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
