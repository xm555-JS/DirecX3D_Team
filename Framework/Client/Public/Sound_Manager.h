#pragma once
//#include "../FMOD/include/fmod.h"
#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "../Bin/FMOD/fmod.h"
#include "GameObject.h"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSound_Manager
{
public:
	static CSound_Manager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSound_Manager;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
public:
	enum CHANNELID
	{
		BGM,
		CH1,
		CH2,
		CH3,
		CH4,
		CH5,
		CH6,
		CH7,
		CH8,
		CH9,
		CH10,
		CH11,
		CH12,
		CH13,
		CH14,
		CH15,
		CH16,
		CH17,
		CH18,
		CH19,
		CH20,
		CH21,
		CH22,
		CH23,
		CH24,
		CH25,
		CH26,
		CH27,
		CH28,
		CH29,
		CH30,
		CH31,
		CH_END
	};

	enum CHANNEL_TYPE {
		CHANNEL_BGM,
		CHANNEL_UI,
		CHANNEL_PLAYER,
		CHANNEL_PLAYER_EFFECT,
		CHANNEL_NPC,
		CHANNEL_ENEMY,
		CHANNEL_ENEMY_EFFECT,
		CHANNEL_ENV_DISTANCE,
		CHANNEL_ENVIRONMENT,

		CHANNEL_END
	};


private:
	enum SOUND_DIR
	{
		CENTER,
		BACKRIGHT,
		BACKLEFT,
		SIDELEFT,
		FRONTLEFT,
		FRONTRIGHT,
		SIDERIGHT,

		DIR_END,
	};

private:
	CSound_Manager();
	~CSound_Manager();

public:
	HRESULT Initialize();

	void Release();
public:
	int  Set_Volume(CHANNELID eID, _float _vol);
	int  VolumeUp(CHANNELID eID, _float _vol);
	int  VolumeDown(CHANNELID eID, _float _vol);
	int  BGMVolumeUp(_float _vol);
	int  BGMVolumeDown(_float _vol);
	int  Pause(CHANNELID eID);
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float _vol);
	void PlayBGM(TCHAR* pSoundKey);
	void PlayBGM(TCHAR* pSoundKey, _float fVolume);
	void FadeOutBGM(_float fSoundDelta = 1.f);
	void StopSound(CHANNELID eID);
	void StopAll();


	CHANNELID		PlaySound(const TCHAR* strSoundKey, CHANNEL_TYPE eType, _float fVolume = 1.f, _bool bLoop = false);
	CHANNELID		PlaySurroundSound(const TCHAR* strSoundKey, CHANNEL_TYPE eType, _vector vSpeakerPos, _float fVolume = 1.f);
	void		PlaySound(const TCHAR* strSoundKey, CHANNEL_TYPE eType, _vector vListenerPos, _vector vSpeakerPos, _vector vRight, _float fVolume = 1.f);
	void		ChangeBGM(TCHAR * pSoundKey, _float fVolume = 1.f, _float fSoundDelta = 1.f);
	void		Tick(_float fTimeDelta);
	CHANNELID	Get_CurChannel(CHANNEL_TYPE etype) { return m_eCurChannel[etype]; }
	void			SetUp_FadeInSound(CHANNELID ID, _float fSound, _float fSoundDelta);
	void			SetUp_FadeOutSound(CHANNELID ID, _float fSound, _float fSoundDelta);


private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;

private:
	void LoadSoundFile(string strPath);

private:
	static CSound_Manager* m_pInstance;

	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL*			m_pChannelArr[CH_END];
	FMOD_CHANNELGROUP*		m_pChannelGroupArr[CH_END];

	CHANNELID		m_eCurChannel[CHANNEL_END];
	CHANNELID		m_eChannelMaxNum[CHANNEL_END];
	CHANNELID		m_eChannelMinNum[CHANNEL_END];


	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM*	m_pSystem;
	_bool			m_bPause = false;

	_float			m_fSound = 0.f;
	_float			m_fSoundDelta = 0.f;
	_float			m_fSoundMaxDistance = 0.f;
	_float			m_fMaxBGMVolume = 1.f;
	_float			m_fStandardMess = 0.f;


	_bool			m_bBGMOut = false;
	TCHAR *			m_NextSound;

private:
	_matrix			m_RotationMatrix;


	_float4			m_vCameraRight;
	_float4			m_vCamearPos;

private:
	_bool			m_bBGMChange = false;

private:
	//bState, TargetSound, SoundDelta
	tuple<_bool, _float, _float>		m_pChannelFadeInState[CH_END];
	tuple<_bool, _float, _float>		m_pChannelFadeOutState[CH_END];

private:
	void			Setup_CamState();
	void			Tick_FadeSound(_float fTimeDelta);

private:
	void			SetUp_Chennel(_uint* pNumChannels);



};

