#include "stdafx.h"
#include "..\Public\State_Player_Attack_A.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"

CState_Player_Attack_A::CState_Player_Attack_A(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Attack_A::CState_Player_Attack_A(const CState_Player_Attack_A& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Attack_A::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);

	m_ReserveStates.push_back(STATE_PLAYER_ATTACK_B);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Attack_A::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Attack_A::Enter(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	m_iCurKeyFrame = 0;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
}

void CState_Player_Attack_A::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if(nullptr != m_pColCom)
		m_pColCom->Set_ColliderState(true);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (KEY_DOWN(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
	}


	if (m_pState_Handler->Get_KeyInput() &&
		15 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;


	_uint iCurKeyFrame = m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame();
	if (m_iCurKeyFrame != iCurKeyFrame)
	{
		m_iCurKeyFrame = iCurKeyFrame;
		if (6 == m_iCurKeyFrame)
		{
			_matrix matPosition = m_pTargetModel->Find_HierarcyNode("Root_Bone")->Get_CombinedMatrix() * XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());

			matPosition.r[0] = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()).r[0];
			matPosition.r[1] = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()).r[1];
			matPosition.r[2] = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()).r[2];
			USE_EFFECT("Attack_A_M", matPosition);
		}
	}


	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Attack_A::Exit(void* pArg)
{
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	m_pColCom->Set_ColliderState(false);
	__super::Exit();
}

_bool CState_Player_Attack_A::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN(VK_LBUTTON))
	{
		return true;
	}

	return false;
}


CState_Player_Attack_A* CState_Player_Attack_A::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Attack_A* pInstance = new CState_Player_Attack_A(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Attack_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Attack_A::Clone(void* pArg)
{
	CState_Player_Attack_A* pInstance = new CState_Player_Attack_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Attack_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Attack_A::Free()
{
}
