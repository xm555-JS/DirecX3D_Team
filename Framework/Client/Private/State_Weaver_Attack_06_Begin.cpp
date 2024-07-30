#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_06_Begin.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CState_Weaver_Attack_06_Begin::CState_Weaver_Attack_06_Begin(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_06_Begin::CState_Weaver_Attack_06_Begin(const CState_Weaver_Attack_06_Begin& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_06_Begin::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_ATTACK_06_BREAK);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_06_LOOP);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_ATTACK_06_LOOP] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_06_Begin::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_06_Begin::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
}

void CState_Weaver_Attack_06_Begin::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (30 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_09_Begin.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Boss_Weaver_Attack_09_ShockWave_Aura", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.8f, -0.1f, 1.f));
		//for (_uint i = 0; i < 20; i++)
		//{
		//	EFFECT_POS("Boss_Weaver_Attack_06_Spear_Ground", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-10.f, 10.f), 0.f, fRandom(-10.f, 10.f), 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1, XMVectorSet(fRandom(1.f, 1.2f), 1.7f, fRandom(1.f, 1.2f), 1.f));
		//	EFFECT_POS("Boss_Weaver_Attack_06_Spear_Ground", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-10.f, 10.f), 0.f, fRandom(-10.f, 10.f), 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1, XMVectorSet(fRandom(1.f, 1.2f), 1.7f, fRandom(1.f, 1.2f), 1.f));
		//	EFFECT_POS("Boss_Weaver_Attack_06_Spear_Ground", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-10.f, 10.f), 0.f, fRandom(-10.f, 10.f), 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1, XMVectorSet(fRandom(1.f, 1.2f), 1.7f, fRandom(1.f, 1.2f), 1.f));
		//}
		);
	}

	if (100.f <= Get_CurKeyFrame())
	{
		m_fTimeAcc_Effect0 += fTimeDelta;
		if (0.2f * 1.f < m_fTimeAcc_Effect0)
		{
			IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_06_Knife.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
			IMPL_ONCE(m_bEffect1, EFFECT_EXCUTE("Boss_Weaver_Attack_06_Spear_Knife", XMMatrixIdentity(), 0.75f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone019"));
		}
		if (0.2f * 2.f < m_fTimeAcc_Effect0)
		{
			IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_06_Knife.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
			IMPL_ONCE(m_bEffect2, EFFECT_EXCUTE("Boss_Weaver_Attack_06_Spear_Knife", XMMatrixIdentity(), 0.75f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone020"));
		}
		if (0.2f * 3.f < m_fTimeAcc_Effect0)
		{
			IMPL_ONCE(m_bSound[3], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_06_Knife.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
			IMPL_ONCE(m_bEffect3, EFFECT_EXCUTE("Boss_Weaver_Attack_06_Spear_Knife", XMMatrixIdentity(), 0.75f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone021"));
		}
		if (0.2f * 4.f < m_fTimeAcc_Effect0)
		{
			IMPL_ONCE(m_bSound[4], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_06_Knife.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
			IMPL_ONCE(m_bEffect4, EFFECT_EXCUTE("Boss_Weaver_Attack_06_Spear_Knife", XMMatrixIdentity(), 0.75f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone022"));
		}
		if (0.2f * 5.f < m_fTimeAcc_Effect0)
		{
			IMPL_ONCE(m_bSound[5], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_06_Knife.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
			IMPL_ONCE(m_bEffect5, EFFECT_EXCUTE("Boss_Weaver_Attack_06_Spear_Knife", XMMatrixIdentity(), 0.75f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone023"));
		}
		if (0.2f * 6.f < m_fTimeAcc_Effect0)
		{
			IMPL_ONCE(m_bSound[6], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_06_Knife.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
			IMPL_ONCE(m_bEffect6, EFFECT_EXCUTE("Boss_Weaver_Attack_06_Spear_Knife", XMMatrixIdentity(), 0.75f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone024"));
		}
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	//Frozen »ý¼º

	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_06_Begin::Exit(void* pArg)
{
	m_fTimeAcc_Effect0 = 0.f;
	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
	RESET_ONCE(m_bEffect4);
	RESET_ONCE(m_bEffect5);
	RESET_ONCE(m_bEffect6);
}

_bool CState_Weaver_Attack_06_Begin::Check_Condition(CState_Handler* pHandler)
{
	CUnit* pOwner = ((CUnit*)pHandler->Get_Owner());
#if 0
	if ((pOwner->Get_UnitInfo()->fHP) * 0.4f < pOwner->Get_UnitCurInfo()->fHP)
		return false;
#endif // 0


	return true;
}

CState_Weaver_Attack_06_Begin* CState_Weaver_Attack_06_Begin::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_06_Begin* pInstance = new CState_Weaver_Attack_06_Begin(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_06_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_06_Begin::Clone(void* pArg)
{
	CState_Weaver_Attack_06_Begin* pInstance = new CState_Weaver_Attack_06_Begin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_06_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_06_Begin::Free()
{
	__super::Free();
}
