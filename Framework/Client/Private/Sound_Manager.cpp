#include "stdafx.h"
//#include"../FMOD/include/fmod.hpp"
//#include"../FMOD/include/fmod_errors.h"
#include "../Public/Sound_Manager.h"
#include "../Bin/FMOD/fmod.hpp"
#include "../Bin/FMOD/fmod_errors.h"

#include "GameInstance.h"
#include "Camera.h"

CSound_Manager* CSound_Manager::m_pInstance = nullptr;
CSound_Manager::CSound_Manager()
{
	m_pSystem = nullptr;
	m_NextSound = nullptr;
}


CSound_Manager::~CSound_Manager()
{
	Release();
}

HRESULT CSound_Manager::Initialize()
{
	m_fSound = 1.f;
	m_fSoundDelta = 1.f;
	m_bBGMOut = false;
	m_fSoundMaxDistance = 15.f;
	m_fStandardMess = 1.f;
	m_RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(60.f));

	// Fade 객체 초기화
	for (size_t i = 0; i < CH_END; ++i)
	{
		get<0>(m_pChannelFadeInState[i]) = false;
		get<1>(m_pChannelFadeInState[i]) = 0.f;
		get<0>(m_pChannelFadeOutState[i]) = false;
		get<1>(m_pChannelFadeOutState[i]) = 0.f;
	}


	//ZeroMemory(m_eCurChannel, sizeof(m_eCurChannel));
	//ZeroMemory(m_eCurChannel, sizeof(m_eCurChannel));

	// 다 더해서 32개 만들어야함
	_uint	iNumChennel[CHANNEL_END];
	iNumChennel[CHANNEL_BGM] = 1;
	iNumChennel[CHANNEL_UI] = 2;
	iNumChennel[CHANNEL_NPC] = 1;
	iNumChennel[CHANNEL_PLAYER] = 4;
	iNumChennel[CHANNEL_PLAYER_EFFECT] = 4;
	iNumChennel[CHANNEL_ENEMY] = 4;
	iNumChennel[CHANNEL_ENEMY_EFFECT] = 4;
	iNumChennel[CHANNEL_ENV_DISTANCE] = 6;
	iNumChennel[CHANNEL_ENVIRONMENT] = 6;

	SetUp_Chennel(iNumChennel);

	FMOD_System_Create(&m_pSystem);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FAILEDCHECK(FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL));
	//FAILEDCHECK(FMOD_System_Init(m_pSystem, 32, FMOD_INIT_DISABLEDOLBY, NULL));

	stringvec vecPath;
	string strBasePath = "../Bin/Sound/";
	read_directory(strBasePath.c_str(), vecPath);
	for(auto& strPath : vecPath)
		LoadSoundFile(strBasePath + strPath + "/");


	for (size_t i = 0; i < 32; ++i)
	{
		string	GroupName = "ChennelGruop" + to_string(i);
		FMOD_System_CreateChannelGroup(m_pSystem, GroupName.c_str(), &m_pChannelGroupArr[i]);
	}

	return S_OK;
}
void CSound_Manager::Release()
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

int CSound_Manager::Set_Volume(CHANNELID eID, _float _vol)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], _vol);

	return 0;
}

int CSound_Manager::VolumeUp(CHANNELID eID, _float _vol)
{
	if (m_volume < SOUND_MAX) {
		m_volume += _vol;
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_volume);

	return 0;
}

int CSound_Manager::VolumeDown(CHANNELID eID, _float _vol)
{
	if (m_volume > SOUND_MIN) {
		m_volume -= _vol;
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_volume);

	return 0;
}

int CSound_Manager::BGMVolumeUp(_float _vol)
{
	if (m_BGMvolume < SOUND_MAX) {
		m_BGMvolume += _vol;
	}

	FMOD_Channel_SetVolume(m_pChannelArr[BGM], m_BGMvolume);

	return 0;
}

int CSound_Manager::BGMVolumeDown(_float _vol)
{
	if (m_BGMvolume > SOUND_MIN) {
		m_BGMvolume -= _vol;
	}

	FMOD_Channel_SetVolume(m_pChannelArr[BGM], m_BGMvolume);

	return 0;
}

int CSound_Manager::Pause(CHANNELID eID)
{
	m_bPause = !m_bPause;
	FMOD_Channel_SetPaused(m_pChannelArr[eID], m_bPause);

	return 0;
}


