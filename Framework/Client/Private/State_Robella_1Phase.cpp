#include "stdafx.h"
#include "..\Public\State_Robella_1Phase.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_1Phase::CState_Robella_1Phase(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_1Phase::CState_Robella_1Phase(const CState_Robella_1Phase& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_1Phase::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_ROBELLA_DEAD);
	m_ForceStates.push_back(STATE_BOSS_ROBELLA_1PHASE_DOWN);
	m_ForceStates.push_back(STATE_BOSS_ROBELLA_CHANGE_PHASE);

	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_03);		// 머리흔들기
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_01);		// 파이어볼 사방 6개
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_02_BEGIN);	// Loop상태일때 꽃기둥 ( 아직 생성 X )
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_01);		// 파이어볼 사방 6개

	return S_OK;
}

HRESULT CState_Robella_1Phase::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_1Phase::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAccTime = 0.f;
	m_fEasingTime = 2.5f;

	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
}

void CState_Robella_1Phase::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vLook = XMLoadFloat4(&vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float	fCurTime = min(1.f, m_fAccTime / m_fEasingTime);

	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime * fCurTime);

	_float fDgree = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(vLook, vFinalLook)));

	m_pOwnerTranformCom->YFixedLook(vFinalLook);

	if (1.5f < m_fAccTime && 20.f > fDgree)
	{
		m_bEndCurState = true;
	}

	CState_Boss::Tick(fTimeDelta);
}

void CState_Robella_1Phase::Exit(void* pArg)
{
}

_bool CState_Robella_1Phase::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Robella_1Phase* CState_Robella_1Phase::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_1Phase* pInstance = new CState_Robella_1Phase(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_1Phase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_1Phase::Clone(void* pArg)
{
	CState_Robella_1Phase* pInstance = new CState_Robella_1Phase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_1Phase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_1Phase::Free()
{
	__super::Free();
}
