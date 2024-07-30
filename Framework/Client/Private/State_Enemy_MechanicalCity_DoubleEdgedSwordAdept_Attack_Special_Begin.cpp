#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Camera.h"
CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin(const CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin& rhs)
	: CState_Enemy_Normal(rhs)
{

}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin::Initialize_Prototype()
{

	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DEAD_END);


	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_END);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_ING);



	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R] = 1;



	m_fAttackLength_Min = 1.f;
	m_fAttackLength_Max = 3.f;

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin::Initialize(void * pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;
	m_strRootBoneName = "Root";

	m_pColCom[COLLIDER_OWNER] = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));

}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin::Tick(_float fTimeDelta)
{
	if (Is_Damaged())
		m_bEndCurState = true;




	if (Is_AnimFinished())
	{
		CEnemy* pOwner = (CEnemy*)m_pState_Handler->Get_Owner();

		if (nullptr == pOwner->Get_Target())
		{
			m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_END] = 1;
			m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_ING] = 0;
		}
		else
		{
			m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_END] = 0;
			m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_ING] = 1;
		}

		m_bEndCurState = true;

	}

	if (7 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect0, EFFECT_POS("TimeIllusion_Ready", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			-XMVector3Normalize(GAMEINSTANCE->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]),
			//XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	CState_Enemy_Normal::Tick(fTimeDelta);

}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin::Exit(void* pArg)
{
	RESET_ONCE(m_bEffect0);
}

_bool CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin::Check_Condition(CState_Handler* pHandler)
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


CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin::Free()
{
	__super::Free();
}
