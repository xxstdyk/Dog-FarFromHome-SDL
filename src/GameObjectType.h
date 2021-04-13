#pragma once
#ifndef __GAME_OBJECT_TYPE__
#define __GAME_OBJECT_TYPE__

enum GameObjectType {
	NONE = -1,
	PLAYER,
	ENEMY,
	LEVER,
	BLACK_LEVER,
	PRESSURE_PLATE,
	PLATFORM,
	PUSHABLE_OBJECT,
	START_BUTTON,
	SETTINGS_BUTTON,
	RESTART_BUTTON,
	BACK_BUTTON,
	NEXT_BUTTON,
	NUM_OF_TYPES
};
#endif /* defined (__GAME_OBJECT_TYPE__) */