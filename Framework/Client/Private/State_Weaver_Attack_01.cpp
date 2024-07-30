#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_01.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CState_Weaver_Attack_01::CState_Weaver_Attack_01(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_01::CState_Weaver_Attack_01(const CState_Weaver_Attack_01& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_01::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_01::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_01::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
	m_fAnimSpeed = 2.f;
	m_fDamage = fRandom(540.f, 660.f);
	for (size_t i = 0; i < 6; ++i)
		m_pColCom[i] = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_AABB, COL_ENEMY_ATTACK, i)));

}

void CState_Weaver_Attack_01::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (55 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_01.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
		EFFECT_POS("Boss_Weaver_Attack_01", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), 
			XMVectorSet(1.f, 2.f, -0.6f, 1.f),
			XMVectorSet(XMConvertToRadians(-40.f), XMConvertToRadians(160.f), XMConvertToRadians(0), 0.f),
			1.f,
			XMVectorSet(1.3f, 1.f, 2.1f, 0.f)
			));
	}
	if (100 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_01.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect1,
		EFFECT_POS("Boss_Weaver_Attack_01", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), 
			XMVectorSet(-0.7f, 3.5f, -0.1f, 1.f),
			XMVectorSet(XMConvertToRadians(210.f), 0.f, XMConvertToRadians(25.f), 0.f),
			1.f,
			XMVectorSet(1.3f, 1.f, 2.1f, 0.f)
			));
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	for (size_t i = 0; i < 6; ++i)
		Hit_Unit(m_pColCom[i], ELEMENTAL_ICE, m_fDamage, 65, 130, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[i]);

	//Hit_Unit(m_pColCom, ELEMENTAL_ICE, m_fDamage, 65, 85, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0);
	//Hit_Unit(m_pColCom, ELEMENTAL_ICE, m_fDamage, 110, 130, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit1);

	m_pOwnerTranformCom->YFixedLookAt(m_pTarget->Get_Pos());

	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_01::Exit(void* pArg)
{
	m_bHit0 = false;
	m_bHit1 = false;
	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
}

_bool CState_Weaver_Attack_01::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Weaver_Attack_01* CState_Weaver_Attack_01::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_01* pInstance = new CState_Weaver_Attack_01(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_01::Clone(void* pArg)
{
	CState_Weaver_Attack_01* pInstance = new CState_Weaver_Attack_01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_01::Free()
{
	__super::Free();
}
