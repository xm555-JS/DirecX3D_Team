#include "stdafx.h"
#include "..\Public\State_Player_Swim_Idle.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Weapon.h"
#include "Player.h"
#include "Effect_Manager.h"

///collider
#include "Collider.h"

#include "Imgui_Manager.h"

CState_Player_Swim_Idle::CState_Player_Swim_Idle(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Swim_Idle::CState_Player_Swim_Idle(const CState_Player_Swim_Idle& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Swim_Idle::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_DASH);
	m_ForceStates.push_back(STATE_PLAYER_SKATE_CALL);

	m_ForceStates.push_back(STATE_PLAYER_RUN);
	m_ForceStates.push_back(STATE_PLAYER_WALK);
	m_ForceStates.push_back(STATE_PLAYER_HAND_IDLE);

	return S_OK;
}

HRESULT CState_Player_Swim_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Swim_Idle::Enter(void* pArg)
{

	if (STATE_PLAYER_GLIDE_LOOP == m_pState_Handler->Get_PreStateType())
	{
		MGR(CSound_Manager)->PlaySound(TEXT("Player_GliderOff.mp3"), CSound_Manager::CHANNEL_PLAYER, 1.f);
	}
	else if (STATE_PLAYER_MOUNT_UP == m_pState_Handler->Get_PreStateType() ||
		STATE_PLAYER_MOUNT_IDLE == m_pState_Handler->Get_PreStateType() ||
		STATE_PLAYER_MOUNT_DRIVE == m_pState_Handler->Get_PreStateType() ||
		STATE_PLAYER_MOUNT_DRIVE_AIR == m_pState_Handler->Get_PreStateType())
	{
		MGR(CSound_Manager)->PlaySound(TEXT("Player_BikeOff.mp3"), CSound_Manager::CHANNEL_PLAYER);
	}

	__super::Enter(pArg);
	//m_pTargetModel->Set_AnimationGroup(ANIM_PLAYER_FEMALE, WEAPON_SWORD);
	m_bEndCurState = true;
	m_fAnimSpeed = 1.f;
	//static_cast<CPlayer*>(m_pState_Handler->Get_Player())->Set_SwimHeight(DEBUGFLOAT4(0).x);
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pState_Handler->Get_Player()->Get_Transform()->Swim(static_cast<CPlayer*>(m_pState_Handler->Get_Player())->Get_SwimHeight(), pPlayerNavi);

	m_pPlayer->Set_Target(nullptr);

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

	m_pPlayer->Get_Mobility_Glider()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_Glider()->Set_IsGlow(true);

	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsGlow(true);


}

void CState_Player_Swim_Idle::Tick(_float fTimeDelta)
{
	
	if (STATE_PLAYER_JUMP_FALL == m_pState_Handler->Get_PreStateType()
		|| STATE_PLAYER_JUMP_END == m_pState_Handler->Get_PreStateType())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Swim_Diving.mp3"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 1.f));
	}
	Set_PlayerLook(fTimeDelta);

	////////////test///////////////////////////////
	USEGAMEINSTANCE;
	if (KEY_PRESSING(VK_LBUTTON))
	{
		m_pState_Handler->Add_KeyPressingTime(VK_LBUTTON, fTimeDelta);
	}

	if (KEY_DOWN(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_LBUTTON);
	}

	m_fAccTime += fTimeDelta;

	if (0.8f < m_fAccTime)
	{
		m_fAccTime = 0.f;

		EFFECT_POS("Player_Swim_Idle", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 1.185f, 0.f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f,
			XMVectorSet(0.5f, 0.5f, 0.5f, 0.f));
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Swim_Idle::Exit(void* pArg)
{
	__super::Exit();

	m_fAccTime = 0.f;
}

_bool CState_Player_Swim_Idle::Check_Condition(CState_Handler* pHandler)
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
			return true;
	}

	return false;
}


CState_Player_Swim_Idle* CState_Player_Swim_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Swim_Idle* pInstance = new CState_Player_Swim_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Swim_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Swim_Idle::Clone(void* pArg)
{
	CState_Player_Swim_Idle* pInstance = new CState_Player_Swim_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Swim_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Swim_Idle::Free()
{
}
