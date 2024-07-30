#include "stdafx.h"
#include "..\Public\State_Player_Sword_Attack_Jump_Charge.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"

CState_Player_Sword_Attack_Jump_Charge::CState_Player_Sword_Attack_Jump_Charge(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Sword_Attack_Jump_Charge::CState_Player_Sword_Attack_Jump_Charge(const CState_Player_Sword_Attack_Jump_Charge& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Sword_Attack_Jump_Charge::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SWORD_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);


	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);

	return S_OK;
}

HRESULT CState_Player_Sword_Attack_Jump_Charge::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Sword_Attack_Jump_Charge::Enter(void* pArg)
{
	USEGAMEINSTANCE;
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);
	m_fAnimSpeed = 1.5f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Sword_Attack_Jump_Charge::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}


	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Sword_Attack_Jump_Charge::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Sword_Attack_Jump_Charge::Check_Condition(CState_Handler* pHandler)
{
	if(0.5f < pHandler->Get_CurState()->Get_KeyPressTimeAcc())
		return true;

	return false;
}


CState_Player_Sword_Attack_Jump_Charge* CState_Player_Sword_Attack_Jump_Charge::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Sword_Attack_Jump_Charge* pInstance = new CState_Player_Sword_Attack_Jump_Charge(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Sword_Attack_Jump_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Sword_Attack_Jump_Charge::Clone(void* pArg)
{
	CState_Player_Sword_Attack_Jump_Charge* pInstance = new CState_Player_Sword_Attack_Jump_Charge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Sword_Attack_Jump_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Sword_Attack_Jump_Charge::Free()
{
}
