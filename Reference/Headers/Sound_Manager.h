#pragma once

#ifndef __SOUND_MANAGER_H__

#include "Base.h"
BEGIN(Engine)

class CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)
public:
	enum CHANNELID { BGM, PLAYER, PLAYER_JUMP, PLAYER_DEAD, PLAYER_REVIVE , KEY, ITEM, LADDER, SCULL_SHAKE, SCULL_COL, SPRING_BOX, PUSH_BOX, BULLON_BOX, DOOR, LASER, ROCK , COLOR_OPEN, COLOR_CLOSE, CHANNELID_END };

private:
	explicit CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT Ready_Sound_Manager();

public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	void LoadSoundFile();

public:
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

private:
	// 사운드 리소스 정보를 갖는 객체 
	unordered_map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[CHANNELID_END];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;
};

END

#define __SOUND_MANAGER_H__
#endif // !__SOUND_MANAGER_H__
