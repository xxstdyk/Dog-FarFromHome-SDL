#pragma once
#ifndef __SETTING_SCENE__
#define __SETTING_SCENE__

#include "Scene.h"
#include "Label.h"
#include "Button.h"

class SettingScene final : public Scene
{
public:
	SettingScene();
	~SettingScene();

	// Inherited via Scene
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;

private:
	Label* m_settinglabel{};
	Button* m_resolutionButton;
	Button* m_backButton;
	Button* m_soundButton;
	Button* m_controlsButton;
};

#endif /* defined (__END_SCENE__) */