#include "stdafx.h"
#include "..\Public\State_Angel_Laser_SkillA_R.h"

CState_Angel_Laser_SkillA_R::CState_Angel_Laser_SkillA_R(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Laser_SkillA_R::CState_Angel_Laser_SkillA_R(const CState_Angel_Laser_SkillA_R& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Laser_SkillA_R::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Angel_Laser_SkillA_R::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Laser_SkillA_R::Enter(void* pArg)
{
}

void CState_Angel_Laser_SkillA_R::Tick(_float fTimeDelta)
{
}

void CState_Angel_Laser_SkillA_R::Exit(void* pArg)
{
}

_bool CState_Angel_Laser_SkillA_R::Check_Condition(CState_Handler* pHandler)
{
	return false;
}

CState_Angel_Laser_SkillA_R* CState_Angel_Laser_SkillA_R::Create(CState_Manager * pManager, STATE_TYPE eState)
{
	CState_Angel_Laser_SkillA_R* pInstance = new CState_Angel_Laser_SkillA_R(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Laser_SkillA_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Laser_SkillA_R::Clone(void* pArg)
{
	CState_Angel_Laser_SkillA_R* pInstance = new CState_Angel_Laser_SkillA_R(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Laser_SkillA_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Laser_SkillA_R::Free()
{
	__super::Free();
}
