#pragma once
#include "Base.h"
#include "Delegate.h"

BEGIN(Engine)

class CLevel;
class CGameObject;
class CComponent;

class ENGINE_DLL CEvent_Manager : public CBase
{
public:

	DECLARE_SINGLETON(CEvent_Manager)


private:
	CEvent_Manager();
	virtual ~CEvent_Manager() = default;


public:
	void	Tick(_float fTimeDelta);
	void	LateTick(_float fTimeDelta);

public:
	void	Add_Event(const EVENTDESC& tEvent) { m_vecEvent.push_back(tEvent); }
	void	Add_Event(EVENT_TYPE _eEventType, void* _lParam, void* _wParam);
	void	Change_Level(_uint iLevelIndex, CLevel* pNextLevel);

	//void	Add_DeadObject(CBase* pObject);
	void	Add_DeadBase(CBase* pObject);
	//void	Add_PoolingObject(CGameObject* pObject);

	void	Add_CallbackList(CBase* pBase, EVENT_TYPE eEventType);
	void	Remove_CallbackList(CBase* pBase, EVENT_TYPE eEventType);


private:
	void	Key_Event_Check();

private:
	vector<EVENTDESC>		m_vecEvent;

	vector<CBase*>			m_vecAlive;
	vector<CBase*>			m_vecDead;

	list<CBase*>			m_Subscriber[EVENT_END];


public:
	virtual void Free() override;

};

END