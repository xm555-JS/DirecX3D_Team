#include "stdafx.h"
#include "..\Public\State_Enemy_HyenaGang_Shooter_Turn_R.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_HyenaGang_Shooter_Turn_R::CState_Enemy_HyenaGang_Shooter_Turn_R(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HyenaGang_Shooter_Turn_R::CState_Enemy_HyenaGang_Shooter_Turn_R(const CState_Enemy_HyenaGang_Shooter_Turn_R& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HyenaGang_Shooter_Turn_R::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DEAD_1);

	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_F);
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_NORMAL);

	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_ATTACK_CHARGE_SHOT);
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_ATTACK_NORMAL_SHOT);

	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_F] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_NORMAL] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_ATTACK_CHARGE_SHOT] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_ATTACK_NORMAL_SHOT] = 7;

	return S_OK;

}

HRESULT CState_Enemy_HyenaGang_Shooter_Turn_R::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HyenaGang_Shooter_Turn_R::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;

	m_fEasingTime = m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_Duration();
	m_fAccTime = 0.f;

	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));

}

void CState_Enemy_HyenaGang_Shooter_Turn_R::Tick(_float fTimeDelta)
{
	if (Is_Damaged())
		m_bEndCurState = true;

	if (Is_AnimFinished())
		m_bEndCurState = true;

	m_fAccTime += fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();

	_vector vLook = Get_LookAtPlayerPos_Dir();

	_float	fCurTime = min(1.f, m_fAccTime / m_fEasingTime);

	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime);

	m_pOwnerTranformCom->YFixedLook(vFinalLook);


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HyenaGang_Shooter_Turn_R::Exit(void* pArg)
{

}

_bool CState_Enemy_HyenaGang_Shooter_Turn_R::Check_Condition(CState_Handler* pHandler)
{
	if (Is_Damaged(pHandler))
		return false;

	_vector vCurLook, vGoalLook;
	vCurLook = pHandler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK);

	_float4 vGoalPos, vCurPos;
	XMStoreFloat4(&vGoalPos, pHandler->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	XMStoreFloat4(&vCurPos, pHandler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vGoalPos.y = vCurPos.y;
	vGoalLook = XMLoadFloat4(&vGoalPos) - XMLoadFloat4(&vCurPos);
	XMVector3Normalize(vGoalLook);

	_float fDgree = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(vCurLook, vGoalLook)));

	if (fDgree > 30.f)
		return true;

	return false;
}


CState_Enemy_HyenaGang_Shooter_Turn_R* CState_Enemy_HyenaGang_Shooter_Turn_R::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HyenaGang_Shooter_Turn_R* pInstance = new CState_Enemy_HyenaGang_Shooter_Turn_R(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HyenaGang_Shooter_Turn_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HyenaGang_Shooter_Turn_R::Clone(void* pArg)
{
	CState_Enemy_HyenaGang_Shooter_Turn_R* pInstance = new CState_Enemy_HyenaGang_Shooter_Turn_R(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HyenaGang_Shooter_Turn_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HyenaGang_Shooter_Turn_R::Free()
{
	__super::Free();
}
