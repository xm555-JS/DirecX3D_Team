#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Dead_End.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_HunterDog_Dead_End::CState_Enemy_HunterDog_Dead_End(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Dead_End::CState_Enemy_HunterDog_Dead_End(const CState_Enemy_HunterDog_Dead_End& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Dead_End::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_HUNTERDOG_DEAD_END);

	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Dead_End::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Dead_End::Enter(void* pArg)
{

	m_fAnimSpeed = 0.5f;
	m_pTargetModel->Get_Animation()[21]->Set_KeyFrame(60);

}

void CState_Enemy_HunterDog_Dead_End::Tick(_float fTimeDelta)
{

	if (59<Get_CurrentKeyFrame(m_pState_Handler,m_pTargetModel))
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HunterDog_Dead_End::Exit(void* pArg)
{
}

_bool CState_Enemy_HunterDog_Dead_End::Check_Condition(CState_Handler* pHandler)
{
	if (Is_OwnerDead(pHandler))
		return true;


	return false;
}


CState_Enemy_HunterDog_Dead_End* CState_Enemy_HunterDog_Dead_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Dead_End* pInstance = new CState_Enemy_HunterDog_Dead_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Dead_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Dead_End::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Dead_End* pInstance = new CState_Enemy_HunterDog_Dead_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Dead_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Dead_End::Free()
{
	__super::Free();
}
