#include "stdafx.h"
#include "..\Public\State_Enemy_Gangster_Cap_Run.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_Gangster_Cap_Run::CState_Enemy_Gangster_Cap_Run(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Gangster_Cap_Run::CState_Enemy_Gangster_Cap_Run(const CState_Enemy_Gangster_Cap_Run& rhs)
	: CState_Enemy_Normal(rhs)
{
	m_fRunStartDistance = rhs.m_fRunStartDistance;

}

HRESULT CState_Enemy_Gangster_Cap_Run::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_GANGSTER_CAP_DEAD_1);
	m_ForceStates.push_back(STATE_ENEMY_GANGSTER_CAP_RUN_RETURN);

	//STATE_ENEMY_GANGSTER_CAP_DAMAGE_B);
	//STATE_ENEMY_GANGSTER_CAP_DAMAGE_L);
	//STATE_ENEMY_GANGSTER_CAP_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_IDLE_BATTLE);

	//m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_RUN_RETURN);


	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_DAMAGE_B] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_DAMAGE_L] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_IDLE_BATTLE] = 1;
	
	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_RUN_RETURN] = 1;


	m_fRunStartDistance = 5.f; //플레이어와의 거리가 m_fRunStartDistance보다 크다면 실행한다.

	return S_OK;
}

HRESULT CState_Enemy_Gangster_Cap_Run::Initialize(void * pArg)
{

	m_fRunEndDistance = 4.f;

	return S_OK;
}

void CState_Enemy_Gangster_Cap_Run::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fPlayTime = 2.5f;
	m_fAnimSpeed = 0.8f;

	m_fSpeed = 7.f;

}

void CState_Enemy_Gangster_Cap_Run::Tick(_float fTimeDelta)
{
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

	//if (m_fPlayTime < m_pState_Handler->Get_TimeAcc())
	//{
	//	//달리다가 멈추고 행동할 동작
	//	m_bEndCurState = true;
	//}

	IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cap_Foot.wav"), CSound_Manager::CHANNEL_ENEMY, m_pOwner->Get_Pos(), 0.1f), 0.4f, fTimeDelta);

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Gangster_Cap_Run::Exit(void* pArg)
{

}

_bool CState_Enemy_Gangster_Cap_Run::Check_Condition(CState_Handler* pHandler)
{
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


CState_Enemy_Gangster_Cap_Run* CState_Enemy_Gangster_Cap_Run::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Gangster_Cap_Run* pInstance = new CState_Enemy_Gangster_Cap_Run(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Gangster_Cap_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Gangster_Cap_Run::Clone(void* pArg)
{
	CState_Enemy_Gangster_Cap_Run* pInstance = new CState_Enemy_Gangster_Cap_Run(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Gangster_Cap_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Gangster_Cap_Run::Free()
{
	__super::Free();
}
