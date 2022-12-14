#pragma once

#ifndef __ENGINE_ENUM_H__

enum COLLISION_GROUP
{
	COLLISION_PLAYER
	, COLLISION_LASER
	, COLLISION_TILE
	, COLLISION_PUSHBOX
	, COLLISION_SPRINGBOX
	, COLLISION_BALLOON
	, COLLISION_LADDER
	, COLLISION_DOOR
	, COLLISION_KEY
	, COLLISION_ROCK
	, COLLISION_SCULL
	, COLLISION_END
};

enum EVENT
{
	ALIVE,
	DEAD,
	EVENT_END
};

#define __ENGINE_ENUM_H__
#endif // !__ENGINE_ENUM_H__
