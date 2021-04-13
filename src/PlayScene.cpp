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
	m_pBox->GetTransform()->position = glm::vec2(1155.0f, 1080.0f);
	AddChild(m_pBox);

	// Sniff thing
	m_pSniff = new Sniff();
	m_pSniff->GetTransform()->position = glm::vec2(0.0f, 0.0f);
	AddChild(m_pSniff, 11);

	// Lever Sprite (Red)
	m_pLever = new Lever();
	m_pLever->GetTransform()->position = glm::vec2(2100.0f, 2177.0f);
	AddChild(m_pLever);

	// Lever Sprite (Black)
	m_pLeverBlack = new Lever;
	m_pLever->GetTransform()->position = glm::vec2(5550.0f, 1500);
	leverIsActivated == true;
	AddChild(m_pLeverBlack);


	// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->SetMovementEnabled(true);
	m_pPlayer->GetTransform()->position = glm::vec2(4000.0f, 1950); /*(800.0f, 2200.0f);*/
	AddChild(m_pPlayer, 10);
	m_playerFacingRight = true;

	// Platform Handler
	// MUST BE CREATED AFTER PLAYER
	m_pPlatformHandler = new PlatformHandler();
	m_pPlatformHandler->SetGCCollider(m_pPlayer->GetCollider("groundCheck"));
	CreatePlatforms();

	//Enemy Sprite (cat)
	m_pEnemy = new Enemy();
	m_pEnemy->GetTransform()->position = glm::vec2(2110.0f, 1850);
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
		for (auto platform : m_pPlatformHandler->GetPlatforms()) AddChild(platform);
		m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2725.0f, 1100), 100, 30));    //Appearing Platform 1 (Low)
		m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2900.0f, 1250), 100, 30));    //Appearing Platform 2 (Mid)
		m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2900.0f, 1000), 100, 30));    //Appearing Platform 2 (High)

		if (leverIsActivated == true)
		{
			m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(5153.0f, 100), 100, 100));  //Ground Platform above Area 3 with Lever(Right) 
		}

	}

	// Move camera to track player
	GetTransform()->position = m_pPlayer->GetTransform()->position - glm::vec2(760.0f, 550.0f);

	// Stop camera from moving out of bounds
	const int LEFT_BOUND = 0, RIGHT_BOUND = 4350, VERTICAL_BOUND = 2400;
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
	//Level 2 - Area 1 (Anything with an X is final position - DO NOT TOUCH)
	//Ground Platforms
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(720.0f, 2325.0f), 1000, 10));      //Ground Platform/Spawn X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2576.0f, 2325.0f), 480, 10));      //Ground Platform for Enemy/Cave X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(670.0f, 2282.0f), 52, 55));        //Ground Corner Platform (Left) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(460.0f, 2082.0f), 220, 255));      //Ground Platform Far Left X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(0.0f, 2038.0f), 483, 50));         //Ground Platform Far Far Left  X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1883.0f, 2240.0f), 656, 40));      //Ground Platform with Lever X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1722, 2282.0f), 194, 50));         //Ground Corner Platform (Right) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2523, 2282.0f), 55, 55));          //Ground Corner Platform (Right inside of Cave) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2453.0f, 2120), 75, 40));          //Ground Platform above cave entrance X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2529.0f, 2080), 3444, 50));        //Ground Platform under fire tower - Extends to Area 2X

	//Side/Wall Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(3057.0f, 2125), 10, 220));         //Side Platform for Enemy/Cave (Right) X

	//Wooden Platforms
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1863.0f, 1877), 660, 10));         //Wood Platform with Lantern X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1556.0f, 1800), 208, 10));         //Wood Platform with Elevator X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1400.0f, 1300), 100, 500));        //(Temp) Elevator Platform X 
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(792.0f, 1483.0f), 248, 10));       //Wood Platform with Two Pressureplates X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1036.0f, 1200.0f), 410, 10));      //Wood Platform with Telescope X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1560.0f, 1280.0f), 354, 10));      //Wood Platform attached to Tree Platform (Right of Telescope) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2297.0f, 1315.0f), 387, 10));      //Wood Platform attached to Tree (Left of Fire Tower) X

	//Fire Tower Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(3020.0f, 884), 650 , 10));         //Fire Tower Platform X

	//Level 2 - Area 2
	//Ground Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4478.0f, 1560), 457, 10));         //Ground Platform above Area 3 (Left) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(5153.0f, 1560), 810, 10));         //Ground Platform above Area 3 with Lever(Right) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4900.0f, 1560), 300, 10));         //(Temp) Ground Platform above Area 3 (Middle) X

	//Wooden Platforms
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4000.0f, 1960), 100, 30));         //Short Wooden Platform X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4120.0f, 1837), 100, 30));         //Medium Wooden Platform X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4240.0f, 1760), 100, 30));         //Medium-High Wooden Platform X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4320.0f, 1640), 100, 30));         //High Wooden Platform X

	//Side/Wall Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4478.0f, 1560), 45, 518));         //Side Platform for Area 3 (Left) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(5913.0f, 1560), 45, 518));         //Side Platform for Area 3 (Right) X



	for (auto platform : m_pPlatformHandler->GetPlatforms()) AddChild(platform);
}

void PlayScene::CollisionHandler() {

	bool soundPlayed = false;

	if (CollisionManager::AABBCheck(m_pPlayer, m_pEnemy)) {

		std::cout << "Hit an Enemy!" << std::endl;
		SoundManager::Instance().playSound("enemyCollision", 0);
	}

	if (CollisionManager::AABBCheck(m_pBox, m_pPressurePlate)) {
	
		if (!soundPlayed) {

			std::cout << "Box is on top of the pressure plate" << std::endl;
			m_pPressurePlate->GetTransform()->position = glm::vec2(1950.0f, 1880.0f);
			SoundManager::Instance().playSound("pressurePlateCollision", 0);
			soundPlayed = true;
		}

	}

	if (!CollisionManager::AABBCheck(m_pPlayer, m_pPressurePlate)) {
		m_pPressurePlate->GetTransform()->position = glm::vec2(1950.0f, 1880.0f);
	}
	m_pLever->GetRigidBody()->isColliding = false;

	if (CollisionManager::AABBCheck(m_pPlayer, m_pLever)) {

		m_playerCanActivateLever = true;
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
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone) {
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else {
				if (m_playerFacingRight) {
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else {
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
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D)) {
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else {
			if (m_playerFacingRight) {
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else {
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