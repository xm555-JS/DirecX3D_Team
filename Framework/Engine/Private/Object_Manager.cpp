#include "..\Public\Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Event_Manager.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

CComponent* CObject_Manager::Get_Component(_uint iLevelIndex, ID_UINT tLayerID, ID_UINT tComponentID, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, tLayerID);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(tComponentID, iIndex);
}

CGameObject* CObject_Manager::Get_Object(_uint iLevelIndex, ID_UINT iLayerID, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, iLayerID);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Object(iIndex);
}

list<class CGameObject*>* CObject_Manager::Get_Objects(_uint iLevelIndex, ID_UINT iLayerID)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, iLayerID);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Objects();;
}

HRESULT CObject_Manager::Get_Prototypes(ID_UINT iPrototypeID, map<_uint, CGameObject*>* pPrototypes, _uint iUseIDNum)
{
	for (auto& Prototype : m_Prototypes)
	{
		ID_UINT m_PrototypeID;
		m_PrototypeID.iID = Prototype.first;

		_bool	bCheck = true;
		switch (iUseIDNum)
		{
		case 4:
			if (iPrototypeID.tNums.Num3 != m_PrototypeID.tNums.Num3)
				bCheck = false;
		case 3:
			if (iPrototypeID.tNums.Num2 != m_PrototypeID.tNums.Num2)
				bCheck = false;
		case 2:
			if (iPrototypeID.tNums.Num1 != m_PrototypeID.tNums.Num1)
				bCheck = false;
		case 1:
			if (iPrototypeID.tNums.Num0 != m_PrototypeID.tNums.Num0)
				bCheck = false;
		default:
			break;
		}

		if (true == bCheck)
			pPrototypes->emplace(Prototype.first, Prototype.second);

	}

	return S_OK;
}

HRESULT CObject_Manager::Reserve_Container(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		__debugbreak();

	m_iNumLevels = iNumLevels;

	m_pLayers = new LAYERS[iNumLevels];
	m_pDisableLayers = new LAYERS[iNumLevels];

	return S_OK;
}

HRESULT CObject_Manager::Reserve_TickContainer(_uint iNumTickGroups)
{
	for (size_t i = 0; i < iNumTickGroups; ++i)
	{
		m_pTickObject.push_back(CLayer::Create(i));
	}

	m_pTickObjectState = new _bool[iNumTickGroups];
	m_iNumTickGroups = iNumTickGroups;

	for (size_t i = 0; i < iNumTickGroups; ++i)
		m_pTickObjectState[i] = true;


	return S_OK;
}

