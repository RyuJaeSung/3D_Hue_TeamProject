#include "..\Headers\Sound_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CSound_Manager)

CSound_Manager::CSound_Manager()
{
	m_pSystem = nullptr;
}

HRESULT CSound_Manager::Ready_Sound_Manager()
{
	FMOD_System_Create(&m_pSystem);

	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();

	return S_OK;
}

void CSound_Manager::PlaySound(TCHAR * pSoundKey, CHANNELID eID)
{
	unordered_map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	}
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlayBGM(TCHAR * pSoundKey)
{
	unordered_map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[BGM], 0.3f);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSound_Manager::StopAll()
{
	for (int i = 0; i < CHANNELID_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Manager::LoadSoundFile()
{
	_finddata_t fd;

	long handle = _findfirst("../../Client/Resources/Sound/*.*", &fd);

	if (handle == 0)
		return;

	int iResult = 0;

	char szCurPath[128] = "../../Client/Resources/Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _findnext(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}

void CSound_Manager::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
