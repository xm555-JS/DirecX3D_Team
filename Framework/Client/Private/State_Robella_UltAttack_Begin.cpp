#include "stdafx.h"
#include "..\Public\State_Robella_UltAttack_Begin.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Enemy_Boss_Robella.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_UltAttack_Begin::CState_Robella_UltAttack_Begin(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_UltAttack_Begin::CState_Robella_UltAttack_Begin(const CState_Robella_UltAttack_Begin& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_UltAttack_Begin::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_ULT);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_ATTACK_ULT] = 1;

	return S_OK;
}

HRESULT CState_Robella_UltAttack_Begin::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_UltAttack_Begin::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Robella_UltAttack_Begin::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	// 해당 클래스의 애니메이션이 끝나면 Idle_state로 넘어감
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_UltAttack_Begin::Exit(void* pArg)
{
	((CEnemy_Boss_Robella*)m_pOwner)->Set_CoolTime();
}

_bool CState_Robella_UltAttack_Begin::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Robella_UltAttack_Begin* CState_Robella_UltAttack_Begin::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_UltAttack_Begin* pInstance = new CState_Robella_UltAttack_Begin(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_UltAttack_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_UltAttack_Begin::Clone(void* pArg)
{
	CState_Robella_UltAttack_Begin* pInstance = new CState_Robella_UltAttack_Begin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_UltAttack_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_UltAttack_Begin::Free()
{
	__super::Free();
}
