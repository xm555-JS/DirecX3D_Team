#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End(const CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End& rhs)
	: CState_Enemy_Normal(rhs)
{

}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End::Initialize_Prototype()
{

	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DEAD_END);


	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_NORMAL);


	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_NORMAL] = 1;


	m_fAttackLength_Min = 1.f;
	m_fAttackLength_Max = 3.f;

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;
	m_strRootBoneName = "Root";

	m_fDamage = 3.f;

	m_pColCom[COLLIDER_OWNER] = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));

}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End::Tick(_float fTimeDelta)
{
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 63, 67, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_1]);

	if (Is_AnimFinished())
		m_bEndCurState = true;

	if (14 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect0, EFFECT_POS("DoubleSword_Attack_Swing_3", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(-0.f, -0.4f, 1.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));

		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Double_Attack.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));
	}

	CState_Enemy_Normal::Tick(fTimeDelta);

}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End::Exit(void* pArg)
{
	for (int i = 0; i < HIT_END; i++)
		m_bHit[i] = false;

	RESET_ONCE(m_bEffect0);
}

_bool CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End::Check_Condition(CState_Handler* pHandler)
{
	if (Is_Damaged(pHandler))
		return false;

	return true;
}


CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End::Free()
{
	__super::Free();
}
