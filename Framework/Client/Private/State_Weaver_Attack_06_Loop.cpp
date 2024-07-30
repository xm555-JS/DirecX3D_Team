#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_06_Loop.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CState_Weaver_Attack_06_Loop::CState_Weaver_Attack_06_Loop(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_06_Loop::CState_Weaver_Attack_06_Loop(const CState_Weaver_Attack_06_Loop& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_06_Loop::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_ATTACK_06_BREAK);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_06_END);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_ATTACK_06_END] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_06_Loop::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_06_Loop::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
	m_iAttCnt = 0;
}

void CState_Weaver_Attack_06_Loop::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;
	m_fTimeAcc_Effect0 += fTimeDelta;
	m_fTimeAcc_Effect1 += fTimeDelta;

	if (m_fAccTime > 5.f)
		m_bEndCurState = true;

	if (1.f < m_fTimeAcc_Effect0)
	{
		m_fTimeAcc_Effect0 = 0.f;
		EFFECT_POS("Boss_Weaver_Attack_06_Spear_Loop_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	}
	if (0.25f < m_fTimeAcc_Effect1)
	{
		m_fTimeAcc_Effect1 = 0.f;
		_float fRange = 10;
		_float4 vPos;
		XMStoreFloat4(&vPos, m_pOwner->Get_Pos());
		if (m_bToggle_Effcet)
		{
			if (5 < m_iAttCnt)
			{
				m_iAttCnt = 0;
				XMStoreFloat4(&vPos, m_pTarget->Get_Pos());
				vPos.w = 1.f; // Save Phase State
				GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_FROZEN), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_FROZEN), &vPos);
			}
			else
			{
				vPos.x += fRandom(-10.f, 10.f);
				vPos.z += fRandom(-10.f, 10.f);
				vPos.w = 1.f; // Save Phase State
				GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_FROZEN), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_FROZEN), &vPos);
			}
			m_bToggle_Effcet = !m_bToggle_Effcet;
		}
		else
		{
			if (5 < m_iAttCnt)
			{
				m_iAttCnt = 0;
				XMStoreFloat4(&vPos, m_pTarget->Get_Pos());
				vPos.w = 0.f; // Save Phase State
				GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_FROZEN), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_FROZEN), &vPos);
			}
			else
			{
				vPos.x += fRandom(-10.f, 10.f);
				vPos.z += fRandom(-10.f, 10.f);
				vPos.w = 0.f; // Save Phase State
				GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_FROZEN), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_FROZEN), &vPos);
			}

			m_bToggle_Effcet = !m_bToggle_Effcet;
		}
		++m_iAttCnt;
	}


	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_06_Loop::Exit(void* pArg)
{
	m_fTimeAcc_Effect0 = 0.f;
	m_fTimeAcc_Effect1 = 0.f;
}

_bool CState_Weaver_Attack_06_Loop::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Weaver_Attack_06_Loop* CState_Weaver_Attack_06_Loop::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_06_Loop* pInstance = new CState_Weaver_Attack_06_Loop(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_06_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_06_Loop::Clone(void* pArg)
{
	CState_Weaver_Attack_06_Loop* pInstance = new CState_Weaver_Attack_06_Loop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_06_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_06_Loop::Free()
{
	__super::Free();
}
