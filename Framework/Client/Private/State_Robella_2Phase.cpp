#include "stdafx.h"
#include "..\Public\State_Robella_2Phase.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_2Phase::CState_Robella_2Phase(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_2Phase::CState_Robella_2Phase(const CState_Robella_2Phase& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_2Phase::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_ROBELLA_DEAD);
	m_ForceStates.push_back(STATE_BOSS_ROBELLA_2PHASE_DOWN);

	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_ULT_READY);		// 30�ʸ��� �ڵ����� ���Ȱ�.
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_07);		 // ���̾ ����
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_05);		 // ä�� ª�� ����
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_09_BEGIN);	 // �ɱ��.(���� �ȴ�)
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_07);		 // ���̾ ����
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_06);		 // ä�� �ΰ�
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_04);		 // ���İ���

	//m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_08);		 // �Ⱦ� X 

	return S_OK;
}

HRESULT CState_Robella_2Phase::Initialize(void* pArg)
{
	return S_OK;

}

void CState_Robella_2Phase::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fEasingTime = 2.5f;
	m_fAccTime = 0.f;

	m_fAnimSpeed = 1.f;

	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
}

void CState_Robella_2Phase::Tick(_float fTimeDelta)
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

void CState_Robella_2Phase::Exit(void* pArg)
{
}

_bool CState_Robella_2Phase::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	// ower�� ä���� ���� ������ ��

	return true;
}

CState_Robella_2Phase* CState_Robella_2Phase::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_2Phase* pInstance = new CState_Robella_2Phase(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_2Phase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_2Phase::Clone(void* pArg)
{
	CState_Robella_2Phase* pInstance = new CState_Robella_2Phase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_2Phase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_2Phase::Free()
{
	__super::Free();
}
