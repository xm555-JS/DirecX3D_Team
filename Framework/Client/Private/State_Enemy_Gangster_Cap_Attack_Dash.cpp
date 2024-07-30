#include "stdafx.h"
#include "..\Public\State_Enemy_Gangster_Cap_Attack_Dash.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Effect_Manager.h"
#include "Camera.h"

CState_Enemy_Gangster_Cap_Attack_Dash::CState_Enemy_Gangster_Cap_Attack_Dash(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Gangster_Cap_Attack_Dash::CState_Enemy_Gangster_Cap_Attack_Dash(const CState_Enemy_Gangster_Cap_Attack_Dash& rhs)
	: CState_Enemy_Normal(rhs)
{

}

HRESULT CState_Enemy_Gangster_Cap_Attack_Dash::Initialize_Prototype()
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

	m_fAttackLength_Min = 2.5f;
	m_fAttackLength_Max = 7.f;

	return S_OK;
}

HRESULT CState_Enemy_Gangster_Cap_Attack_Dash::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_Gangster_Cap_Attack_Dash::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;
	m_strRootBoneName = "root-bone";

	m_fDamage = fRandom(700.f, 900.f);


	m_pColCom[COLLIDER_OWNER] = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));

}

void CState_Enemy_Gangster_Cap_Attack_Dash::Tick(_float fTimeDelta)
{
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 0, 101, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_1]);

	if (Is_AnimFinished())
		m_bEndCurState = true;

	if (9 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Cap_Dash_Attack", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(-0.35f, 2.2f, 2.f, 1.f),
			XMVectorSet(0.f, 0.f, -4.f, 0.f), 1.f,
			XMVectorSet(0.3f, 0.3f, 0.3f, 0.f)));
	}

	if (15 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect1, EFFECT_POS("TimeIllusion_Ready", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			//XMVectorSet(0.f, 0.f, 2.f, 1.f),
			-XMVector3Normalize(GAMEINSTANCE->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));
	}

	if (18 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect2, EFFECT_POS("Cap_Dash_Attack", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(-0.35f, 2.2f, 2.f, 1.f),
			XMVectorSet(0.f, 0.f, -4.f, 0.f), 1.f,
			XMVectorSet(0.3f, 0.3f, 0.3f, 0.f)));
	}

	if (21 < Get_CurKeyFrame())
		IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cap_Dash.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f), 0.32f, fTimeDelta);

	CState_Enemy_Normal::Tick(fTimeDelta);

}

void CState_Enemy_Gangster_Cap_Attack_Dash::Exit(void* pArg)
{
	for (int i = 0; i < HIT_END; i++)
		m_bHit[i] = false;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
}

_bool CState_Enemy_Gangster_Cap_Attack_Dash::Check_Condition(CState_Handler* pHandler)
{
	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (m_fAttackLength_Min < fLength && m_fAttackLength_Max > fLength)
		return true;

	return false;
}


CState_Enemy_Gangster_Cap_Attack_Dash* CState_Enemy_Gangster_Cap_Attack_Dash::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Gangster_Cap_Attack_Dash* pInstance = new CState_Enemy_Gangster_Cap_Attack_Dash(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Gangster_Cap_Attack_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Gangster_Cap_Attack_Dash::Clone(void* pArg)
{
	CState_Enemy_Gangster_Cap_Attack_Dash* pInstance = new CState_Enemy_Gangster_Cap_Attack_Dash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Gangster_Cap_Attack_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Gangster_Cap_Attack_Dash::Free()
{
	__super::Free();
}
