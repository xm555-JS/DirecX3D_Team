#include "stdafx.h"
#include "..\Public\State_Enemy_Mutation_Mechanic_Attack_DownHit.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Effect_Manager.h"
#include "Camera.h"

CState_Enemy_Mutation_Mechanic_Attack_DownHit::CState_Enemy_Mutation_Mechanic_Attack_DownHit(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Mutation_Mechanic_Attack_DownHit::CState_Enemy_Mutation_Mechanic_Attack_DownHit(const CState_Enemy_Mutation_Mechanic_Attack_DownHit& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Mutation_Mechanic_Attack_DownHit::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DEAD_1);


	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_B);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_B] = 1;
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_L);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_L] = 1;
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_R);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_R] = 1;
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_F);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_F] = 1;

	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_IDLE_BATTLE);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_IDLE_BATTLE] = 1;


	return S_OK;
}

HRESULT CState_Enemy_Mutation_Mechanic_Attack_DownHit::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Enemy_Mutation_Mechanic_Attack_DownHit::Enter(void* pArg)
{

}

void CState_Enemy_Mutation_Mechanic_Attack_DownHit::Tick(_float fTimeDelta)
{
	if (16 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("TimeIllusion_Ready", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			-XMVector3Normalize(GAMEINSTANCE->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]),
			//XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (28 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Mutation_Mechanic_Down", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));

		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Mutation_Attack.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Mutation_Mechanic_Attack_DownHit::Exit(void* pArg)
{
	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
}

_bool CState_Enemy_Mutation_Mechanic_Attack_DownHit::Check_Condition(CState_Handler* pHandler)
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


CState_Enemy_Mutation_Mechanic_Attack_DownHit* CState_Enemy_Mutation_Mechanic_Attack_DownHit::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Mutation_Mechanic_Attack_DownHit* pInstance = new CState_Enemy_Mutation_Mechanic_Attack_DownHit(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Mutation_Mechanic_Attack_DownHit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Mutation_Mechanic_Attack_DownHit::Clone(void* pArg)
{
	CState_Enemy_Mutation_Mechanic_Attack_DownHit* pInstance = new CState_Enemy_Mutation_Mechanic_Attack_DownHit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Mutation_Mechanic_Attack_DownHit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Mutation_Mechanic_Attack_DownHit::Free()
{
	__super::Free();
}
