#include "stdafx.h"
#include "..\Public\State_Angel_Ult_Skill_Start.h"

CState_Angel_Ult_Skill_Start::CState_Angel_Ult_Skill_Start(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Ult_Skill_Start::CState_Angel_Ult_Skill_Start(const CState_Angel_Ult_Skill_Start& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Ult_Skill_Start::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Angel_Ult_Skill_Start::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Ult_Skill_Start::Enter(void* pArg)
{
}

void CState_Angel_Ult_Skill_Start::Tick(_float fTimeDelta)
{
}

void CState_Angel_Ult_Skill_Start::Exit(void* pArg)
{
}

_bool CState_Angel_Ult_Skill_Start::Check_Condition(CState_Handler* pHandler)
{
	return false;
}

CState_Angel_Ult_Skill_Start* CState_Angel_Ult_Skill_Start::Create(CState_Manager * pManager, STATE_TYPE eState)
{
	CState_Angel_Ult_Skill_Start* pInstance = new CState_Angel_Ult_Skill_Start(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Ult_Skill_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Ult_Skill_Start::Clone(void* pArg)
{
	CState_Angel_Ult_Skill_Start* pInstance = new CState_Angel_Ult_Skill_Start(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Ult_Skill_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Ult_Skill_Start::Free()
{
	__super::Free();
}
