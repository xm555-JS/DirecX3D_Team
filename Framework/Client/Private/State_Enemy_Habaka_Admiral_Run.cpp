#include "stdafx.h"
#include "..\Public\State_Enemy_Habaka_Admiral_Run.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_Habaka_Admiral_Run::CState_Enemy_Habaka_Admiral_Run(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Habaka_Admiral_Run::CState_Enemy_Habaka_Admiral_Run(const CState_Enemy_Habaka_Admiral_Run& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Habaka_Admiral_Run::Initialize_Prototype()//���󰡴ٰ� �÷��̾ �־����ٸ�?  ->Idle_Battle
{																//���󰡴ٰ� �÷��̾ ������ ����ٸ�?	->Idle_Battle
	m_ForceStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DEAD_1);

	//m_ForceStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_FRONTJUMP);

	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F);

	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE);

	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_RUN_RETURN);

	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_RUN_RETURN] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_FRONTJUMP] = 1;

	m_fRunStartDistance = 5.f; //�÷��̾���� �Ÿ��� m_fRunStartDistance���� ũ�ٸ� �����Ѵ�.

	return S_OK;
}

HRESULT CState_Enemy_Habaka_Admiral_Run::Initialize(void* pArg)
{
	m_fRunEndDistance = 3.f;

	return S_OK;
}

void CState_Enemy_Habaka_Admiral_Run::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fPlayTime = 2.5f;
	m_fAnimSpeed = 0.8f;

	m_fSpeed = 7.f;


}

void CState_Enemy_Habaka_Admiral_Run::Tick(_float fTimeDelta)
{
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

	if (m_fPlayTime < m_pState_Handler->Get_TimeAcc())
	{
		//�޸��ٰ� ���߰� �ൿ�� ����
		m_bEndCurState = true;
	}

	IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Habaka_Foot.wav"), CSound_Manager::CHANNEL_ENEMY, m_pOwner->Get_Pos(), 0.1f), 0.6f, fTimeDelta);

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Habaka_Admiral_Run::Exit(void* pArg)
{

}

_bool CState_Enemy_Habaka_Admiral_Run::Check_Condition(CState_Handler* pHandler)
{
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


CState_Enemy_Habaka_Admiral_Run* CState_Enemy_Habaka_Admiral_Run::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Habaka_Admiral_Run* pInstance = new CState_Enemy_Habaka_Admiral_Run(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Habaka_Admiral_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Habaka_Admiral_Run::Clone(void* pArg)
{
	CState_Enemy_Habaka_Admiral_Run* pInstance = new CState_Enemy_Habaka_Admiral_Run(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Habaka_Admiral_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Habaka_Admiral_Run::Free()
{
	__super::Free();
}
