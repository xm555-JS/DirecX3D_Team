#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return(const CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DEAD_END);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_NORMAL);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_BATTLE);

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_NORMAL] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_BATTLE] = 1;

	m_fReturnStartDistance = 30.f;
	m_fPatrolLength = 5.f;
	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return::Initialize(void* pArg)
{
	m_fPlayerPosLength = 3.f; //탈출조건) 플레이어와의 거리가 m_fPlayerLength보다 작아진다면 
	m_fStartPosLength = 3.f; //탈출조건) 시작지점과의 거리가 m_fStartPosLength보다 작아진다면

	return S_OK;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;

	m_fSpeed = 5.f;

	m_fEasingTime = 2.f;
	m_fAccTime = 0.f;

	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return::Tick(_float fTimeDelta)
{
	if (Is_Damaged())
		m_bEndCurState = true;

	_float fStartDistance, fPlayerDistance;
	_vector  pos = ((CEnemy*)m_pOwner)->Get_StartPos();
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

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return::Exit(void* pArg)
{

}

_bool CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return::Check_Condition(CState_Handler* pHandler)
{
	if (Is_Damaged(pHandler))
		return false;

	CEnemy* pOwner = (CEnemy*)pHandler->Get_Owner();

	if (nullptr == pOwner->Get_Target())
	{
		if (m_fPatrolLength > Get_StartPosLength(pHandler))
			return false;

		return true;
	}

	_float fDistance;
	fDistance = XMVectorGetX(XMVector3Length(pOwner->Get_StartPos() - pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

	if (m_fReturnStartDistance < fDistance) //시작지점과의 거리가 20.f보다 커진다면 실행한다.
		return true;

	return false;
}

CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return::Free()
{
	__super::Free();
}
