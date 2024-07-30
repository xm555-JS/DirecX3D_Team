#include "stdafx.h"
#include "..\Public\State_Player_Shield_Idle.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Npc_Shirley.h"
#include "Player.h"
#include "Weapon.h"

///collider
#include "Collider.h"

//#include "Imgui_Manager.h"

CState_Player_Shield_Idle::CState_Player_Shield_Idle(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Shield_Idle::CState_Player_Shield_Idle(const CState_Player_Shield_Idle& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Shield_Idle::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_BASE_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_LOW);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_MEDIUM);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_GLIDE_UP);
	m_ForceStates.push_back(STATE_PLAYER_SKATE_CALL);
	m_ForceStates.push_back(STATE_PLAYER_MOUNT_UP);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);
	m_ForceStates.push_back(STATE_PLAYER_EAT);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_WALK);

	m_ReserveStates.push_back(STATE_PLAYER_HAND_IDLE);

	return S_OK;
}

HRESULT CState_Player_Shield_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Shield_Idle::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 2.f;
}

void CState_Player_Shield_Idle::Tick(_float fTimeDelta)
{
	Set_PlayerLook(fTimeDelta);

	//CTransform* pPlayerTrasform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);

	//_matrix	Combined = m_pTargetModel->Find_HierarcyNode("Bip001")->Get_CombinedMatrix();
	//_matrix TransformMat = m_pTargetModel->Find_HierarcyNode("Bip001")->Get_TransformMatrix();
	//_matrix playerMat = pPlayerTrasform->Get_WorldMatrix();


	//_matrix CombinedMatrix = Combined * TransformMat * playerMat;
	//m_bEndCurState = true;
	//CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	//USEGAMEINSTANCE;
	//if(KEY_DOWN(VK_SPACE))
	//	m_pState_Handler->Get_Player()->Get_Transform()->Jump(DEBUGFLOAT, pPlayerNavi);

	////////////test///////////////////////////////
	USEGAMEINSTANCE;
	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	if (KEY_PRESSING('W') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('D'))
	{
		m_pState_Handler->Set_KeyInput(true);
	}

	if (KEY_DOWN(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_LBUTTON);
	}

	if (KEY_DOWN(VK_SPACE))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_SPACE);
	}

	if (m_pState_Handler->Get_KeyInput() || 0.5f < m_pState_Handler->Get_TimeAcc())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Shield_Idle::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	__super::Exit();
}

_bool CState_Player_Shield_Idle::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_NONE('W') &&
		KEY_NONE('A') &&
		KEY_NONE('S') &&
		KEY_NONE('D'))
	{
		_float4 vPos;
		XMStoreFloat4(&vPos, pHandler->Get_Player()->Get_Pos());
		if (vPos.y < static_cast<CPlayer*>(pHandler->Get_Player())->Get_SwimHeight() + 0.1f)
			return false;

		return true;
	}

	return false;
}


CState_Player_Shield_Idle* CState_Player_Shield_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Shield_Idle* pInstance = new CState_Player_Shield_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Shield_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Shield_Idle::Clone(void* pArg)
{
	CState_Player_Shield_Idle* pInstance = new CState_Player_Shield_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Shield_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Shield_Idle::Free()
{
}
