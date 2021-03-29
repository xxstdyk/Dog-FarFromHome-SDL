#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "TextureManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

void PlayScene::Start() {
	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	// Background
	m_pBackground = new Background();
	AddChild(m_pBackground);

	// Pressure plate Sprite
	m_pPressurePlate = new PressurePlate();
	m_pPressurePlate->GetTransform()->position = glm::vec2(2100.0f, 2100.0f);
	AddChild(m_pPressurePlate);

	// Box sprite
	m_pBox = new PushableObject();
	m_pBox->GetTransform()->position = glm::vec2(1150.0f, 800.0f);
	AddChild(m_pBox);
	
	// Sniff thing
	m_pSniff = new Sniff();
	m_pSniff->GetTransform()->position = glm::vec2(0.0f, 0.0f);
	AddChild(m_pSniff, 11);

	// Lever Sprite
	m_pLever = new Lever();
	m_pLever->GetTransform()->position = glm::vec2(1750.0f, 1920.0f);
	AddChild(m_pLever);

	// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->SetMovementEnabled(true);
	m_pPlayer->GetTransform()->position = glm::vec2(650.0f, 1920.0f);
	AddChild(m_pPlayer, 10);
	m_playerFacingRight = true;

	// Platform Handler
	// MUST BE CREATED AFTER PLAYER
	m_pPlatformHandler = new PlatformHandler();
	m_pPlatformHandler->SetGCCollider(m_pPlayer->GetCollider("groundCheck"));
	CreatePlatforms();

	//Enemy Sprite (cat)
	m_pEnemy = new Enemy();
	m_pEnemy->GetTransform()->position = glm::vec2(2110.0f, 1868.0f);
	AddChild(m_pEnemy);

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->GetTransform()->position = glm::vec2(5000.0f, 2000.0f);
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
	m_pNextButton->GetTransform()->position = glm::vec2(5000.0f, 4000.0f);
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
	SoundManager::Instance().load("../Assets/audio/forestBGM1.mp3", "forestSong", SOUND_MUSIC);
	SoundManager::Instance().playMusic("forestSong", -1);
}

void PlayScene::Update() {

	UpdateDisplayList();
	m_pPlatformHandler->Update();
	CollisionHandler();
	TickGravity();
	UpdateGlobalPositions();
	CollisionHandler();

	if (m_pPlayer->GetInteracting() && m_playerCanActivateLever)
	{
		m_pLever->SetEnabled(!m_pLever->GetEnabled());
		std::cout << "You activated lever" << std::endl;
		
		m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2525.0f, 700), 100, 30));     //Appearing Platform 1 (Lower)
		m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2700.0f, 850), 100, 30));     //Appearing Platform 2 (Higher)
		std::cout << "stupid ass program" << std::endl;
		for (auto platform : m_pPlatformHandler->GetPlatforms()) AddChild(platform);
		m_appearingPlatformEnabled = false;
	}

	
	GetTransform()->position =  m_pPlayer->GetTransform()->position - glm::vec2(760.0f, 550.0f);

	std::cout << GetTransform()->position.x << std::endl;

	const int LEFT_BOUND = 0, RIGHT_BOUND = 2240, VERTICAL_BOUND = 1160;
	if (GetTransform()->position.x < LEFT_BOUND) GetTransform()->position.x = LEFT_BOUND;
	if (GetTransform()->position.x > RIGHT_BOUND) GetTransform()->position.x = RIGHT_BOUND;
	if (GetTransform()->position.y > VERTICAL_BOUND) GetTransform()->position.y = VERTICAL_BOUND;

}

void PlayScene::Draw() {

	DrawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);

	m_pPlatformHandler->Draw();
	m_pPlayer->GetCollider("groundCheck")->Draw();

	if (EventManager::Instance().isIMGUIActive()) {
		GUI_Function();
	}
}

