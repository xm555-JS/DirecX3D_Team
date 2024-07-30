#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_SwordGhost_Attack_Moving.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Effect_Manager.h"
#include "Camera.h"

CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving::CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving::CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving(const CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving& rhs)
	: CState_Enemy_Normal(rhs)
{

}

HRESULT CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving::Initialize_Prototype()
{

	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DEAD_END);


	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_SWORDGHOST_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_SWORDGHOST_IDLE_NORMAL);


	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_IDLE_NORMAL] = 1;


	m_fAttackLength_Min = 1.f;
	m_fAttackLength_Max = 3.5f;

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;
	m_strRootBoneName = "Root";

	m_fDamage = fRandom(100, 120.f);

	m_pColCom[COLLIDER_OWNER] = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));

}

void CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving::Tick(_float fTimeDelta)
{
	if (Is_Damaged())
		m_bEndCurState = true;
	m_fDamage = fRandom(100, 120.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 36, 37, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_1]);
	m_fDamage = fRandom(100, 120.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 38, 39, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_2]);
	m_fDamage = fRandom(100, 120.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 43, 44, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_3]);
	m_fDamage = fRandom(100, 120.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage * 1.5f, 54, 57, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_4]);

	if (Is_AnimFinished())
		m_bEndCurState = true;

	if (20 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("TimeIllusion_Ready", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			-XMVector3Normalize(GAMEINSTANCE->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]),
			//XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (34 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Ghost_Sword_Combo_Attack1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
		
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Ghost_Attack.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT,m_pOwner->Get_Pos(), 0.1f));
	}

	if (40 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect2, EFFECT_POS("Ghost_Sword_Combo_Attack2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, -0.3f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));

		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Ghost_Attack.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));
	}

	if (35 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Ghost_Attack_End.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	if (51 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect3, EFFECT_POS("Ghost_Sword_Combo_Attack3", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}


	//if (37 < Get_CurKeyFrame())
	//{
	//	// 포스 회전 스케일
	//	IMPL_ONCE(m_bEffect0, EFFECT_POS("Ghost_Sword_Combo_Attack1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
	//		XMVectorSet(DEBUGFLOAT4(0).x, DEBUGFLOAT4(0).y, DEBUGFLOAT4(0).z, 1.f),
	//		XMVectorSet(DEBUGFLOAT4(1).x, DEBUGFLOAT4(1).y, DEBUGFLOAT4(1).z, 0.f), DEBUGFLOAT4(3).x,
	//		XMVectorSet(DEBUGFLOAT4(2).x, DEBUGFLOAT4(2).y, DEBUGFLOAT4(2).z, 0.f)));
	//}

	CState_Enemy_Normal::Tick(fTimeDelta);
}

void CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving::Exit(void* pArg)
{
	for (int i = 0; i < HIT_END; i++)
		m_bHit[i] = false;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
}

_bool CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving::Check_Condition(CState_Handler* pHandler)
{
	if (Is_Damaged(pHandler))
		return false;

	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (m_fAttackLength_Min < fLength && m_fAttackLength_Max > fLength)
		return true;

	return false;
}


CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving* CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving* pInstance = new CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving* pInstance = new CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving::Free()
{
	__super::Free();
}
