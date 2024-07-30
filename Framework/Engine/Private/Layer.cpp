#include "..\Public\Layer.h"
#include "GameObject.h"
#include "Event_Manager.h"
#include "Object_Manager.h"

CLayer::CLayer()
{
}

CComponent* CLayer::Get_Component(ID_UINT iComponentID, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Component(iComponentID);
}

CGameObject* CLayer::Get_Object(_uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter);
}

CGameObject* CLayer::Pooling_Object(ID_UINT tPrototypeID, void* pArg)
{
	CGameObject* pPoolingObject = nullptr;
	for (auto& pObject : m_GameObjects)
	{
		if (tPrototypeID.iID == pObject->Get_PrototypeID())
		{
			pPoolingObject = pObject;
			break;
		}
	}

	if (pPoolingObject)
	{
		pPoolingObject->Initialize_Pooling(pArg);
		pPoolingObject->SetUp_PoolingState(m_iCurLevel, m_tLayerID.iID, tPrototypeID.iID);
		m_GameObjects.remove(pPoolingObject);
		pPoolingObject->Setup_ComponentAble();
		return pPoolingObject;
	}

	return nullptr;
}

HRESULT CLayer::Clear_Object()
{
	//CObject_Manager*	pObject_Manager = MGR(CObject_Manager);
	//CEvent_Manager*		pEvent_Manager = MGR(CEvent_Manager);

	////for (auto& pObject : m_RemoveObjects)
	////	m_GameObjects.remove(pObject);
	////m_RemoveObjects.clear();

	//for (auto& pObject : m_GameObjects)
	//{
	//	pObject_Manager->Remove_TickGroup(pObject->Get_CurTickGroup(), pObject);
	//	pObject_Manager->Remove_LateTickGroup(pObject->Get_CurLateTickGroup(), pObject);

	//	pEvent_Manager->Add_DeadBase(pObject);
	//}

	m_GameObjects.clear();

	return S_OK;
}

HRESULT CLayer::Remove_TickGroup()
{
	CObject_Manager* pObject_Manager = MGR(CObject_Manager);

	for (auto& pObject : m_GameObjects)
		pObject_Manager->Remove_TickGroup(pObject->Get_CurTickGroup(), pObject);

	return S_OK;
}

HRESULT CLayer::Remove_LateTickGroup()
{
	CObject_Manager* pObject_Manager = MGR(CObject_Manager);

	for (auto& pObject : m_GameObjects)
		pObject_Manager->Remove_LateTickGroup(pObject->Get_CurLateTickGroup(), pObject);

	return S_OK;
}

HRESULT CLayer::Remove_Group()
{
	CObject_Manager* pObject_Manager = MGR(CObject_Manager);

	for (auto& pObject : m_GameObjects)
	{
		pObject_Manager->Remove_TickGroup(pObject->Get_CurLateTickGroup(), pObject);
		pObject_Manager->Remove_LateTickGroup(pObject->Get_CurLateTickGroup(), pObject);
	}

	return S_OK;
}

HRESULT CLayer::Delete_Object()
{
	//CObject_Manager* pObject_Manager = MGR(CObject_Manager);
	CEvent_Manager* pEvent_Manager = MGR(CEvent_Manager);

	//for (auto& pObject : m_RemoveObjects)
	//	m_GameObjects.remove(pObject);
	//m_RemoveObjects.clear();

	for (auto& pObject : m_GameObjects)
	{
		//pObject_Manager->Remove_TickGroup(pObject->Get_CurTickGroup(), pObject);
		//pObject_Manager->Remove_LateTickGroup(pObject->Get_CurLateTickGroup(), pObject);

		pEvent_Manager->Add_DeadBase(pObject);
	}

	m_GameObjects.clear();

	return S_OK;
}

HRESULT CLayer::Delete_Object(CGameObject* pObject)
{
	MGR(CEvent_Manager)->Add_DeadBase(pObject);
	m_GameObjects.remove(pObject);

	return S_OK;
}

HRESULT CLayer::Change_Level()
{
	CObject_Manager* pObject_Manager = MGR(CObject_Manager);
	CEvent_Manager* pEvent_Manager = MGR(CEvent_Manager);

	for (auto& pObject : m_GameObjects)
	{
		pObject_Manager->Remove_TickGroup(pObject->Get_CurTickGroup(), pObject);
		pObject_Manager->Remove_LateTickGroup(pObject->Get_CurLateTickGroup(), pObject);

		pEvent_Manager->Add_DeadBase(pObject);
	}

	m_GameObjects.clear();

	return S_OK;
}

HRESULT CLayer::Initialize()
{

	return S_OK;
}

void CLayer::Tick(_float fTimeDelta)
{
	_uint	iTemp = 0;
	for (auto& pObject : m_GameObjects)
	{
		++iTemp;

		if (pObject->Is_Able() && pObject->Is_Alive())
			pObject->Tick(fTimeDelta * pObject->Get_TimeRate());
	}

	//for (auto& pObject : m_RemoveObjects)
	//	m_GameObjects.remove(pObject);

	//m_RemoveObjects.clear();

}

void CLayer::LateTick(_float fTimeDelta)
{
	for (auto& pObject : m_GameObjects)
	{
		if (pObject->Is_Able() && pObject->Is_Alive())
		{
			pObject->LateTick(fTimeDelta * pObject->Get_TimeRate());
			pObject->Compute_CamDistance();
		}
	}

	//for (auto& pObject : m_RemoveObjects)
	//	m_GameObjects.remove(pObject);

	//m_RemoveObjects.clear();
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

//void CLayer::Tick(_float fTimeDelta)
//{
//	GAMEOBJECTS	DeadObjectList;
//
//	for (auto& pGameObject : m_GameObjects)
//	{
//		if (true == pGameObject->Is_Dead())
//			DeadObjectList.push_back(pGameObject);
//
//		if (nullptr != pGameObject)
//			pGameObject->Tick(fTimeDelta);
//	}
//
//	for (auto& pGameObject : DeadObjectList)
//		m_GameObjects.remove(pGameObject);
//}
//
//void CLayer::LateTick(_float fTimeDelta)
//{
//	GAMEOBJECTS	DeadObjectList;
//	for (auto& pGameObject : m_GameObjects)
//	{
//		if (true == pGameObject->Is_Dead())
//			DeadObjectList.push_back(pGameObject);
//
//		if (nullptr != pGameObject)
//			pGameObject->LateTick(fTimeDelta);
//	}
//	for (auto& pGameObject : DeadObjectList)
//		m_GameObjects.remove(pGameObject);
//}

CLayer* CLayer::Create(_uint iCurLevel, ID_UINT tLayerID)
{
	CLayer* pInstance = new CLayer();
	pInstance->m_iCurLevel = iCurLevel;
	pInstance->m_tLayerID = tLayerID;

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CLayer* CLayer::Create(_uint iCurLevel)
{
	CLayer* pInstance = new CLayer();
	pInstance->m_iCurLevel = iCurLevel;

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLayer::Free()
{
	//USEGAMEINSTANCE;

	CEvent_Manager* pEventMgr = CEvent_Manager::Get_Instance();


	for (auto& pGameObject : m_GameObjects)
		pEventMgr->Add_DeadBase(pGameObject);
	//Safe_Release(pGameObject);

	m_GameObjects.clear();
}

