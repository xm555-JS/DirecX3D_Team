#include "stdafx.h"
#include "..\Public\State_Player_Hand_Idle.h"
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

CState_Player_Hand_Idle::CState_Player_Hand_Idle(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Hand_Idle::CState_Player_Hand_Idle(const CState_Player_Hand_Idle& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Hand_Idle::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_UI_APPEAR);
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

	m_ReserveStates.push_back(STATE_PLAYER_EAT);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_A);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);

	return S_OK;
}

HRESULT CState_Player_Hand_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Hand_Idle::Enter(void* pArg)
{
	if (STATE_PLAYER_MOUNT_UP == m_pState_Handler->Get_PreStateType() ||
		STATE_PLAYER_MOUNT_IDLE == m_pState_Handler->Get_PreStateType() ||
		STATE_PLAYER_MOUNT_DRIVE == m_pState_Handler->Get_PreStateType() ||
		STATE_PLAYER_MOUNT_DRIVE_AIR == m_pState_Handler->Get_PreStateType())
	{
		MGR(CSound_Manager)->PlaySound(TEXT("Player_BikeOff.mp3"), CSound_Manager::CHANNEL_PLAYER);
	}
	__super::Enter(pArg);
	//m_pTargetModel->Set_AnimationGroup(ANIM_PLAYER_FEMALE, WEAPON_SWORD);
	m_bEndCurState = true;
	m_fAnimSpeed = 2.f;
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL())
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsDissolve(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsUsing(false);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsGlow(true);
	}

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR())
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsDissolve(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsUsing(false);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsGlow(true);
	}

	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsGlow(true);
}

void CState_Player_Hand_Idle::Tick(_float fTimeDelta)
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

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Hand_Idle::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	__super::Exit();
}

_bool CState_Player_Hand_Idle::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	if (STATE_PLAYER_MOUNT_UP == pHandler->Get_CurStateType() ||
		STATE_PLAYER_MOUNT_IDLE == pHandler->Get_CurStateType() ||
		STATE_PLAYER_MOUNT_DRIVE == pHandler->Get_CurStateType() ||
		STATE_PLAYER_MOUNT_DRIVE_AIR == pHandler->Get_CurStateType())
	{
		if (KEY_DOWN('V'))
		{
			return true;
		}
	}
	else
	{
		if (KEY_NONE('W') &&
			KEY_NONE('A') &&
			KEY_NONE('S') &&
			KEY_NONE('D'))
		{
			_float4 vPos;
			XMStoreFloat4(&vPos, pHandler->Get_Player()->Get_Pos());
			if (vPos.y < static_cast<CPlayer*>(pHandler->Get_Player())->Get_SwimHeight() + 0.1f)
				return false;

			if (((CPlayer*)pHandler->Get_Player())->Get_InEquip_Window())
			{
				return false;
			}

			return true;
		}
	}

	return false;
}


CState_Player_Hand_Idle* CState_Player_Hand_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Hand_Idle* pInstance = new CState_Player_Hand_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Hand_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Hand_Idle::Clone(void* pArg)
{
	CState_Player_Hand_Idle* pInstance = new CState_Player_Hand_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Hand_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Hand_Idle::Free()
{
}
