#include "stdafx.h"
#include "..\Public\State_Enemy_Gangster_Cap_Attack_Swing.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Effect_Manager.h"
#include "Camera.h"

CState_Enemy_Gangster_Cap_Attack_Swing::CState_Enemy_Gangster_Cap_Attack_Swing(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Gangster_Cap_Attack_Swing::CState_Enemy_Gangster_Cap_Attack_Swing(const CState_Enemy_Gangster_Cap_Attack_Swing& rhs)
	: CState_Enemy_Normal(rhs)
{

}

HRESULT CState_Enemy_Gangster_Cap_Attack_Swing::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_GANGSTER_CAP_DEAD_1);


	//m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_DAMAGE_B);
	//m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_DAMAGE_L);
	//m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_IDLE_NORMAL);

	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_DAMAGE_B] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_DAMAGE_L] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_IDLE_NORMAL] = 1;

	m_fAttackLength_Min = 4.f;
	m_fAttackLength_Max = 5.f;

	return S_OK;
}

HRESULT CState_Enemy_Gangster_Cap_Attack_Swing::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_Gangster_Cap_Attack_Swing::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;
	m_strRootBoneName = "root-bone";

	m_fDamage = fRandom(700.f, 800.f);

	m_pColCom[COLLIDER_WEAPON] = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Weapon()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));
}

void CState_Enemy_Gangster_Cap_Attack_Swing::Tick(_float fTimeDelta)
{
	Hit_Unit(m_pColCom[COLLIDER_WEAPON], ELEMENTAL_MELEE, m_fDamage, 41, 51, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_1]);

	if (Is_AnimFinished())
	{
		for (int i = 0; i < HIT_END; i++)
			m_bHit[i] = false;

		m_bEndCurState = true;
	}

	if (27 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("TimeIllusion_Ready", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			//XMVectorSet(0.f, 0.f, 2.f, 1.f),
			-XMVector3Normalize(GAMEINSTANCE->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));
	}

	if (42 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Cap_Attack", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.8f, 0.f, 1.f),
			XMVectorSet(-0.3f, -1.f, 0.f, 0.f), 1.f,
			XMVectorSet(2.5f, 2.f, 2.5f, 0.f)));

		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cap_Attack.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}	

	CState_Enemy_Normal::Tick(fTimeDelta);

}

void CState_Enemy_Gangster_Cap_Attack_Swing::Exit(void* pArg)
{
	for (int i = 0; i < HIT_END; i++)
		m_bHit[i] = false;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
}

_bool CState_Enemy_Gangster_Cap_Attack_Swing::Check_Condition(CState_Handler* pHandler)
{
	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (m_fAttackLength_Max > fLength && m_fAttackLength_Min < fLength)
		return true;

	return false;
}


CState_Enemy_Gangster_Cap_Attack_Swing* CState_Enemy_Gangster_Cap_Attack_Swing::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Gangster_Cap_Attack_Swing* pInstance = new CState_Enemy_Gangster_Cap_Attack_Swing(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Gangster_Cap_Attack_Swing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Gangster_Cap_Attack_Swing::Clone(void* pArg)
{
	CState_Enemy_Gangster_Cap_Attack_Swing* pInstance = new CState_Enemy_Gangster_Cap_Attack_Swing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Gangster_Cap_Attack_Swing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Gangster_Cap_Attack_Swing::Free()
{
	__super::Free();
}