HRESULT CObject_Manager::Reserve_LateTickContainer(_uint iNumLateTickGroups)
{
	for (size_t i = 0; i < iNumLateTickGroups; ++i)
	{
		m_pLateTickObject.push_back(CLayer::Create(i));
	}

	m_pLateTickObjectState = new _bool[iNumLateTickGroups];
	m_iNumLateTickGroups = iNumLateTickGroups;


	for (size_t i = 0; i < iNumLateTickGroups; ++i)
		m_pLateTickObjectState[i] = true;

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_Prototype(ID_UINT tPrototypeID, CGameObject* pPrototype)
{
	if (nullptr != Find_Prototype(tPrototypeID))
	{
		Safe_Release(pPrototype);

		return S_OK;
	}

	m_Prototypes.emplace(tPrototypeID.iID, pPrototype);

	return S_OK;
}

CGameObject* CObject_Manager::Add_GameObject(_uint iLevelIndex, ID_UINT tLayerID, ID_UINT tPrototypeID, void* pArg)
{
	if (tLayerID.iID == 3858)
		_uint iTemp = 0;

	CLayer* pDisableLayer = Find_DisableLayer(iLevelIndex, tLayerID);
	CGameObject* pPoolingObject = nullptr;
	if (pDisableLayer)
	{
		pPoolingObject = pDisableLayer->Pooling_Object(tPrototypeID, pArg);

		if (pPoolingObject)
		{
			CLayer* pLayer = Find_Layer(iLevelIndex, tLayerID);
			pLayer->Add_GameObject(pPoolingObject);
			return pPoolingObject;
		}
		else
			return Cloning_Object(iLevelIndex, tLayerID, tPrototypeID, pArg);

	}
	else
	{
		return Cloning_Object(iLevelIndex, tLayerID, tPrototypeID, pArg);
	}
}

CGameObject* CObject_Manager::Add_GameObject(_uint iLevelIndex, _uint iLayerID, ID_UINT tPrototypeID, void* pArg)
{
	ID_UINT tLayerID;
	tLayerID.iID = iLayerID;
	CLayer* pLayer = Find_DisableLayer(iLevelIndex, tLayerID);
	CGameObject* pPoolingObject = nullptr;
	if (pLayer)
	{
		pPoolingObject = pLayer->Pooling_Object(tPrototypeID, pArg);


		if (pPoolingObject)
		{

			CLayer* pLayer = Find_Layer(iLevelIndex, tLayerID);
			pLayer->Add_GameObject(pPoolingObject);
			return pPoolingObject;
		}
		else
			return Cloning_Object(iLevelIndex, tLayerID, tPrototypeID, pArg);
	}
	else
	{
		return Cloning_Object(iLevelIndex, tLayerID, tPrototypeID, pArg);
	}
}

CGameObject* CObject_Manager::Add_GameObject(_uint iLevelIndex, _uint iLayerID, _uint iPrototypeID, void* pArg)
{
	ID_UINT tPrototypeID;
	ID_UINT tLayerID;
	tPrototypeID.iID = iPrototypeID;
	tLayerID.iID = iLayerID;
	CLayer* pLayer = Find_DisableLayer(iLevelIndex, tLayerID);
	CGameObject* pPoolingObject = nullptr;
	if (pLayer)
	{
		pPoolingObject = pLayer->Pooling_Object(tPrototypeID, pArg);

		if (pPoolingObject)
		{
			CLayer* pLayer = Find_Layer(iLevelIndex, tLayerID);
			pLayer->Add_GameObject(pPoolingObject);
			return pPoolingObject;
		}
		else
			return Cloning_Object(iLevelIndex, tLayerID, tPrototypeID, pArg);
	}
	else
	{
		return Cloning_Object(iLevelIndex, tLayerID, tPrototypeID, pArg);
	}
}

void CObject_Manager::Add_TickGroup(_uint iGroup, CGameObject* pGameObject)
{
	m_pTickObject[iGroup]->Add_GameObject(pGameObject);
	pGameObject->Set_CurTickGroup(iGroup);
}

void CObject_Manager::Add_LateTickGroup(_uint iGroup, CGameObject* pGameObject)
{
	m_pLateTickObject[iGroup]->Add_GameObject(pGameObject);
	pGameObject->Set_CurLateTickGroup(iGroup);
}

void CObject_Manager::Remove_TickGroup(_uint iGroup, CGameObject* pGameObject)
{
	m_pTickObject[iGroup]->Remove_Object(pGameObject);
}

void CObject_Manager::Remove_LateTickGroup(_uint iGroup, CGameObject* pGameObject)
{
	m_pLateTickObject[iGroup]->Remove_Object(pGameObject);
}

void CObject_Manager::Add_DeadObject(CGameObject* pObject)
{
	m_DeadObject.push_back(pObject);
}

void CObject_Manager::Add_PoolingObject(CGameObject* pObject)
{
	pObject->On_Dead();
	pObject->Set_Disable();
	pObject->Set_Dead();

	m_PoolingObject.push_back(pObject);
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumTickGroups; ++i)
	{
		if (false == m_pTickObjectState[i])
			continue;

		m_pTickObject[i]->Tick(fTimeDelta);
	}
}

void CObject_Manager::LateTick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLateTickGroups; ++i)
	{
		if (false == m_pLateTickObjectState[i])
			continue;

		m_pLateTickObject[i]->LateTick(fTimeDelta);
	}

	for (auto& pObject : m_PoolingObject)
		Pooling_Object(pObject);

	for (auto& pObject : m_DeadObject)
		Dead_Object(pObject);
	
	m_PoolingObject.clear();
	m_DeadObject.clear();
}

void CObject_Manager::On_EventMessege(EVENTDESC tEvent)
{
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	USEGAMEINSTANCE;
	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Pair.second->Change_Level();
		Safe_Release(Pair.second);
	}
	m_pLayers[iLevelIndex].clear();

	for (auto& Pair : m_pDisableLayers[iLevelIndex])
	{
		Pair.second->Change_Level();
		Safe_Release(Pair.second);
	}
	m_pDisableLayers[iLevelIndex].clear();

	m_PoolingObject.clear();
	m_DeadObject.clear();

}

CGameObject* CObject_Manager::Find_Prototype(ID_UINT tPrototypeID)
{
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CID_Finder(tPrototypeID.iID));

	if (iter == m_Prototypes.end())
	{
		return nullptr;
	}

	return iter->second;
}

