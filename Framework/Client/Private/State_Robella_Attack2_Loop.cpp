#include "stdafx.h"
#include "..\Public\State_Robella_Attack2_Loop.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"
#include "Effect_Manager.h"

CState_Robella_Attack2_Loop::CState_Robella_Attack2_Loop(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Attack2_Loop::CState_Robella_Attack2_Loop(const CState_Robella_Attack2_Loop& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Attack2_Loop::Initialize_Prototype()
{
	// 다음으로 넘어갈 수 있는 state      
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_02_END);
	//m_ReserveStates.push_back(STATE_BOSS_ROBELLA_2PHASE);

	// 다음 state로 넘어갈 확률
	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_ATTACK_02_END] = 1.f;
	//m_ReserveProbabilitys[STATE_BOSS_ROBELLA_2PHASE] = 1.f;

	return S_OK;
}

HRESULT CState_Robella_Attack2_Loop::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Attack2_Loop::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_iPlayCnt = 0;
	m_fAccTime = 0.f;
	m_fAnimSpeed = 1.f;
}

void CState_Robella_Attack2_Loop::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if (2 < m_iPlayCnt)
		m_bEndCurState = true;

	// 불기둥
	if (10 < Get_CurKeyFrame() && !m_bCreate)
	{
		_float4 vPos;
		XMStoreFloat4(&vPos, ((CUnit*)m_pOwner)->Get_Target()->Get_Pos());

		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_FIRESTROM), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_FIRESTROM), &vPos);
		m_bCreate = true;
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_iPlayCnt++;
		m_bCreate = false;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Attack2_Loop::Exit(void* pArg)
{
	RESET_ONCE(m_bEffect0);
}

_bool CState_Robella_Attack2_Loop::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_Attack2_Loop* CState_Robella_Attack2_Loop::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Attack2_Loop* pInstance = new CState_Robella_Attack2_Loop(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Attack2_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Attack2_Loop::Clone(void* pArg)
{
	CState_Robella_Attack2_Loop* pInstance = new CState_Robella_Attack2_Loop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Attack2_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Attack2_Loop::Free()
{
	__super::Free();
}