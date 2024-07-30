#include "stdafx.h"
#include "..\Public\State_Angel_Laser_SkillA.h"

CState_Angel_Laser_SkillA::CState_Angel_Laser_SkillA(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Laser_SkillA::CState_Angel_Laser_SkillA(const CState_Angel_Laser_SkillA& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Laser_SkillA::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Angel_Laser_SkillA::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Laser_SkillA::Enter(void* pArg)
{
}

void CState_Angel_Laser_SkillA::Tick(_float fTimeDelta)
{
}

void CState_Angel_Laser_SkillA::Exit(void* pArg)
{
}

_bool CState_Angel_Laser_SkillA::Check_Condition(CState_Handler* pHandler)
{
	return false;
}

CState_Angel_Laser_SkillA* CState_Angel_Laser_SkillA::Create(CState_Manager * pManager, STATE_TYPE eState)
{
	CState_Angel_Laser_SkillA* pInstance = new CState_Angel_Laser_SkillA(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Laser_SkillA");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Laser_SkillA::Clone(void* pArg)
{
	CState_Angel_Laser_SkillA* pInstance = new CState_Angel_Laser_SkillA(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Laser_SkillA");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Laser_SkillA::Free()
{
	__super::Free();
}