CLayer* CObject_Manager::Find_Layer(_uint iLevelIndex, ID_UINT tLayerID)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CID_Finder(tLayerID.iID));

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CLayer* CObject_Manager::Find_DisableLayer(_uint iLevelIndex, ID_UINT tLayerID)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = find_if(m_pDisableLayers[iLevelIndex].begin(), m_pDisableLayers[iLevelIndex].end(), CID_Finder(tLayerID.iID));

	if (iter == m_pDisableLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CGameObject* CObject_Manager::Cloning_Object(_uint iLevelIndex, ID_UINT tLayerID, ID_UINT tPrototypeID, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(tPrototypeID);
	if (nullptr == pPrototype)
		return nullptr;
	pPrototype->Set_CurLevel(iLevelIndex);

	CGameObject* pCloneObject = pPrototype->Clone(pArg);
	if (nullptr == pCloneObject)
		return nullptr;

	pCloneObject->Set_Clone();
	pCloneObject->Set_PrototypeID(tPrototypeID);
	pCloneObject->Set_LayerID(tLayerID.iID);

	CLayer* pLayer = Find_Layer(iLevelIndex, tLayerID);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create(iLevelIndex, tLayerID);
		pLayer->Add_GameObject(pCloneObject);
		m_pLayers[iLevelIndex].emplace(tLayerID.iID, pLayer);
	}
	else
		pLayer->Add_GameObject(pCloneObject);

	return pCloneObject;
}

void CObject_Manager::Pooling_Object(CGameObject* pObject)
{
	m_pTickObject[pObject->Get_CurTickGroup()]->Remove_Object(pObject);
	m_pLateTickObject[pObject->Get_CurLateTickGroup()]->Remove_Object(pObject);


	ID_UINT	tLayerID;
	_uint	iCurLevel;
	tLayerID.iID = pObject->Get_LayerID();
	iCurLevel = pObject->Get_CurLevel();

	CLayer* pLayer = Find_Layer(iCurLevel, tLayerID);
	if (pLayer)
		pLayer->Remove_Object(pObject);

	CLayer* pDisableLayer = Find_DisableLayer(iCurLevel, tLayerID);
	if (nullptr == pDisableLayer)
	{
		pDisableLayer = CLayer::Create(iCurLevel, tLayerID);
		pDisableLayer->Add_GameObject(pObject);
		m_pDisableLayers[pObject->Get_CurLevel()].emplace(tLayerID.iID, pDisableLayer);
	}
	else
		pDisableLayer->Add_GameObject(pObject);

	pObject->Setup_ComponentDisable();
}

void CObject_Manager::Dead_Object(CGameObject* pObject)
{
	m_pTickObject[pObject->Get_CurTickGroup()]->Remove_Object(pObject);
	m_pLateTickObject[pObject->Get_CurLateTickGroup()]->Remove_Object(pObject);

	ID_UINT	tLayerID;
	_uint	iCurLevel;
	tLayerID.iID = pObject->Get_LayerID();
	iCurLevel = pObject->Get_CurLevel();
	CLayer* pLayer = Find_Layer(iCurLevel, tLayerID);
	if (pLayer)
		pLayer->Delete_Object(pObject);
	else
		MGR(CEvent_Manager)->Add_DeadBase(pObject);
		//__debugbreak();

	pObject->Setup_ComponentDead();
}

void CObject_Manager::Free()
{
	USEGAMEINSTANCE;
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Delete_Object();
			Pair.second->Clear_Object();

			Safe_Release(Pair.second);
		}

		m_pLayers[i].clear();

		for (auto& Pair : m_pDisableLayers[i])
		{
			Pair.second->Delete_Object();
			Pair.second->Clear_Object();

			Safe_Release(Pair.second);
		}

		m_pDisableLayers[i].clear();
	}


	for (_uint i = 0; i < m_iNumTickGroups; ++i)
	{
		m_pTickObject[i]->Clear_Object();
		Safe_Release(m_pTickObject[i]);
	}

	for (_uint i = 0; i < m_iNumLateTickGroups; ++i)
	{
		m_pLateTickObject[i]->Clear_Object();
		Safe_Release(m_pLateTickObject[i]);
	}


	Safe_Delete_Array(m_pLayers);
	Safe_Delete_Array(m_pDisableLayers);

	Safe_Delete_Array(m_pTickObjectState);
	Safe_Delete_Array(m_pLateTickObjectState);

	for (auto& Pair : m_Prototypes)
		pGameInstance->Add_DeadBase(Pair.second);

	m_Prototypes.clear();

}
