#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Attack_Jump_Charge_A.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"


CState_Player_2Gun_Attack_Jump_Charge_A::CState_Player_2Gun_Attack_Jump_Charge_A(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Attack_Jump_Charge_A::CState_Player_2Gun_Attack_Jump_Charge_A(const CState_Player_2Gun_Attack_Jump_Charge_A& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Attack_Jump_Charge_A::Initialize_Prototype()
{
	//m_ForceStates.push_back(STATE_PLAYER_2GUN_SKILL_NORMAL);
	//m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_B);
	//m_ReserveStates.push_back(STATE_PLAYER_WALK);
	//m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_2Gun_Attack_Jump_Charge_A::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Attack_Jump_Charge_A::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	m_iCurKeyFrame = 0;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_2Gun_Attack_Jump_Charge_A::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	/*if(nullptr != m_pColCom)
		m_pColCom->Set_ColliderState(true);*/

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	//if (KEY_PRESSING('W') ||
	//	KEY_PRESSING('S') ||
	//	KEY_PRESSING('A') ||
	//	KEY_PRESSING('D'))
	//{
	//	m_pState_Handler->Set_KeyInput(true);
	//}

	//if (KEY_DOWN(VK_LBUTTON))
	//{
	//	m_pState_Handler->Set_KeyInput(true);
	//	m_pState_Handler->Add_KeyInput(VK_LBUTTON);
	//}


	/*if (m_pState_Handler->Get_KeyInput() &&
		25 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}*/

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;


	CState_Player::Tick(fTimeDelta);
}

void CState_Player_2Gun_Attack_Jump_Charge_A::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	pPlayerTransform->Set_Gravity(90.f);

	__super::Exit();
}

_bool CState_Player_2Gun_Attack_Jump_Charge_A::Check_Condition(CState_Handler* pHandler)
{
	//for (auto& iKey : *pHandler->Get_KeyInputList())
	//{
	//	if (VK_LBUTTON == iKey && 0.2f < m_fKeyPressTimeAcc)
	//		return true;
	//}
	if (0.3f < pHandler->Get_CurState()->Get_KeyPressTimeAcc())
		return true;

	return false;
}


CState_Player_2Gun_Attack_Jump_Charge_A* CState_Player_2Gun_Attack_Jump_Charge_A::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Attack_Jump_Charge_A* pInstance = new CState_Player_2Gun_Attack_Jump_Charge_A(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Attack_Jump_Charge_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Attack_Jump_Charge_A::Clone(void* pArg)
{
	CState_Player_2Gun_Attack_Jump_Charge_A* pInstance = new CState_Player_2Gun_Attack_Jump_Charge_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Attack_Jump_Charge_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Attack_Jump_Charge_A::Free()
{
}
