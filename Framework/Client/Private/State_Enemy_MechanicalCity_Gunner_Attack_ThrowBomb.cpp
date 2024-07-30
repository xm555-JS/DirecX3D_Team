#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Effect_Manager.h"
#include "Camera.h"

CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb::CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb::CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb(const CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DEAD_END);


	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_NORMAL);


	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_NORMAL] = 1;


	m_fAttackLength_Min = 2.5f;
	m_fAttackLength_Max = 10.f;


	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;
	m_fDamage = fRandom(400.f, 500.f);

	m_pColCom[COLLIDER_OWNER] = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));

	m_vStartColScale = m_pColCom[COLLIDER_OWNER]->Get_Scale();
	m_vStartColPos = m_pColCom[COLLIDER_OWNER]->Get_Position();
	m_pColCom[COLLIDER_OWNER]->Set_Scale(XMVectorSet(3.5f, 3.5f, 3.5f, 1.f));
	m_pColCom[COLLIDER_OWNER]->Set_Position(XMVectorSet(0.f, 0.f, 4.5f, 1.f));

}

void CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb::Tick(_float fTimeDelta)
{
	if (Is_Damaged())
		m_bEndCurState = true;

	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 43, 50, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_1]);

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

	if (42 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Gunner_Shot", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 1.1f, 0.2f, 1.f),
			XMVectorSet(1.7f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(0.3f, 0.3f, 0.3f, 0.f)));

		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Gunner_Shot.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));
	}
		

	if (45.f < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect2, EFFECT_POS("Gunner_Bomb", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, -2.f, 4.7, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(3.f, 3.f, 3.f, 0.f)));

		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("ThrowBoulder.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));
	}	

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb::Exit(void* pArg)
{
	for (int i = 0; i < HIT_END; i++)
		m_bHit[i] = false;

	m_pColCom[COLLIDER_OWNER]->Set_Scale(m_vStartColScale);
	m_pColCom[COLLIDER_OWNER]->Set_Position(m_vStartColPos);

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
}

_bool CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb::Check_Condition(CState_Handler* pHandler)
{
	if (Is_Damaged(pHandler))
		return false;

	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (m_fAttackLength_Max > fLength && m_fAttackLength_Min < fLength)
		return true;

	return false;
}


CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb* CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb* pInstance = new CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb* pInstance = new CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb::Free()
{
	__super::Free();
}
