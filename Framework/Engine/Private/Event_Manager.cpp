#include "Event_Manager.h"
#include "Level_Manager.h"
#include "Level.h"
#include "GameObject.h"
#include "Object_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CEvent_Manager);

CEvent_Manager::CEvent_Manager()
{
}

void CEvent_Manager::Tick(_float fTimeDelta)
{
	// Defualt Event cycle
	for (_uint i = 0; i < m_vecEvent.size(); ++i)
	{
		for (auto& elem : m_Subscriber[m_vecEvent[i].eEventType])
		{
			elem->On_EventMessege(m_vecEvent[i]);
			elem->Push_EventMessege(m_vecEvent[i]);
		}
		//Excute(m_vecEvent[i]);
	}
	m_vecEvent.clear();




	// Dead , Alive cycle
	for (auto& elem : m_vecDead)
	{
		Safe_Release(elem);
	}
	m_vecDead.clear();

	for (auto& elem : m_vecAlive)
	{
		elem->Set_Dead();
		m_vecDead.push_back(elem);
	}
	m_vecAlive.clear();


	// Create cycle


}

void CEvent_Manager::LateTick(_float fTimeDelta)
{
	// Defualt Event cycle
	auto iter = m_vecEvent.begin();
	for (; iter != m_vecEvent.end();)
	{
		for (auto& elem : m_Subscriber[(*iter).eEventType])
		{
			if (EVENT_PICKING == (*iter).eEventType)
			{
				elem->On_EventMessege(*iter);
				elem->Push_EventMessege(*iter);
			}
		}
		if (EVENT_PICKING == (*iter).eEventType)
			iter = m_vecEvent.erase(iter);
		else
			++iter;

		//Excute(m_vecEvent[i]);
	}

}

void CEvent_Manager::Add_Event(EVENT_TYPE _eEventType, void* _lParam, void* _wParam)
{
	m_vecEvent.push_back(EVENTDESC(_eEventType, _lParam, _wParam));
}

void CEvent_Manager::Change_Level(_uint iLevelIndex, CLevel* pNextLevel)
{
	EVENTDESC	tEvent;
	tEvent.eEventType = LEVEL_CHANGE;
	tEvent.lParam = (void*)iLevelIndex;
	tEvent.wParam = (void*)pNextLevel;
	Add_Event(tEvent);
}

//void CEvent_Manager::Add_DeadObject(CBase* pObject)
//{
//	for (auto& pBase : m_vecAlive)
//	{
//		if (pObject == pBase)
//			return;
//	}
//
//	EVENTDESC	tEvent;
//	tEvent.eEventType = OBJECT_DEAD;
//	tEvent.lParam = (CGameObject*)pObject;
//	tEvent.wParam = nullptr;
//	Add_Event(tEvent);
//
//	m_vecAlive.push_back(pObject);
//	pObject->On_Dead();
//	pObject->Set_Disable();
//	pObject->Set_Dead();
//
//}

void CEvent_Manager::Add_DeadBase(CBase* pObject)
{
	for (auto& pBase : m_vecAlive)
	{
		if (pObject == pBase)
			return;
	}

	m_vecAlive.push_back(pObject);
	pObject->On_Dead();
	pObject->Set_Disable();
	pObject->Set_Dead();

}

//void CEvent_Manager::Add_PoolingObject(CGameObject* pObject)
//{
//	EVENTDESC	tEvent;
//	tEvent.eEventType = OBJECT_POOLING;
//	tEvent.lParam = pObject;
//	tEvent.wParam = nullptr;
//	Add_Event(tEvent);
//
//	pObject->On_Dead();
//	pObject->Set_Disable();
//	pObject->Set_Dead();
//}

void CEvent_Manager::Add_CallbackList(CBase* pBase, EVENT_TYPE eEventType)
{
	for (auto& elem : m_Subscriber[eEventType])
	{
		if (elem == pBase)
			return;
	}

	m_Subscriber[eEventType].push_back(pBase);
}

void CEvent_Manager::Remove_CallbackList(CBase* pBase, EVENT_TYPE eEventType)
{
	m_Subscriber[eEventType].remove(pBase);
}

void CEvent_Manager::Key_Event_Check()
{
	USEGAMEINSTANCE;
	if (KEY_DOWN(VK_LBUTTON))
	{

	}
	if (KEY_DOWN(VK_RBUTTON))
	{

	}
	if (pGameInstance->Get_DIMouseMoveState(MMS_WHEEL))
	{

	}
}


//void CEvent_Manager::Excute(const EventDesc & tEvent)
//{

	//for (auto& elem : m_Subscriber[tEvent.eEventType])
	//{
	//	elem->On_EventMessege(tEvent);
	//}




	//switch (EventDesc.eEventType)
	//{
	//case EVENT_TYPE::LEVEL_CHANGE:
	//	// lParam : Next Scene Type
	//	//CLevel_Manager::Get_Instance()->Open_Level((_uint)EventDesc.lParam, (CLevel*)EventDesc.wParam);
	//	for (auto& elem : m_Subscriber[LEVEL_CHANGE])
	//	{
	//		elem->On_EventMessege(EventDesc);
	//	}
	//	break;

	//case EVENT_TYPE::CREATE_OBJECT:
	//{
	//	// lParam : Object Adress
	//	// wParam : Group Type
	//	tCreateObjectDesc tCreateObjectDesc;
	//	memcpy(&tCreateObjectDesc, EventDesc.wParam, sizeof(tCreateObjectDesc));
	//	CObject_Manager::Get_Instance()->Add_GameObject((_uint)EventDesc.lParam, EventDesc.wParam);

	//}
	//break;
	//case EVENT_TYPE::DELETE_OBJECT:
	//{
	//	// lParam : Object Adress
	//	// Object 를 Dead 상태로 변경
	//	// 삭제 예정 오브젝트들을 모아둔다.
	//	CObj* pDeadObj = (CObj*)EventDesc.lParam;
	//	pDeadObj->SetDead();
	//	m_vecDead.push_back(pDeadObj);
	//}
	//break;


	//}
//}

void CEvent_Manager::Free()
{
	m_vecEvent.clear();

	for (auto& pObject : m_vecDead)
	{
		Safe_Release(pObject);
	}

	for (auto& pObject : m_vecAlive)
	{
		Safe_Release(pObject);
	}

	__super::Free();
}