void CSound_Manager::PlaySound(TCHAR * pSoundKey, CHANNELID eID, _float _vol)
{
	if (BGM == eID)
		return;

	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem,FMOD_CHANNEL_FREE,iter->second, FALSE, &m_pChannelArr[eID]);
		if (_vol >= SOUND_MAX)
			_vol = 1.f;
		else if (_vol <= SOUND_MIN)
			_vol = 0.f;
		FMOD_Channel_SetVolume(m_pChannelArr[eID], _vol);
	}
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlayBGM(TCHAR * pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;
	
	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlayBGM(TCHAR * pSoundKey, _float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
	{
		__debugbreak();
		return;
	}


	//fVolume = 0.1f;
	//m_fMaxBGMVolume = fVolume;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[BGM], fVolume);
	FMOD_System_Update(m_pSystem);

}

void CSound_Manager::FadeOutBGM(_float fSoundDelta)
{
	m_bBGMOut = true;
	m_fSoundDelta = fSoundDelta;
	m_fMaxBGMVolume = 0.f;
}

void CSound_Manager::StopSound(CHANNELID eID)
{
	if (BGM == eID)
		return;

	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSound_Manager::StopAll()
{
	for (int i = 0; i < CH_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}


CSound_Manager::CHANNELID CSound_Manager::PlaySound(const TCHAR * strSoundKey, CHANNEL_TYPE eType, _float fVolume, _bool bLoop)
{
 	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
	{
		return !lstrcmp(strSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		__debugbreak();


	FMOD_BOOL bPlay = FALSE;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[m_eCurChannel[eType]]);
	FMOD_Channel_SetVolume(m_pChannelArr[UINT(m_eCurChannel[eType])], fVolume);
	if(true == bLoop)
		FMOD_Channel_SetMode(m_pChannelArr[UINT(m_eCurChannel[eType])], FMOD_LOOP_NORMAL);
	CHANNELID	iCurChennelID = m_eCurChannel[eType];

	if (m_eCurChannel[eType] >= m_eChannelMaxNum[eType])
	{
		m_eCurChannel[eType] = m_eChannelMinNum[eType];
	}
	else
	{
		m_eCurChannel[eType] = (CHANNELID)(m_eCurChannel[eType] + 1);
	}


	FMOD_System_Update(m_pSystem);

	return iCurChennelID;
}

CSound_Manager::CHANNELID CSound_Manager::PlaySurroundSound(const TCHAR * strSoundKey, CHANNEL_TYPE eType, _vector vSpeakerPos, _float fVolume)
{
	_vector vListenerPos = XMLoadFloat4(&m_vCamearPos);

	_vector	vTargetDir = XMVector3Normalize(vSpeakerPos - vListenerPos);
	_float	fTargetLength = XMVectorGetX(XMVector3Length(vSpeakerPos - vListenerPos)) * 0.5f;

	CHANNELID	iCurChennelID = m_eCurChannel[eType];

	if (fTargetLength > m_fSoundMaxDistance)
		return iCurChennelID;

	_float	fDistanceRange = fTargetLength / m_fSoundMaxDistance;



	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
	{
		return !lstrcmp(strSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
	{
		__debugbreak();
		return iCurChennelID;
	}

	FMOD_BOOL bPlay = FALSE;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[m_eCurChannel[eType]]);
	FMOD_Channel_SetVolume(m_pChannelArr[UINT(m_eCurChannel[eType])], fVolume);

	_float	fDirs[DIR_END];
	_vector vDirs[DIR_END];
	ZeroMemory(&fDirs, sizeof(_float) * DIR_END);
	ZeroMemory(&vDirs, sizeof(_float) * DIR_END);



	if (1.f < fDistanceRange)
		fDistanceRange = 1.f;

	_float	fTotalVolume = 0.f;
	//1.f ~ 0.f;
	fDistanceRange = 1.f - fDistanceRange;
	fDirs[CENTER] = fDistanceRange;
	//fTotalVolume += fDistanceRange;
	//vDirs[SIDERIGHT] = XMVector3Normalize(vRight);
	_vector vRight;
	vRight = XMLoadFloat4(&m_vCameraRight);

	for (size_t i = 1; i < DIR_END; ++i)
	{
		vRight = XMVector3TransformNormal(vRight, m_RotationMatrix);
		vDirs[i] = vRight;
		_float	fVolume = XMVectorGetX(XMVector3Dot(vRight, vTargetDir));
		if (0.f < fVolume)
		{
			fDirs[i] = fVolume * fDistanceRange;
			fTotalVolume += fVolume;
		}
	}

	// normalize 과정
	_uint	err;
	err = FMOD_Channel_SetSpeakerMix(m_pChannelArr[UINT(m_eCurChannel[eType])],
		fDirs[FRONTLEFT] / fTotalVolume,
		fDirs[FRONTRIGHT] / fTotalVolume,
		fDirs[CENTER] / fTotalVolume,
		0.f,
		fDirs[BACKLEFT] / fTotalVolume,
		fDirs[BACKRIGHT] / fTotalVolume,
		fDirs[SIDELEFT] / fTotalVolume,
		fDirs[SIDERIGHT] / fTotalVolume
	);

	if (m_eCurChannel[eType] >= m_eChannelMaxNum[eType])
	{
		m_eCurChannel[eType] = m_eChannelMinNum[eType];
	}
	else
	{
		m_eCurChannel[eType] = (CHANNELID)(m_eCurChannel[eType] + 1);
	}


	FMOD_System_Update(m_pSystem);

	return iCurChennelID;
}

void CSound_Manager::PlaySound(const TCHAR * strSoundKey, CHANNEL_TYPE eType, _vector vListenerPos, _vector vSpeakerPos, _vector vRight, _float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
	{
		return !lstrcmp(strSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
	{
		__debugbreak();
		return;
	}
		
	FMOD_BOOL bPlay = FALSE;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[m_eCurChannel[eType]]);
	FMOD_Channel_SetVolume(m_pChannelArr[UINT(m_eCurChannel[eType])], fVolume);

	_float	fDirs[DIR_END];
	_vector vDirs[DIR_END];
	ZeroMemory(&fDirs, sizeof(_float) * DIR_END);
	ZeroMemory(&vDirs, sizeof(_float) * DIR_END);


	_vector	vTargetDir = XMVector3Normalize(vSpeakerPos - vListenerPos);
	_float	fTargetLength = XMVectorGetX(XMVector3Length(vSpeakerPos - vListenerPos));
	_float	fDistanceRange = fTargetLength / m_fSoundMaxDistance;
	if (1.f < fDistanceRange)
		fDistanceRange = 1.f;

	_float	fTotalVolume = 0.f;
	//1.f ~ 0.f;
	fDistanceRange = 1.f - fDistanceRange;
	fDirs[CENTER] = fDistanceRange;
	//fTotalVolume += fDistanceRange;
	//vDirs[SIDERIGHT] = XMVector3Normalize(vRight);
	for (size_t i = 1; i < DIR_END; ++i)
	{
		vRight = XMVector3TransformNormal(vRight, m_RotationMatrix);
		vDirs[i] = vRight;
		_float	fVolume = XMVectorGetX(XMVector3Dot(vRight, vTargetDir));
		if (0.f < fVolume)
		{
			fDirs[i] = fVolume * fDistanceRange;
			fTotalVolume += fVolume;
		}
	}

	// normalize 과정
	_uint	err;
	err = FMOD_Channel_SetSpeakerMix(m_pChannelArr[UINT(m_eCurChannel[eType])],
		fDirs[FRONTLEFT] / fTotalVolume,
		fDirs[FRONTRIGHT] / fTotalVolume,
		fDirs[CENTER] / fTotalVolume,
		0.f,
		fDirs[BACKLEFT] / fTotalVolume,
		fDirs[BACKRIGHT] / fTotalVolume,
		fDirs[SIDELEFT] / fTotalVolume,
		fDirs[SIDERIGHT] / fTotalVolume
	);

	if (m_eCurChannel[eType] >= m_eChannelMaxNum[eType])
	{
		m_eCurChannel[eType] = m_eChannelMinNum[eType];
	}
	else
	{
		m_eCurChannel[eType] = (CHANNELID)(m_eCurChannel[eType] + 1);
	}

	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::ChangeBGM(TCHAR * pSoundKey, _float fVolume, _float fSoundDelta)
{
	m_bBGMOut = true;
	m_NextSound = pSoundKey;
	m_fMaxBGMVolume = fVolume;
	m_fSoundDelta = fSoundDelta;
	m_bBGMChange = true;
}

void CSound_Manager::Tick(_float fTimeDelta)
{
	Setup_CamState();

	if (m_bBGMChange)
	{
		_float fVolume = 0.f;
		FMOD_Channel_GetVolume(m_pChannelArr[0], &fVolume);
		fVolume -= fTimeDelta * m_fSoundDelta;

		if (0.f > fVolume)
		{
			fVolume = 0.f;
			m_bBGMChange = false;
			StopSound(CHANNELID::BGM);
			PlayBGM(m_NextSound, 0.f);
		}
		FMOD_Channel_SetVolume(m_pChannelArr[0], fVolume);
	}
	else
	{
		_float fVolume = 0.f;
		FMOD_Channel_GetVolume(m_pChannelArr[0], &fVolume);
		fVolume += fTimeDelta * m_fSoundDelta;

		if (m_fMaxBGMVolume < fVolume)
		{
			fVolume = m_fMaxBGMVolume;
		}
		FMOD_Channel_SetVolume(m_pChannelArr[0], fVolume);
	}


	Tick_FadeSound(fTimeDelta);
}


void CSound_Manager::LoadSoundFile(string strPath)
{
	wstring	wstrPath = towstring(strPath);
	wstrPath = wstrPath + L"*.*";

	_tfinddata_t fd;
	__int64 handle = _tfindfirst(wstrPath.c_str(), &fd);
	//__int64 handle = _tfindfirst(L"../Bin/Sound/*.*", &fd);
	if (handle == -1 || handle == 0)
		return;

	int iResult = 0;

	//char szCurPath[128] = "../Bin/Sound/";
	//char szCurPath[128] = "../Bin/Sound/";
	char szFullPath[128] = "";
	char szFilename[MAX_PATH];
	while (iResult != -1)
	{
		WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		strcpy_s(szFullPath, strPath.c_str());
		//strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, szFilename);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);
		if (eRes == FMOD_OK)
		{
			size_t iLength = strlen(szFilename) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, szFilename, (int)iLength, pSoundKey, (int)iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _tfindnext(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);

	_findclose(handle);
}


void CSound_Manager::Setup_CamState()
{
	USEGAMEINSTANCE;
	CCamera* pCurCamera = pGameInstance->Get_CurrentCamera();
	if (pCurCamera)
	{
		CTransform* pCameraTransform = (CTransform*)pCurCamera->Get_Component(TRANSFORM);
		XMStoreFloat4(&m_vCameraRight, XMVector3Normalize(pCameraTransform->Get_State(CTransform::STATE_RIGHT)));
		m_vCamearPos = pGameInstance->Get_CamPosition();
	}
}

void CSound_Manager::Tick_FadeSound(_float fTimeDelta)
{
	for (size_t i = 0; i < CH_END; ++i)
	{
		if(true == get<0>(m_pChannelFadeInState[i]))
		{
			_float	fCurSound = 0.f;
			FMOD_Channel_GetVolume(m_pChannelArr[i], &fCurSound);

			get<1>(m_pChannelFadeInState[i]) = fCurSound + get<2>(m_pChannelFadeInState[i])  * fTimeDelta;
			if (0.f > get<1>(m_pChannelFadeInState[i]))
				get<1>(m_pChannelFadeInState[i]) = 0.f;

			FMOD_Channel_SetVolume(m_pChannelArr[i], get<1>(m_pChannelFadeInState[i]));

			if (fCurSound > get<1>(m_pChannelFadeInState[i]))
			{
				get<0>(m_pChannelFadeInState[i]) = false;
				get<1>(m_pChannelFadeInState[i]) = 0.f;
				get<2>(m_pChannelFadeInState[i]) = 0.f;
			}
		}
		else if (true == get<0>(m_pChannelFadeOutState[i]))
		{
			_float	fCurSound = 0.f;
			FMOD_Channel_GetVolume(m_pChannelArr[i], &fCurSound);

			get<1>(m_pChannelFadeOutState[i]) = fCurSound - get<2>(m_pChannelFadeOutState[i]) * fTimeDelta;
			if (0.f > get<1>(m_pChannelFadeOutState[i]))
				get<1>(m_pChannelFadeOutState[i]) = 0.f;

			FMOD_Channel_SetVolume(m_pChannelArr[i], get<1>(m_pChannelFadeOutState[i]));

			if (fCurSound < get<1>(m_pChannelFadeOutState[i]))
			{
				get<0>(m_pChannelFadeOutState[i]) = false;
				get<1>(m_pChannelFadeOutState[i]) = 0.f;
				get<2>(m_pChannelFadeOutState[i]) = 0.f;
			}
		}
	}
}

void CSound_Manager::SetUp_FadeInSound(CHANNELID ID, _float fSound, _float fSoundDelta)
{
	get<0>(m_pChannelFadeInState[ID]) = true;
	get<1>(m_pChannelFadeInState[ID]) = fSound;
	get<2>(m_pChannelFadeInState[ID]) = fSoundDelta;
}

void CSound_Manager::SetUp_FadeOutSound(CHANNELID ID, _float fSound, _float fSoundDelta)
{
	get<0>(m_pChannelFadeOutState[ID]) = true;
	get<1>(m_pChannelFadeOutState[ID]) = fSound;
	get<2>(m_pChannelFadeOutState[ID]) = fSoundDelta;
}



// 한번 확인해봐
void CSound_Manager::SetUp_Chennel(_uint * pNumChannels)
{
	_uint	iIdx = 0;
	for (size_t i = 0; i < CHANNEL_END; ++i)
	{
		m_eChannelMinNum[i] = (CHANNELID)iIdx;
		m_eCurChannel[i] = (CHANNELID)iIdx;

		iIdx += pNumChannels[i];
		m_eChannelMaxNum[i] = (CHANNELID)(iIdx -1);

	}




	// ChennelGroup 지어주는 부분
	//for (size_t i = m_eChannelMinNum[CHANNEL_OBJECT]; i <  m_eChannelMaxNum[CHANNEL_OBJECT]; ++i)
	//	FMOD_Channel_SetChannelGroup(m_pChannelArr[i], m_pChannelGroupArr[i]);

	if (32 < iIdx)
		__debugbreak();
}
