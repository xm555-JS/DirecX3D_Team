#include "stdafx.h"
#include "..\Public\State_Robella_Apear.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_Apear::CState_Robella_Apear(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Apear::CState_Robella_Apear(const CState_Robella_Apear& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Apear::Initialize_Prototype()
{
	// 다음으로 넘어갈 수 있는 state      
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_CAM_ACTION);
	//m_ReserveStates.push_back(STATE_BOSS_ROBELLA_IDLE2);
	//m_ReserveStates.push_back(STATE_BOSS_ROBELLA_2PHASE);

	// 다음 state로 넘어갈 확률
	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_CAM_ACTION] = 1.f;
	//m_ReserveProbabilitys[STATE_BOSS_ROBELLA_IDLE2] = 1.f;
	//m_ReserveProbabilitys[STATE_BOSS_ROBELLA_2PHASE] = 1.f;

	return S_OK;
}

HRESULT CState_Robella_Apear::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Apear::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAccTime = 0.f;
	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
}

void CState_Robella_Apear::Tick(_float fTimeDelta)
{
	m_bEndCurState = true;

	m_fAccTime += fTimeDelta;

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vLook = XMLoadFloat4(&vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float	fCurTime = min(1.f, m_fAccTime / 2.f);

	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime * fCurTime);

	_float fDgree = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(vLook, vFinalLook)));

	m_pOwnerTranformCom->YFixedLook(vFinalLook);

	if (20.f > fDgree)
	{
		m_bEndCurState = true;
	}

	//m_fAccTime += fTimeDelta;
	//
	//if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	//	m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Apear::Exit(void* pArg)
{
}

_bool CState_Robella_Apear::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Robella_Apear* CState_Robella_Apear::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Apear* pInstance = new CState_Robella_Apear(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Apear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Apear::Clone(void* pArg)
{
	CState_Robella_Apear* pInstance = new CState_Robella_Apear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Apear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Apear::Free()
{
	__super::Free();
}