#include "stdafx.h"
#include "..\Public\State_Angel_Laser_SkillA_L.h"

CState_Angel_Laser_SkillA_L::CState_Angel_Laser_SkillA_L(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Laser_SkillA_L::CState_Angel_Laser_SkillA_L(const CState_Angel_Laser_SkillA_L& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Laser_SkillA_L::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Angel_Laser_SkillA_L::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Laser_SkillA_L::Enter(void* pArg)
{
}

void CState_Angel_Laser_SkillA_L::Tick(_float fTimeDelta)
{
}

void CState_Angel_Laser_SkillA_L::Exit(void* pArg)
{
}

_bool CState_Angel_Laser_SkillA_L::Check_Condition(CState_Handler* pHandler)
{
	return false;
}

CState_Angel_Laser_SkillA_L* CState_Angel_Laser_SkillA_L::Create(CState_Manager * pManager, STATE_TYPE eState)
{
	CState_Angel_Laser_SkillA_L* pInstance = new CState_Angel_Laser_SkillA_L(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Laser_SkillA_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Laser_SkillA_L::Clone(void* pArg)
{
	CState_Angel_Laser_SkillA_L* pInstance = new CState_Angel_Laser_SkillA_L(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Laser_SkillA_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Laser_SkillA_L::Free()
{
	__super::Free();
}
