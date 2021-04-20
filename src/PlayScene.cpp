#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "TextureManager.h"

// Required for IMGUI
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
	m_pPressurePlate->GetTransform()->position = glm::vec2(2576.0f, 2325.0f);
	AddChild(m_pPressurePlate);

	// Box sprite
	m_pBox = new PushableObject();
	m_pBox->GetTransform()->position = glm::vec2(1155.0f, 1080.0f);
	AddChild(m_pBox);

	// Sniff thing
	m_pSniff = new Sniff();
	m_pSniff->GetTransform()->position = glm::vec2(0.0f, 0.0f);
	AddChild(m_pSniff, 11);

	// Lever Sprite (Area 1)
	m_pLever = new Lever();
	m_pLever->GetTransform()->position = glm::vec2(2100.0f, 2177.0f);
	AddChild(m_pLever);

	// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->SetMovementEnabled(true);
	m_pPlayer->GetTransform()->position = glm::vec2(800.0f, 2255.0f);
	AddChild(m_pPlayer, 10);
	m_playerFacingRight = true;

	// Elevator Sprite
	m_pElevator = new Elevator();
	m_pElevator->GetTransform()->position = glm::vec2(1363.0f, 1640);
	AddChild(m_pElevator);

	// Platform Handler
	// MUST BE CREATED AFTER PLAYER
	m_pPlatformHandler = new PlatformHandler();
	m_pPlatformHandler->SetGCCollider(m_pPlayer->GetCollider("groundCheck"));
	CreatePlatforms();

	// enemy
	m_pEnemy = new Enemy();
	m_pEnemy->GetTransform()->position = { 2576.0f + 240.0f, 2325.0f };
	m_pEnemy->GetTransform()->position.y -= m_pEnemy->GetHeight();
	m_pEnemy->SetHome(m_pEnemy->GetTransform()->position);
	AddChild(m_pEnemy, 3);

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

	if (m_pPlayer->GetInteracting() && m_playerCanActivateLever) {
		m_pLever->SetEnabled(!m_pLever->GetEnabled());
		m_appearingPlatformEnabled = true;
		std::cout << "You activated lever 1" << std::endl;

		for (auto platform : m_pPlatformHandler->GetPlatforms()) AddChild(platform);
		m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2725.0f, 1100), 100, 30));    //Appearing Platform 1 (Low)
		m_disappearingPlatforms.push_back(m_pPlatformHandler->GetPlatforms().at(m_pPlatformHandler->GetPlatforms().size() - 1));
		m_disappearingPlatforms.at(0)->leverIsActivated = true;
		
		m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2900.0f, 1250), 100, 30));    //Appearing Platform 2 (Mid)
		m_disappearingPlatforms.push_back(m_pPlatformHandler->GetPlatforms().at(m_pPlatformHandler->GetPlatforms().size() - 1));
		m_disappearingPlatforms.at(1)->leverIsActivated = true;
		
		m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2900.0f, 1000), 100, 30));    //Appearing Platform 2 (High)
		m_disappearingPlatforms.push_back(m_pPlatformHandler->GetPlatforms().at(m_pPlatformHandler->GetPlatforms().size() - 1));
		m_disappearingPlatforms.at(2)->leverIsActivated = true;
	}

	// arf
	if (m_pPlayer->m_barking) {

		auto dist = Util::distance(m_pPlayer->GetTransform()->position, m_pEnemy->GetTransform()->position);

		if (dist < 300.0f)
			m_pEnemy->Stun();
	}

	// Move camera to track player
	GetTransform()->position = m_pPlayer->GetTransform()->position - glm::vec2(760.0f, 550.0f);

	// Stop camera from moving out of bounds
	const int LEFT_BOUND = 0, RIGHT_BOUND = 4350, VERTICAL_BOUND = 0;
	if (GetTransform()->position.x < LEFT_BOUND) GetTransform()->position.x = LEFT_BOUND;
	if (GetTransform()->position.x > RIGHT_BOUND) GetTransform()->position.x = RIGHT_BOUND;
	if (GetTransform()->position.y < VERTICAL_BOUND) GetTransform()->position.y = VERTICAL_BOUND;
}

void PlayScene::Draw() {
	DrawDisplayList();
	//Debug key
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H)) {
		//Player Hitbox
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
		m_pPlatformHandler->Draw();
		m_pPlayer->GetCollider("groundCheck")->Draw();
	}

	// If the lever is activated the appearing platforms will be drawn 
	if (m_appearingPlatformEnabled) {
		for (int x = 0; x < m_disappearingPlatforms.size(); x++) {
			m_disappearingPlatforms[x]->Draw();		
		}
	}

	if (EventManager::Instance().isIMGUIActive()) {
		GUI_Function();
	}
}

