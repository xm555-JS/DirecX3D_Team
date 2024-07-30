#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Effect_Manager.h"
#include "Camera.h"

CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting::CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting::CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting(const CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting& rhs)
	: CState_Enemy_Normal(rhs)
{

}

HRESULT CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting::Initialize_Prototype()
{

	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DEAD_END);


	//m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DAMAGE_B);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_NORMAL);


	//m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DAMAGE_B] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_NORMAL] = 1;


	m_fAttackLength_Min = 1.5f;
	m_fAttackLength_Max = 4.f;

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;
	m_fSpeed = 20.f;

	m_strRootBoneName = "Root_Bone";

	m_fDamage = fRandom(120.f, 140.f);

	m_pColCom[COLLIDER_OWNER] = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));
}

void CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting::Tick(_float fTimeDelta)
{
	m_fDamage = fRandom(120.f, 140.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 56, 60, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_1]);
	m_fDamage = fRandom(120.f, 140.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 68, 72, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_2]);
	m_fDamage = fRandom(120.f, 140.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 84, 88, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_3]);
	m_fDamage = fRandom(120.f, 140.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 94, 97, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_4]);
	m_fDamage = fRandom(120.f, 140.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 100, 103, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_5]);
	m_fDamage = fRandom(120.f, 140.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 110, 113, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_6]);
	m_fDamage = fRandom(120.f, 140.f);
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 148, 150, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_7]);

	if (Is_AnimFinished())
	{
		for (int i = 0; i < HIT_END; i++)
			m_bHit[i] = false;

		m_bEndCurState = true;
	}

	if (25 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect0, EFFECT_POS("TimeIllusion_Ready", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			-XMVector3Normalize(GAMEINSTANCE->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]),
			//XMVectorSet(0.f, 1.f, 0.3f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));
	}

	if (50 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect1, EFFECT_POS("DoorKeeper_Keep_R", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (56 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("DoorKeeper_Keep.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	if (62 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect2, EFFECT_POS("DoorKeeper_Keep_L", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (68 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("DoorKeeper_Keep.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	if (80 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect3, EFFECT_POS("DoorKeeper_Keep_R", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (86 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("DoorKeeper_Keep.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	if (88 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect4, EFFECT_POS("DoorKeeper_Keep_L", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (94 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[3], MGR(CSound_Manager)->PlaySurroundSound(TEXT("DoorKeeper_Keep.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	if (95 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect5, EFFECT_POS("DoorKeeper_Keep_R", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (101 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[4], MGR(CSound_Manager)->PlaySurroundSound(TEXT("DoorKeeper_Keep.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	if (105 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect6, EFFECT_POS("DoorKeeper_Keep_L", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (111 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[5], MGR(CSound_Manager)->PlaySurroundSound(TEXT("DoorKeeper_Keep.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	if (143 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect7, EFFECT_POS("DoorKeeper_Chop", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(-0.5f, 0.3f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (146 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[6], MGR(CSound_Manager)->PlaySurroundSound(TEXT("DoorKeeperChop.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	CState_Enemy_Normal::Tick(fTimeDelta);

}

void CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting::Exit(void* pArg)
{
	for (int i = 0; i < HIT_END; i++)
		m_bHit[i] = false;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
	RESET_ONCE(m_bEffect4);
	RESET_ONCE(m_bEffect5);
	RESET_ONCE(m_bEffect6);
	RESET_ONCE(m_bEffect7);
}

_bool CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting::Check_Condition(CState_Handler* pHandler)
{
	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (m_fAttackLength_Min < fLength && m_fAttackLength_Max > fLength)
		return true;

	return false;
}


CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting* CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting* pInstance = new CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting* pInstance = new CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting::Free()
{
	__super::Free();
}
