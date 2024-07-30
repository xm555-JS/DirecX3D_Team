#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_DoorKeeper_Turn_L.h"
#include "GameInstance.h"
#include "Enemy.h"

CState_Enemy_MechanicalCity_DoorKeeper_Turn_L::CState_Enemy_MechanicalCity_DoorKeeper_Turn_L(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{

}

CState_Enemy_MechanicalCity_DoorKeeper_Turn_L::CState_Enemy_MechanicalCity_DoorKeeper_Turn_L(const CState_Enemy_MechanicalCity_DoorKeeper_Turn_L& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_MechanicalCity_DoorKeeper_Turn_L::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DEAD_END);

	//m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DAMAGE_B);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_NORMAL);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_ATTACK_CHOP);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_ATTACK_JUMPCHOP);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_ATTACK_kEEPHITTING);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_RUN);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_RUN_RETURN);


	//m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DAMAGE_B] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_NORMAL] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_ATTACK_CHOP] = 5;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_ATTACK_JUMPCHOP] = 5;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_ATTACK_kEEPHITTING] = 5;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_RUN] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_RUN_RETURN] = 1;



	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_DoorKeeper_Turn_L::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_DoorKeeper_Turn_L::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;

	m_fEasingTime = m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_Duration();
	m_fAccTime = 0.f;

	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
}

void CState_Enemy_MechanicalCity_DoorKeeper_Turn_L::Tick(_float fTimeDelta)
{
	if (Is_AnimFinished())
		m_bEndCurState = true;

	m_fAccTime += fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();

	_vector vLook = Get_LookAtPlayerPos_Dir();

	_float	fCurTime = min(1.f, m_fAccTime / m_fEasingTime);

	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime);

	m_pOwnerTranformCom->YFixedLook(vFinalLook);

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_MechanicalCity_DoorKeeper_Turn_L::Exit(void* pArg)
{
}

_bool CState_Enemy_MechanicalCity_DoorKeeper_Turn_L::Check_Condition(CState_Handler* pHandler)
{
	_vector vCurLook, vGoalLook;
	vCurLook = pHandler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK);

	_float4 vGoalPos, vCurPos;
	XMStoreFloat4(&vGoalPos, pHandler->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	XMStoreFloat4(&vCurPos, pHandler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vGoalPos.y = vCurPos.y;
	vGoalLook = XMLoadFloat4(&vGoalPos) - XMLoadFloat4(&vCurPos);
	XMVector3Normalize(vGoalLook);

	_float fDgree = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(vCurLook, vGoalLook)));

	if (0 > XMVectorGetY(XMVector3Cross(vCurLook, vGoalLook)) && fDgree > 30.f)
		return true;

	return false;
}


CState_Enemy_MechanicalCity_DoorKeeper_Turn_L* CState_Enemy_MechanicalCity_DoorKeeper_Turn_L::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_DoorKeeper_Turn_L* pInstance = new CState_Enemy_MechanicalCity_DoorKeeper_Turn_L(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_DoorKeeper_Turn_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_DoorKeeper_Turn_L::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_DoorKeeper_Turn_L* pInstance = new CState_Enemy_MechanicalCity_DoorKeeper_Turn_L(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_DoorKeeper_Turn_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_DoorKeeper_Turn_L::Free()
{
	__super::Free();
}
