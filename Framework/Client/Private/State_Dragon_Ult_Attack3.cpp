#include "stdafx.h"
#include "..\Public\State_Dragon_Ult_Attack3.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

CState_Dragon_Ult_Attack3::CState_Dragon_Ult_Attack3(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Ult_Attack3::CState_Dragon_Ult_Attack3(const CState_Dragon_Ult_Attack3& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Ult_Attack3::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Ult_Attack3::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Ult_Attack3::Enter(void* pArg)
{
}

void CState_Dragon_Ult_Attack3::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Ult_Attack3::Exit(void* pArg)
{
}

_bool CState_Dragon_Ult_Attack3::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Dragon_Ult_Attack3* CState_Dragon_Ult_Attack3::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Ult_Attack3* pInstance = new CState_Dragon_Ult_Attack3(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Ult_Attack3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Ult_Attack3::Clone(void* pArg)
{
	CState_Dragon_Ult_Attack3* pInstance = new CState_Dragon_Ult_Attack3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Ult_Attack3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Ult_Attack3::Free()
{
	__super::Free();
}
