#include "stdafx.h"
#include "..\Public\State_Player_Jump_B.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Effect_Manager.h"
#include "Channel.h"
#include "Player.h"

//#include "Imgui_Manager.h"

CState_Player_Jump_B::CState_Player_Jump_B(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Jump_B::CState_Player_Jump_B(const CState_Player_Jump_B& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Jump_B::Initialize_Prototype()
{
	//m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_JUMP_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_GLIDE_LOOP);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);

	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	//m_ReserveStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);

	return S_OK;
}

HRESULT CState_Player_Jump_B::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Jump_B::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_pTargetModel->Set_AnimationGroup(ANIM_PLAYER_FEMALE, WEAPON_HAND4);

	m_pState_Handler->Set_JumpState(true);
	m_bEndCurState = true;
	m_fAnimSpeed = 1.3f;

	USEGAMEINSTANCE;
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pState_Handler->Get_Player()->Get_Transform()->Jump(7.5f, pPlayerNavi);

	m_fKeyPressTimeAcc = 0.f;
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);
}

void CState_Player_Jump_B::Tick(_float fTimeDelta)
{
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Jump_B.mp3"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 1.f));

	Set_PlayerLook(fTimeDelta);

	USEGAMEINSTANCE;
	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D'))
	{
		_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
		_float	fRunSpeed = fPlayerSpeed * 8.f;
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();

		pPlayerTransform->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);
	}

	if (KEY_DOWN(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_LBUTTON);
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	if (3 <= Get_CurKeyFrame())
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Jump", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, -0.7f, 0.f, 1.f)));

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Jump_B::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_Player_Jump_B::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN(VK_SPACE) &&
		false == pHandler->Get_JumpState())
	{
		return true;
	}

	return false;
}


CState_Player_Jump_B* CState_Player_Jump_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Jump_B* pInstance = new CState_Player_Jump_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Jump_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Jump_B::Clone(void* pArg)
{
	CState_Player_Jump_B* pInstance = new CState_Player_Jump_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Jump_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Jump_B::Free()
{
}
