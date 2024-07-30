#include "stdafx.h"
#include "..\Public\State_Enemy_HyenaGang_Shooter_Run.h"
#include "GameInstance.h"
#include "Enemy.h"

CState_Enemy_HyenaGang_Shooter_Run::CState_Enemy_HyenaGang_Shooter_Run(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HyenaGang_Shooter_Run::CState_Enemy_HyenaGang_Shooter_Run(const CState_Enemy_HyenaGang_Shooter_Run& rhs)
	: CState_Enemy_Normal(rhs)
{
	m_fRunStartDistance = rhs.m_fRunStartDistance;

}

HRESULT CState_Enemy_HyenaGang_Shooter_Run::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DEAD_1);

	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_F);
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_BATTLE);

	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_RUN_RETURN);


	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_F] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_BATTLE] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_RUN_RETURN] = 1;


	m_fRunStartDistance = 10.f; //플레이어와의 거리가 m_fRunStartDistance보다 크다면 실행한다.



	return S_OK;
}

HRESULT CState_Enemy_HyenaGang_Shooter_Run::Initialize(void * pArg)
{
	m_fRunEndDistance = 7.f;
	return S_OK;
}

void CState_Enemy_HyenaGang_Shooter_Run::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fPlayTime = 2.5f;
	m_fAnimSpeed = 1.f;

	m_fSpeed = 3.5f;

}

void CState_Enemy_HyenaGang_Shooter_Run::Tick(_float fTimeDelta)
{
	if (Is_Damaged())
		m_bEndCurState = true;

	/*등반할때 플레이어를 바라보고가는 룩을 고정*/
	m_pOwnerTranformCom->YFixedLookAt(m_pTarget->Get_Pos());
	CNavigation* pNavigation = (CNavigation*)m_pOwner->Get_Component(NAVIGATION);
	m_pOwnerTranformCom->Go_Straight(fTimeDelta, m_fSpeed, pNavigation);

	_float fDistance = XMVectorGetX(XMVector3Length(m_pTarget->Get_Pos()
		- m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION)));

	if (fDistance < m_fRunEndDistance)	// 플레이어와의거리가 m_fRunEndDistance보다 작아지면 못따라가게 탈출
	{
		m_bEndCurState = true;
	}

	if (m_fPlayTime < m_pState_Handler->Get_TimeAcc())
	{
		//달리다가 멈추고 행동할 동작
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HyenaGang_Shooter_Run::Exit(void* pArg)
{

}

_bool CState_Enemy_HyenaGang_Shooter_Run::Check_Condition(CState_Handler* pHandler)
{
	if (Is_Damaged(pHandler))
		return false;

	CEnemy* pOwner = (CEnemy*)pHandler->Get_Owner();
	_float fDistance;

	if (nullptr != pOwner->Get_Target())//플레이어가 주변에 있는 상태에서 플레이어와의 거리가 m_fRunStartDistance 보다 클때만 가능
	{
		fDistance = XMVectorGetX(XMVector3Length(pOwner->Get_Target()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)
			- pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

		if (fDistance > m_fRunStartDistance)
			return true;
	}

	return false;
}


CState_Enemy_HyenaGang_Shooter_Run* CState_Enemy_HyenaGang_Shooter_Run::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HyenaGang_Shooter_Run* pInstance = new CState_Enemy_HyenaGang_Shooter_Run(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HyenaGang_Shooter_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HyenaGang_Shooter_Run::Clone(void* pArg)
{
	CState_Enemy_HyenaGang_Shooter_Run* pInstance = new CState_Enemy_HyenaGang_Shooter_Run(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HyenaGang_Shooter_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HyenaGang_Shooter_Run::Free()
{
	__super::Free();
}