void PlayScene::CreatePlatforms() {
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(-4000.0f, 1975.0f), 10000, 10));  //Ground 
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1813.0f, 1545), 637, 10));         //Lantern Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1320.0f, 1470), 390, 10));         //Elevator Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1100.0f, 1300), 200, 10));         //(Temp) Elevator Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(774.0f, 1170.0f), 238, 10));       //Pressureplate Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1009.0f, 882.0f), 398, 10));       //Telescope Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(750.0f, 960.0f), 200, 10));        //(Temp)Telescope Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1512.0f, 960.0f), 345, 10));       //Tree Platform (Right of Telescope)
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2230.0f, 999.0f), 377, 10));       //Tree Platform (Right of Tree Platform(1))
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2230.0f, 999.0f), 377, 10));       //Tree Platform (Right of Tree Platform(1))
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2930.0f, 590), 625, 10));          //Fire Tower Platform 
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1670.0f, 1935), 830, 50));
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1825.0f, 1893), 640, 10));         //Ground Platform under cat/enemy
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2385.0f, 1780), 85, 10));          //Ground Platform above cave entrance
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2455.0f, 1738), 1400, 10));        //Ground Platform under fire tower
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(650.0f, 1935.0f), 50, 10));        //Corner piece on ground (Left)
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(0.0f, 1735.0f), 660, 10));        //Ground Platform Far Far Left 
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(0.0f, 1700.0f), 465, 10));        //Ground Platform Far Left

	for (auto platform : m_pPlatformHandler->GetPlatforms()) AddChild(platform);
}

void PlayScene::CollisionHandler() {

	if (CollisionManager::AABBCheck(m_pPlayer, m_pEnemy)) {

		std::cout << "Hit an Enemy!" << std::endl;
		SoundManager::Instance().playSound("enemyCollision", 0);
	}

	if (CollisionManager::AABBCheck(m_pPlayer, m_pPressurePlate)) {

		std::cout << "Stepped on the pressure plate!" << std::endl;
		m_pPressurePlate->GetTransform()->position = glm::vec2(1950.0f, 1880.0f);
		SoundManager::Instance().playSound("pressurePlateCollision", 0);
	}

	if (!CollisionManager::AABBCheck(m_pPlayer, m_pPressurePlate)) {
		m_pPressurePlate->GetTransform()->position = glm::vec2(1950.0f, 1880.0f);
	}
	m_pLever->GetRigidBody()->isColliding = false;

	if (CollisionManager::AABBCheck(m_pPlayer, m_pLever)) {

		std::cout << "Dog is in range to activate lever" << std::endl;
		m_playerCanActivateLever = true;
		SoundManager::Instance().playSound("pressurePlateCollision", 0);
	}
	else m_playerCanActivateLever = false;


	for (auto platform : m_pPlatformHandler->GetPlatforms()) CollisionManager::AABBCheck(platform, m_pBox);
										
	m_pBox->GetRigidBody()->isColliding = false;
	if (CollisionManager::AABBCheck(m_pPlayer, m_pBox)) {
		m_pPlayer->SetMaxSpeed(2.0f);
		// This sets the player's speed to 2.0f or -2.0f when the player hits the box to make sure that the box has the correct speed
		if (m_pPlayer->GetRigidBody()->velocity.x > 0)
			m_pPlayer->GetRigidBody()->velocity.x = 2.0f;
		else if (m_pPlayer->GetRigidBody()->velocity.x < 0)
			m_pPlayer->GetRigidBody()->velocity.x = -2.0f;
		m_pBox->SetEnabled(true);		
		//m_pBox->GetTransform()->position.y = m_pPlayer->GetTransform()->position.y;
		m_pBox->GetRigidBody()->velocity.x = m_pPlayer->GetRigidBody()->velocity.x;
	}									
	else {								
		m_pBox->SetEnabled(false);
		m_pPlayer->SetMaxSpeed(8.5f);	
	}
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

	m_pSniff->SetEnabled((EventManager::Instance().isKeyDown(SDL_SCANCODE_LSHIFT)));

}
// Sniff logic thing



void PlayScene::Clean() {
	RemoveAllChildren();
}

void PlayScene::GUI_Function() {
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Dog - Far From Home", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float cameraPos[2] = { GetTransform()->position.x, GetTransform()->position.y };
	if (ImGui::SliderFloat2("Camera XY", cameraPos, -1000.0f, 1000.0f)) {
		GetTransform()->position = glm::vec2(cameraPos[0], cameraPos[1]);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

PlayScene::PlayScene() {
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;