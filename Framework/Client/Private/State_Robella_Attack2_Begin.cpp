#include "stdafx.h"
#include "..\Public\State_Robella_Attack2_Begin.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_Attack2_Begin::CState_Robella_Attack2_Begin(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Attack2_Begin::CState_Robella_Attack2_Begin(const CState_Robella_Attack2_Begin& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Attack2_Begin::Initialize_Prototype()
{
	// 다음으로 넘어갈 수 있는 state      
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_02_LOOP);
	//m_ReserveStates.push_back(STATE_BOSS_ROBELLA_2PHASE);

	// 다음 state로 넘어갈 확률
	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_ATTACK_02_LOOP] = 1.f;
	//m_ReserveProbabilitys[STATE_BOSS_ROBELLA_2PHASE] = 1.f;

	return S_OK;
}

HRESULT CState_Robella_Attack2_Begin::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Attack2_Begin::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAccTime = 0.f;
}

void CState_Robella_Attack2_Begin::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	if (68 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Open_Mouse.wav"), CSound_Manager::CHANNEL_ENEMY, m_pOwner->Get_Pos(), 0.1f));

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Attack2_Begin::Exit(void* pArg)
{
}

_bool CState_Robella_Attack2_Begin::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_Attack2_Begin* CState_Robella_Attack2_Begin::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Attack2_Begin* pInstance = new CState_Robella_Attack2_Begin(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Attack2_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Attack2_Begin::Clone(void* pArg)
{
	CState_Robella_Attack2_Begin* pInstance = new CState_Robella_Attack2_Begin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Attack2_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Attack2_Begin::Free()
{
	__super::Free();
}