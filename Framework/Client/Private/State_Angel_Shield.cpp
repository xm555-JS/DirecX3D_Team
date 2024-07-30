#include "stdafx.h"
#include "..\Public\State_Angel_Shield.h"

CState_Angel_Shield::CState_Angel_Shield(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Shield::CState_Angel_Shield(const CState_Angel_Shield& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Shield::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Angel_Shield::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Shield::Enter(void* pArg)
{
}

void CState_Angel_Shield::Tick(_float fTimeDelta)
{
}

void CState_Angel_Shield::Exit(void* pArg)
{
}

_bool CState_Angel_Shield::Check_Condition(CState_Handler* pHandler)
{
	return false;
}

CState_Angel_Shield* CState_Angel_Shield::Create(CState_Manager * pManager, STATE_TYPE eState)
{
	CState_Angel_Shield* pInstance = new CState_Angel_Shield(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Shield");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Shield::Clone(void* pArg)
{
	CState_Angel_Shield* pInstance = new CState_Angel_Shield(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Shield");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Shield::Free()
{
	__super::Free();
}
