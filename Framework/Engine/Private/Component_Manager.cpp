#include "..\Public\Component_Manager.h"
#include "Component.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{

}

HRESULT CComponent_Manager::Get_Component_Prototypes(ID_UINT iPrototypeID, _uint iLevelIndex, map<_uint, CComponent*>* pPrototypes, _uint iUseIDNum)
{
	for (auto& Prototype : m_pPrototypes[iLevelIndex])
	{
		ID_UINT m_PrototypeID;
		m_PrototypeID.iID = Prototype.first;
		//if (iPrototypeID.tNums.Num0 == m_PrototypeID.tNums.Num0)
		//	pPrototypes->emplace(Prototype.first, Prototype.second);


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

HRESULT CComponent_Manager::Reserve_Container(_uint iNumLevels)
{
	if (nullptr != m_pPrototypes)
		__debugbreak();

	m_iNumLevels = iNumLevels;

	m_pPrototypes = new PROTOTYPES[iNumLevels];

	return S_OK;
}

HRESULT CComponent_Manager::Add_Component_Prototype(_uint iLevelIndex, ID_UINT iPrototypeID, CComponent* pPrototype)
{
	if (iLevelIndex >= m_iNumLevels ||
		nullptr == m_pPrototypes)
		__debugbreak();

	if (nullptr != Find_Components(iLevelIndex, iPrototypeID))
		__debugbreak();

	m_pPrototypes[iLevelIndex].emplace(iPrototypeID.iID, pPrototype);

	return S_OK;
}

HRESULT CComponent_Manager::Add_Component_Prototype(_uint iLevelIndex, ID_UINT iPrototypeID, CComponent* pPrototype, string strModelFileName)
{
	if (iLevelIndex >= m_iNumLevels ||
		nullptr == m_pPrototypes)
		__debugbreak();

	if (nullptr != Find_Components(iLevelIndex, iPrototypeID))
		__debugbreak();

	pPrototype->Set_ModelFileName(strModelFileName);
	pPrototype->Set_PrototypeID(iPrototypeID);
	m_pPrototypes[iLevelIndex].emplace(iPrototypeID.iID, pPrototype);

	return S_OK;
}

CComponent* CComponent_Manager::Clone_Component(_uint iLevelIndex, ID_UINT iPrototypeID, void* pArg)
{
	if (iLevelIndex >= m_iNumLevels ||
		nullptr == m_pPrototypes)
		return nullptr;

	CComponent* pPrototype = Find_Components(iLevelIndex, iPrototypeID);
	if (nullptr == pPrototype)
		return nullptr;

	return pPrototype->Clone(pArg);
}

HRESULT CComponent_Manager::Get_Component_Prototypes(ID_UINT iPrototypeID, map<_uint, CComponent*>* pPrototypes, _uint iUseIDNum)
{
	for (auto& Prototype : m_pPrototypes[GAMEINSTANCE->Get_CurLevelIndex()])
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

HRESULT CComponent_Manager::Get_Component_Prototypes(_uint iLevel, ID_UINT iPrototypeID, map<_uint, CComponent*>* pPrototypes, _uint iUseIDNum)
{
	for (auto& Prototype : m_pPrototypes[iLevel])
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

HRESULT CComponent_Manager::Get_Component_Prototypes(_uint iPrototypeID, map<_uint, CComponent*>* pPrototypes, _uint iUseIDNum)
{
	ID_UINT _ID;
	_ID.iID = iPrototypeID;
	for (auto& Prototype : m_pPrototypes[GAMEINSTANCE->Get_CurLevelIndex()])
	{
		ID_UINT m_PrototypeID;
		m_PrototypeID.iID = Prototype.first;

		_bool	bCheck = true;
		switch (iUseIDNum)
		{
		case 4:
			if (_ID.tNums.Num3 != m_PrototypeID.tNums.Num3)
				bCheck = false;
		case 3:
			if (_ID.tNums.Num2 != m_PrototypeID.tNums.Num2)
				bCheck = false;
		case 2:
			if (_ID.tNums.Num1 != m_PrototypeID.tNums.Num1)
				bCheck = false;
		case 1:
			if (_ID.tNums.Num0 != m_PrototypeID.tNums.Num0)
				bCheck = false;
		default:
			break;
		}

		if (true == bCheck)
			pPrototypes->emplace(Prototype.first, Prototype.second);
	}

	return S_OK;
}

HRESULT CComponent_Manager::Get_Component_Prototypes(_uint iLevel, _uint iPrototypeID, map<_uint, CComponent*>* pPrototypes, _uint iUseIDNum)
{
	ID_UINT _ID;
	_ID.iID = iPrototypeID;
	for (auto& Prototype : m_pPrototypes[iLevel])
	{
		ID_UINT m_PrototypeID;
		m_PrototypeID.iID = Prototype.first;

		_bool	bCheck = true;
		switch (iUseIDNum)
		{
		case 4:
			if (_ID.tNums.Num3 != m_PrototypeID.tNums.Num3)
				bCheck = false;
		case 3:
			if (_ID.tNums.Num2 != m_PrototypeID.tNums.Num2)
				bCheck = false;
		case 2:
			if (_ID.tNums.Num1 != m_PrototypeID.tNums.Num1)
				bCheck = false;
		case 1:
			if (_ID.tNums.Num0 != m_PrototypeID.tNums.Num0)
				bCheck = false;
		default:
			break;
		}

		if (true == bCheck)
			pPrototypes->emplace(Prototype.first, Prototype.second);
	}

	return S_OK;
}

void CComponent_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pPrototypes[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}

	m_pPrototypes[iLevelIndex].clear();
}

CComponent* CComponent_Manager::Find_Components(_uint iLevelIndex, ID_UINT iPrototypeID)
{
	auto	iter = find_if(m_pPrototypes[iLevelIndex].begin(), m_pPrototypes[iLevelIndex].end(), CID_Finder(iPrototypeID.iID));

	if (iter == m_pPrototypes[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pPrototypes[i])
			Safe_Release(Pair.second);

		m_pPrototypes[i].clear();
	}

	Safe_Delete_Array(m_pPrototypes);
}
