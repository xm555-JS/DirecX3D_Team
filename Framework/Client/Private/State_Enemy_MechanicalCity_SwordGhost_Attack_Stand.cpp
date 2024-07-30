#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_SwordGhost_Attack_Stand.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Effect_Manager.h"
#include "Camera.h"

CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand::CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand::CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand(const CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand& rhs)
	: CState_Enemy_Normal(rhs)
{

}

HRESULT CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand::Initialize_Prototype()
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


	m_fAttackLength_Min = 5.f;
	m_fAttackLength_Max = 7.f;

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand::Initialize(void * pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;
	m_fSpeed = 20.f;
	m_strRootBoneName = "Root";

	m_fDamage = fRandom(100, 120.f);

	m_pColCom[COLLIDER_OWNER] = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));
	m_pOwnerTranformCom->YFixedLookAt(m_pState_Handler->Get_Player()->Get_Pos());
}

void CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand::Tick(_float fTimeDelta)
{
	if (Is_Damaged())
		m_bEndCurState = true;

	if (7 < Get_CurrentKeyFrame() && 12 > Get_CurrentKeyFrame())
	{	
		m_fAnimSpeed = 0.5f;

		if (Get_PlayerPosLength() > 2.f)
		{
			CNavigation* pNavigation = (CNavigation*)m_pOwner->Get_Component(NAVIGATION);
			m_pOwnerTranformCom->Go_Straight(fTimeDelta, m_fSpeed, pNavigation);
		}
		
	}
	else
	{
		m_fAnimSpeed = 1.f;

	}

	m_fDamage = fRandom(50.f, 70.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 31, 33, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_1]);
	m_fDamage = fRandom(50.f, 70.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 36, 38, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_2]);
	m_fDamage = fRandom(50.f, 70.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 41, 43, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_3]);
	m_fDamage = fRandom(50.f, 70.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 46, 48, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_4]);
	m_fDamage = fRandom(50.f, 70.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 51, 54, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_5]);
	m_fDamage = fRandom(50.f, 70.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 56, 58, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_6]);
	m_fDamage = fRandom(50.f, 70.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage*2.f, 72, 74, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_7]);

	if (Is_AnimFinished())
		m_bEndCurState = true;

	if (24 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect0, EFFECT_POS("TimeIllusion_Ready", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			-XMVector3Normalize(GAMEINSTANCE->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]),
			//XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (30 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Ghost_Sword_Attack_Sustain_New", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, -1.5f, -1.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.3f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (30 < Get_CurKeyFrame() && 60 > Get_CurKeyFrame())
		IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Ghost_Attack.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT,m_pOwner->Get_Pos(), 0.1f), 0.1f, fTimeDelta);

	if (62 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect2, EFFECT_POS("Ghost_Sword_Charge", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 1.f, -0.3f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (66 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect3, EFFECT_POS("Ghost_Sword_Charge_Attack", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));

		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Ghost_Sustain_End.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));
	}

	CState_Enemy_Normal::Tick(fTimeDelta);

}

void CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand::Exit(void* pArg)
{
	for (int i = 0; i < HIT_END; i++)
		m_bHit[i] = false;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
}

_bool CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand::Check_Condition(CState_Handler* pHandler)
{
	if (Is_Damaged(pHandler))
		return false;

	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (m_fAttackLength_Min < fLength&& m_fAttackLength_Max > fLength)
		return true;

	return false;
}


CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand* CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand* pInstance = new CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand* pInstance = new CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand::Free()
{
	__super::Free();
}
