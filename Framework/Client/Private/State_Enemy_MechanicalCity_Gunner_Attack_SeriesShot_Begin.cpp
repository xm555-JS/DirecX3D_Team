#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Effect_Manager.h"
#include "Camera.h"

CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin::CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin::CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin(const CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DEAD_END);


	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_ING);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_END);


	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R] = 1;



	m_fAttackLength_Min = 2.5f;
	m_fAttackLength_Max = 10.f;

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin::Enter(void* pArg)
{
	__super::Enter(pArg);

	

}

void CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin::Tick(_float fTimeDelta)
{
	if (Is_Damaged())
		m_bEndCurState = true;

	CEnemy* pOwner = (CEnemy*)m_pState_Handler->Get_Owner();

	if (Is_AnimFinished())
	{
		if (nullptr == pOwner->Get_Target())
		{
			m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_ING] = 0;
			m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_END] = 1;
			m_bEndCurState = true;
		}
		else
		{
			m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_ING] = 1;
			m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_END] = 0;
			m_bEndCurState = true;
		}
			
	}
	m_pOwnerTranformCom->YFixedLookAt(m_pState_Handler->Get_Player()->Get_Pos());

	if (1 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("TimeIllusion_Ready", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			-XMVector3Normalize(GAMEINSTANCE->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]),
			//XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin::Exit(void* pArg)
{
	RESET_ONCE(m_bEffect0);
}

_bool CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin::Check_Condition(CState_Handler* pHandler)
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


CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin* CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin* pInstance = new CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin* pInstance = new CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin::Free()
{
	__super::Free();
}
