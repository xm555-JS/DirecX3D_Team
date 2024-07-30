#include "stdafx.h"
#include "..\Public\State_Angel_Laser_SkillB.h"

CState_Angel_Laser_SkillB::CState_Angel_Laser_SkillB(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Laser_SkillB::CState_Angel_Laser_SkillB(const CState_Angel_Laser_SkillB& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Laser_SkillB::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Angel_Laser_SkillB::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Laser_SkillB::Enter(void* pArg)
{
}

void CState_Angel_Laser_SkillB::Tick(_float fTimeDelta)
{
}

void CState_Angel_Laser_SkillB::Exit(void* pArg)
{
}

_bool CState_Angel_Laser_SkillB::Check_Condition(CState_Handler* pHandler)
{
	return false;
}

CState_Angel_Laser_SkillB* CState_Angel_Laser_SkillB::Create(CState_Manager * pManager, STATE_TYPE eState)
{
	CState_Angel_Laser_SkillB* pInstance = new CState_Angel_Laser_SkillB(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Laser_SkillB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Laser_SkillB::Clone(void* pArg)
{
	CState_Angel_Laser_SkillB* pInstance = new CState_Angel_Laser_SkillB(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Laser_SkillB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Laser_SkillB::Free()
{
	__super::Free();
}
