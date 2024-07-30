#include "stdafx.h"
#include "..\Public\State_Enemy_Habaka_Admiral_Attack_Kick_A.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Camera.h"
CState_Enemy_Habaka_Admiral_Attack_Kick_A::CState_Enemy_Habaka_Admiral_Attack_Kick_A(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Habaka_Admiral_Attack_Kick_A::CState_Enemy_Habaka_Admiral_Attack_Kick_A(const CState_Enemy_Habaka_Admiral_Attack_Kick_A& rhs)
	: CState_Enemy_Normal(rhs)
{

}

HRESULT CState_Enemy_Habaka_Admiral_Attack_Kick_A::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DEAD_1);


	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_IDLE_NORMAL);

	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_IDLE_NORMAL] = 1;

	m_fAttackLength_Min = 2.5f;
	m_fAttackLength_Max = 5.f;

	return S_OK;
}

HRESULT CState_Enemy_Habaka_Admiral_Attack_Kick_A::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_Habaka_Admiral_Attack_Kick_A::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;
	m_fDamage = fRandom(800.f, 1000.f);

	m_pColCom[COLLIDER_OWNER] = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));


}

void CState_Enemy_Habaka_Admiral_Attack_Kick_A::Tick(_float fTimeDelta)
{
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 44, 50, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_1]);

	if (Is_AnimFinished())
		m_bEndCurState = true;

	if (14 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("TimeIllusion_Ready", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			//XMVectorSet(0.f, 0.5f, 0.f, 1.f),
			-XMVector3Normalize(GAMEINSTANCE->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));
	}

	if (38 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Habaka_Kick_Attack", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));

		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Habaka_Kick.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Habaka_Admiral_Attack_Kick_A::Exit(void* pArg)
{
	for (int i = 0; i < HIT_END; i++)
		m_bHit[i] = false;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
}

_bool CState_Enemy_Habaka_Admiral_Attack_Kick_A::Check_Condition(CState_Handler* pHandler)
{
	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (m_fAttackLength_Max > fLength && m_fAttackLength_Min < fLength)
		return true;

	return false;
}


CState_Enemy_Habaka_Admiral_Attack_Kick_A* CState_Enemy_Habaka_Admiral_Attack_Kick_A::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Habaka_Admiral_Attack_Kick_A* pInstance = new CState_Enemy_Habaka_Admiral_Attack_Kick_A(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Habaka_Admiral_Attack_Kick_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Habaka_Admiral_Attack_Kick_A::Clone(void* pArg)
{
	CState_Enemy_Habaka_Admiral_Attack_Kick_A* pInstance = new CState_Enemy_Habaka_Admiral_Attack_Kick_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Habaka_Admiral_Attack_Kick_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Habaka_Admiral_Attack_Kick_A::Free()
{
	__super::Free();
}
