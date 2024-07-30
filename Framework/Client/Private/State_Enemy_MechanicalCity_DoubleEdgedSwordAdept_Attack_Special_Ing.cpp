#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Effect_Manager.h"

CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing(const CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing& rhs)
	: CState_Enemy_Normal(rhs)
{

}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing::Initialize_Prototype()
{

	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DEAD_END);


	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_END);


	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_END] = 1;


	m_fAttackLength_Min = 1.f;
	m_fAttackLength_Max = 3.f;

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fPlayTime = 4.f;

	m_fAnimSpeed = 1.f;
	m_strRootBoneName = "Root";

	m_fDamage = fRandom(30.f, 40.f);


	m_pColCom[COLLIDER_OWNER] = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));
	m_vStartColScale = m_pColCom[COLLIDER_OWNER]->Get_Scale();
	m_vStartColPos = m_pColCom[COLLIDER_OWNER]->Get_Position();

	m_pColCom[COLLIDER_OWNER]->Set_Scale(XMVectorSet(5.f, 5.f, 5.f, 1.f));
	m_pColCom[COLLIDER_OWNER]->Set_Position(m_vStartColPos);

}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing::Tick(_float fTimeDelta)
{
	if (Is_Damaged())
		m_bEndCurState = true;

	if (m_fPlayTime < m_pState_Handler->Get_TimeAcc())
	{
		m_bEndCurState = true;
	}
	else if (0.5f * m_iCount < m_pState_Handler->Get_TimeAcc())
	{
		m_fDamage = fRandom(30.f, 40.f);
		Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 0, 18, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_1]);
		m_bHit[HIT_1] = false;
		m_iCount++;

		RESET_ONCE(m_bSound[0]);
		RESET_ONCE(m_bSound[1]);

		if (m_iCount % 2 == 1)
		{
			RESET_ONCE(m_bEffect1);
			RESET_ONCE(m_bSound[2]);
		}
	}

	if (0 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect0, EFFECT_POS("DoubleSword_Attack_Tornado", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, -1.3f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(2.f, 2.f, 2.f, 0.f)));
	}

	if (2 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Double_Special.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	if (4 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Double_Special.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	if (5 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect1, EFFECT_POS("DoubleSword_Attack_Thunder", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.7f, 1.7f, 1.7f, 0.f)));
		
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Double_Thunder.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));
	}

	CState_Enemy_Normal::Tick(fTimeDelta);
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing::Exit(void* pArg)
{
	for (int i = 0; i < HIT_END; i++)
		m_bHit[i] = false;

	m_pColCom[COLLIDER_OWNER]->Set_Scale(m_vStartColScale);
	m_pColCom[COLLIDER_OWNER]->Set_Position(m_vStartColPos);

	m_iCount = 0;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
}

_bool CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing::Check_Condition(CState_Handler* pHandler)
{

	return true;
}


CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing::Free()
{
	__super::Free();
}
