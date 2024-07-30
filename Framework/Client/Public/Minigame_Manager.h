#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CMinigame_Manager : public CBase
{
public:
	DECLARE_SINGLETON(CMinigame_Manager)

private:
	CMinigame_Manager();
	virtual ~CMinigame_Manager() = default;

public:
	void Set_FinishedGame(MINIGAME_TYPE eMinigameType) { m_bFinishedGame[eMinigameType] = true; }
	void Plus_GainNum(_uint iNum) { m_iGainNum += iNum; }

	_bool Get_FinishedGame(MINIGAME_TYPE eMinigameType) { return m_bFinishedGame[eMinigameType]; }
	_uint Get_GainNum() { return m_iGainNum; }	
	MINIGAME_TYPE Get_CurMinigame() { return (MINIGAME_TYPE)m_iCur_Minigame; }

public:
	HRESULT	Initialize();
	void	Notice_Minigame();
	void	Start_Minigame();
	void	Finish_Minigame(_uint iCurLevel = LEVEL_GAMEPLAY);

private:
	_uint	m_iCur_Minigame = MINIGAME_GLIDE;
	_uint	m_iGainNum = 0;

	_bool	m_bFinishedGame[MINIGAME_END] = {false, false};

private:
	void	Load_UIData(string strFileName);

public:
	virtual void Free();
};

END