void PlayScene::CreatePlatforms() {
	//Level 2 - Area 1 (Anything with an X is final position - DO NOT TOUCH)
	//Ground Platforms
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(720.0f, 2325.0f), 1000, 10));      //Ground Platform/Spawn X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2578.0f, 2325.0f), 480, 10));      //Ground Platform for Enemy/Cave X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(670.0f, 2282.0f), 52, 55));        //Ground Corner Platform (Left) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(460.0f, 2082.0f), 220, 255));      //Ground Platform Far Left X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(0.0f, 2038.0f), 483, 50));         //Ground Platform Far Far Left  X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1883.0f, 2240.0f), 656, 40));      //Ground Platform with Lever X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1722, 2282.0f), 194, 50));         //Ground Corner Platform (Right) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2523, 2282.0f), 55, 55));          //Ground Corner Platform (Right inside of Cave) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2453.0f, 2120), 75, 40));          //Ground Platform above cave entrance X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2529.0f, 2080), 3444, 50));        //Ground Platform under fire tower - Extends to Area 2X

	//Side/Wall/Fence Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(3057.0f, 2130), 20, 205));         //Side Platform for Enemy/Cave (Right) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(817.0f, 1408.0f), 5, 80));         //Fence Platform for Platform w/ Pressureplate (Left) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1057.0f, 1405.0f), 5, 80));        //Fence Platform for Platform w/ Pressureplate (Right) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(3042.0f, 1985), 40, 100));          //Watch Tower Support Beam (Left - Bottom) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(3042.0f, 890), 25, 910));          //Watch Tower Support Beam (Left) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(3617.0f, 900), 25, 1100));         //Watch Tower Support Beam (Right) X

	//Wooden Platforms
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1863.0f, 1877), 660, 10));         //Wood Platform with Lantern X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1363.0f, 1800), 200, 10));         //Wood Platform under Elevator X
	m_pElevator->setFloor(m_pPlatformHandler->GetPlatforms().at(m_pPlatformHandler->GetPlatforms().size() - 1));
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1363.0f, 1640), 200, 10));         //Wood Platform above Elevator x
	m_pElevator->setRoof(m_pPlatformHandler->GetPlatforms().at(m_pPlatformHandler->GetPlatforms().size() - 1));
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1560.0f, 1800), 200, 10));         //Wood Platform beside Elevator x
	//m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1400.0f, 1300), 100, 500));        //(Temp) Elevator Platform X 
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(792.0f, 1483.0f), 248, 10));       //Wood Platform with Two Pressureplates X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1036.0f, 1200.0f), 410, 10));      //Wood Platform with Telescope X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(1560.0f, 1280.0f), 354, 10));      //Wood Platform attached to Tree Platform (Right of Telescope) X
	//m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(2297.0f, 1315.0f), 387, 10));    //Wood Platform attached to Tree (Left of Fire Tower) X

	//Fire Tower Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(3020.0f, 884), 650, 10));         //Fire Tower Platform X

	//_____Level 2 - Area 2_____//
	//Ground Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4478.0f, 1560), 457, 10));         //Ground Platform above Area 3 (Left) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(5153.0f, 1560), 810, 10));         //Ground Platform above Area 3 with Lever(Right) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4900.0f, 1560), 300, 10));         //(Temp) Ground Platform above Area 3 (Middle) X

	//Wooden Structered Platforms
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4000.0f, 1960), 100, 30));         //Short Wooden Platform X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4120.0f, 1837), 100, 30));         //Medium Wooden Platform X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4240.0f, 1760), 100, 30));         //Medium-High Wooden Platform X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4320.0f, 1640), 100, 30));         //High Wooden Platform X

	//Wooden Platforms
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4425.0f, 880), 178, 10));          //Wooden Platform above Area 2 (Bottom) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4196.0f, 758), 178, 10));          //Wooden Platform above Area 2 (Mid) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(3962.0f, 640), 203, 10));          //Wooden Platform above Area 2 (Top Left) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4282.0f, 560), 523, 10));          //Wooden Platform above Area 2 (Top) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(5155.0f, 237), 355, 10));          //Wooden Platform above Area 2 (Top with Lever) X


	//Side/Wall Platform
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(4478.0f, 1560), 45, 518));         //Side Platform for Area 3 (Left) X
	m_pPlatformHandler->AddPlatform(new Platform(glm::vec2(5913.0f, 1560), 45, 518));         //Side Platform for Area 3 (Right) X

	for (auto platform : m_pPlatformHandler->GetPlatforms()) AddChild(platform);
}

void PlayScene::CollisionHandler() {

	bool soundPlayed = false;
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
	} else m_playerCanActivateLever = false;

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
	} else {
		m_pBox->SetEnabled(false);
		m_pPlayer->SetMaxSpeed(8.5f);
	}

	if (CollisionManager::AABBCheck(m_pElevator->GetFloor(), m_pPlayer)) {
		// true boolean = up, false bool = down
		if (m_pElevator->getDirection() && m_pElevator->TimerReady()) {
			//m_pPlayer->GetTransform()->position.y = m_pElevator->GetFloor()->GetTransform()->position.y;
			m_pPlayer->GetTransform()->position.y -= 10;
		} else if (!m_pElevator->getDirection() && m_pElevator->TimerReady()) {
			m_pPlayer->GetTransform()->position.y += 10;
		}
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

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_LSHIFT)) {
		m_pPlayer->SetMovementEnabled(false);
	}

	else {
		EventManager::Instance().isKeyUp(SDL_SCANCODE_LSHIFT);
		m_pPlayer->SetMovementEnabled(true);
	}
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