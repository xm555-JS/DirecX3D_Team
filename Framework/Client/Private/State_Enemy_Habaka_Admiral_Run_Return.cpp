#include "stdafx.h"
#include "..\Public\State_Enemy_Habaka_Admiral_Run_Return.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_Habaka_Admiral_Run_Return::CState_Enemy_Habaka_Admiral_Run_Return(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Habaka_Admiral_Run_Return::CState_Enemy_Habaka_Admiral_Run_Return(const CState_Enemy_Habaka_Admiral_Run_Return& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Habaka_Admiral_Run_Return::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DEAD_1);

	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F);

	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_IDLE_NORMAL);
	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE);

	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_IDLE_NORMAL] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE] = 1;

	m_fReturnStartDistance = 20.f;
	m_fPatrolLength = 5.f;
	return S_OK;
}

HRESULT CState_Enemy_Habaka_Admiral_Run_Return::Initialize(void* pArg)
{
	m_fPlayerPosLength = 3.5f; //Ż������) �÷��̾���� �Ÿ��� m_fPlayerLength���� �۾����ٸ� 
	m_fStartPosLength = 5.f; //Ż������) ������������ �Ÿ��� m_fStartPosLength���� �۾����ٸ�
	
	return S_OK;
}

void CState_Enemy_Habaka_Admiral_Run_Return::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 0.8f;

	m_fSpeed = 7.f;

	m_fEasingTime = 2.f;
	m_fAccTime = 0.f;

	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
}

void CState_Enemy_Habaka_Admiral_Run_Return::Tick(_float fTimeDelta)//���
{
	_float fStartDistance, fPlayerDistance;

	fStartDistance = XMVectorGetX(XMVector3Length(((CEnemy*)m_pOwner)->Get_StartPos() - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION)));
	fPlayerDistance = XMVectorGetX(XMVector3Length(((CEnemy*)m_pTarget)->Get_Pos() - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION)));

	if (m_fStartPosLength > fStartDistance || m_fPlayerPosLength > fPlayerDistance)
		m_bEndCurState = true;

	_vector vGoalLook = ((CEnemy*)m_pOwner)->Get_StartPos() -
		m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

	m_fAccTime += fTimeDelta;

	_vector vLook = Get_LookAtStartPos_Dir();

	_float	fCurTime = min(1.f, m_fAccTime / m_fEasingTime);

	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime);

	m_pOwnerTranformCom->YFixedLook(vFinalLook);

	CNavigation* pNavigation = (CNavigation*)m_pState_Handler->Get_Owner()->Get_Component(NAVIGATION);
	m_pState_Handler->Get_Owner()->Get_Transform()->Go_Straight(fTimeDelta, m_fSpeed, pNavigation);

	IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Habaka_Foot.wav"), CSound_Manager::CHANNEL_ENEMY, m_pOwner->Get_Pos(), 0.1f), 0.53f, fTimeDelta);

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Habaka_Admiral_Run_Return::Exit(void* pArg)
{

}

_bool CState_Enemy_Habaka_Admiral_Run_Return::Check_Condition(CState_Handler* pHandler)//����
{
	CEnemy* pOwner = (CEnemy*)pHandler->Get_Owner();

	if (nullptr == pOwner->Get_Target())
	{
		if (m_fPatrolLength > Get_StartPosLength(pHandler))
			return false;

		return true;
	}

	_float fDistance;
	fDistance = XMVectorGetX(XMVector3Length(pOwner->Get_StartPos() - pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

	if (m_fReturnStartDistance < fDistance) //������������ �Ÿ��� 20.f���� Ŀ���ٸ� �����Ѵ�.
		return true;

	return false;
}


CState_Enemy_Habaka_Admiral_Run_Return* CState_Enemy_Habaka_Admiral_Run_Return::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Habaka_Admiral_Run_Return* pInstance = new CState_Enemy_Habaka_Admiral_Run_Return(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Habaka_Admiral_Run_Return");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Habaka_Admiral_Run_Return::Clone(void* pArg)
{
	CState_Enemy_Habaka_Admiral_Run_Return* pInstance = new CState_Enemy_Habaka_Admiral_Run_Return(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Habaka_Admiral_Run_Return");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Habaka_Admiral_Run_Return::Free()
{
	__super::Free();
}
