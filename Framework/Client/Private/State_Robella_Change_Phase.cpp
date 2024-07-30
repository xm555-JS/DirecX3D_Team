#include "stdafx.h"
#include "..\Public\State_Robella_Change_Phase.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Enemy_Boss_Robella.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_Change_Phase::CState_Robella_Change_Phase(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Change_Phase::CState_Robella_Change_Phase(const CState_Robella_Change_Phase& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Change_Phase::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_IDLE2);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_IDLE2] = 1;

	return S_OK;
}

HRESULT CState_Robella_Change_Phase::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Change_Phase::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Robella_Change_Phase::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	// 해당 클래스의 애니메이션이 끝나면 Idle_state로 넘어감
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Change_Phase::Exit(void* pArg)
{
}

_bool CState_Robella_Change_Phase::Check_Condition(CState_Handler* pHandler)
{
	CEnemy_Boss_Robella* pOwner = ((CEnemy_Boss_Robella*)pHandler->Get_Owner());

	if ((pOwner->Get_UnitInfo()->fHP) * 0.7f > pOwner->Get_UnitCurInfo()->fHP )
		return true;

	return false;
}

CState_Robella_Change_Phase* CState_Robella_Change_Phase::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Change_Phase* pInstance = new CState_Robella_Change_Phase(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Change_Phase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Change_Phase::Clone(void* pArg)
{
	CState_Robella_Change_Phase* pInstance = new CState_Robella_Change_Phase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Change_Phase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Change_Phase::Free()
{
	__super::Free();
}
