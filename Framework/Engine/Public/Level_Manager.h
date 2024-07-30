#pragma once

#include "Base.h"

/* ���� ���ӳ��� ��������� ������ �����Ѵ�. */
/* ������ü�ÿ� ���� ������ �����Ѵ�. */
/* ������������ ����ߴ� ��ü�� �ڿ�(���ҽ���)�� �����Ѵ�. */

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