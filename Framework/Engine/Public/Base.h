#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	unsigned long AddRef();
	unsigned long Release();

	virtual void	On_EventMessege(EVENTDESC tEvent) {};
	void			Push_EventMessege(EVENTDESC tEvent) { m_Events.push_back(tEvent); };
	virtual void	On_Dead() {};


public:
	_bool	Is_Dead() { return m_bDead; }
	_bool	Is_Alive() { return !m_bDead; }
	_bool	Is_Disable() { return m_bDisable; }
	_bool	Is_Able() { return !m_bDisable; }
	_bool	Is_Clone() { return m_bClone; }


	void	Set_Dead() { m_bDead = true; }
	void	Set_Alive() { m_bDead = false; }
	void	Set_Disable() { m_bDisable = true; }
	void	Set_Able() { m_bDisable = false; }
	void	Set_Clone() { m_bClone = true; }

protected:
	unsigned long		m_dwRefCnt = 0;
	unsigned long		m_dwGarbageCnt = 0;


	list<EVENTDESC>		m_Events;


protected:
	_bool						m_bDead = false;
	_bool						m_bDisable = false;
	_bool						m_bClone = false;

public:
	virtual void Free();
};

END

