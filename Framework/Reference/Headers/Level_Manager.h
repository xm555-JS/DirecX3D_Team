#pragma once

#include "Base.h"

/* 현재 게임내에 보여줘야할 레벨을 보관한다. */
/* 레벨교체시에 기존 레벨을 삭제한다. */
/* 기존레벨에서 사용했던 객체와 자원(리소스등)을 정리한다. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
public:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	_uint			Get_CurLevelIndex() { return m_iCurrentLevelID; }
	class CLevel*	Get_CurLevel() {return m_pCurrentLevel;}

public:
	HRESULT		Initialize();
	void		Tick(_float fTimeDelta);
	void		LateTick(_float fTimeDelta);
	HRESULT		Render();



public:
	virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	class CLevel*			m_pCurrentLevel = nullptr;
	_uint					m_iCurrentLevelID = 0;

public:
	virtual void Free() override;
};

END