#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Effect_Manager.h"
#include "Camera.h"

CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop::CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop::CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop(const CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop& rhs)
	: CState_Enemy_Normal(rhs)
{

}

HRESULT CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop::Initialize_Prototype()
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

HRESULT CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;
	m_fSpeed = 5.f;
	m_strRootBoneName = "Root_Bone";

	m_fDamage = fRandom(800.f, 1000.f);

	m_pColCom[COLLIDER_OWNER] = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));
	m_vStartColScale = m_pColCom[COLLIDER_OWNER]->Get_Scale();
	m_vStartColPos = m_pColCom[COLLIDER_OWNER]->Get_Position();
	m_pColCom[COLLIDER_OWNER]->Set_Scale(XMVectorSet(5.f, 5.f, 5.f, 1.f));
}

void CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop::Tick(_float fTimeDelta)
{
	if (47 < Get_CurKeyFrame() && 73 > Get_CurKeyFrame())
	{
		CNavigation* pNavigation = (CNavigation*)m_pOwner->Get_Component(NAVIGATION);
		m_pOwnerTranformCom->Go_Straight(fTimeDelta, m_fSpeed, pNavigation);
	}
	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 78, 82, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_1]);

	if (Is_AnimFinished())
		m_bEndCurState = true;

	if (38 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect0, EFFECT_POS("TimeIllusion_Ready", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			-XMVector3Normalize(GAMEINSTANCE->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]),
			//XMVectorSet(0.f, 1.f, 0.3f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));
	}

	if (41 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("DoorKeeper_Jump.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	if (65 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect1, EFFECT_POS("DoorKeeper_Jump_Chop", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (71 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("DoorKeeperChop.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	CState_Enemy_Normal::Tick(fTimeDelta);

}

void CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop::Exit(void* pArg)
{
	for (int i = 0; i < HIT_END; i++)
		m_bHit[i] = false;

	m_pColCom[COLLIDER_OWNER]->Set_Scale(m_vStartColScale);
	m_pColCom[COLLIDER_OWNER]->Set_Position(m_vStartColPos);

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
}

_bool CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop::Check_Condition(CState_Handler* pHandler)
{
	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (m_fAttackLength_Min < fLength && m_fAttackLength_Max > fLength)
		return true;

	return false;
}


CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop* CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop* pInstance = new CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop* pInstance = new CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop::Free()
{
	__super::Free();
}
