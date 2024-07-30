#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run(const CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run& rhs)
	: CState_Enemy_Normal(rhs)
{
	m_fRunStartDistance = rhs.m_fRunStartDistance;

}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DEAD_END);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_BATTLE);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_RUN_RETURN);


	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_BATTLE] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_RUN_RETURN] = 1;
	
	m_fRunStartDistance = 3.f; //�÷��̾���� �Ÿ��� m_fRunStartDistance���� ũ�ٸ� �����Ѵ�.

	
	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run::Initialize(void* pArg)
{

	m_fRunEndDistance = 3.f;
	m_fReturnStartDistance = 30.f;


	return S_OK;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fPlayTime = 2.5f;
	m_fAnimSpeed = 1.f;

	m_fSpeed = 5.f;

}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run::Tick(_float fTimeDelta)
{

	if (m_fReturnStartDistance < Get_StartPosLength(m_pState_Handler))
		m_bEndCurState = true;

	if (Is_Damaged())
		m_bEndCurState = true;

	CEnemy* pOwner = (CEnemy*)m_pState_Handler->Get_Owner();
	if (nullptr == pOwner->Get_Target())//�÷��̾ �ֺ��� �ִ� ���¿��� �÷��̾���� �Ÿ��� m_fRunStartDistance ���� Ŭ���� ����
	{
		m_bEndCurState = true;
	}

	/*����Ҷ� �÷��̾ �ٶ󺸰��� ���� ����*/
	m_pOwnerTranformCom->YFixedLookAt(m_pTarget->Get_Pos());
	CNavigation* pNavigation = (CNavigation*)m_pOwner->Get_Component(NAVIGATION);
	m_pOwnerTranformCom->Go_Straight(fTimeDelta, m_fSpeed, pNavigation);

	_float fDistance = XMVectorGetX(XMVector3Length(m_pTarget->Get_Pos()
		- m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION)));

	if (fDistance < m_fRunEndDistance)	// �÷��̾���ǰŸ��� m_fRunEndDistance���� �۾����� �����󰡰� Ż��
	{
		m_bEndCurState = true;
	}

	//if (m_fPlayTime < m_pState_Handler->Get_TimeAcc())
	//{
	//	//�޸��ٰ� ���߰� �ൿ�� ����
	//	m_bEndCurState = true;
	//}

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run::Exit(void* pArg)
{

}

_bool CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run::Check_Condition(CState_Handler* pHandler)
{
	if (Is_Damaged(pHandler))
		return false;

	CEnemy* pOwner = (CEnemy*)pHandler->Get_Owner();
	_float fDistance;

	if (nullptr != pOwner->Get_Target())//�÷��̾ �ֺ��� �ִ� ���¿��� �÷��̾���� �Ÿ��� m_fRunStartDistance ���� Ŭ���� ����
	{
		fDistance = XMVectorGetX(XMVector3Length(pOwner->Get_Target()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)
			- pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

		if (fDistance > m_fRunStartDistance)
			return true;
	}

	return false;
}


CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run::Free()
{
	__super::Free();
}
