#include "stdafx.h"
#include "..\Public\State_Player_Damaged_Medium.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Channel.h"

#include "Effect_Manager.h"

///collider
#include "Collider.h"

//#include "Imgui_Manager.h"

CState_Player_Damaged_Medium::CState_Player_Damaged_Medium(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Damaged_Medium::CState_Player_Damaged_Medium(const CState_Player_Damaged_Medium& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Damaged_Medium::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_HAND_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_A);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);

	return S_OK;
}

HRESULT CState_Player_Damaged_Medium::Initialize(void* pArg)
{
	USEGAMEINSTANCE;

	return S_OK;
}

void CState_Player_Damaged_Medium::Enter(void* pArg)
{
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Damaged_Medium.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

	__super::Enter(pArg);
	//m_pTargetModel->Set_AnimationGroup(ANIM_PLAYER_FEMALE, WEAPON_SWORD);
	m_bEndCurState = false;
	m_fAnimSpeed = 2.f;
}

void CState_Player_Damaged_Medium::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 2.f, 0.f, 0.f), 0.033f, 0.3f, 0.25f));

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING('W') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('D'))
	{
		m_pState_Handler->Set_KeyInput(true);
	}

	CPlayer* pPlayer = (CPlayer*)m_pState_Handler->Get_Player();
	_float fCurDashGauge = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fDashGauge;
	_float fDashGauge = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitInfo())->fDashGauge;

	if ((KEY_DOWN(VK_SHIFT) || KEY_DOWN(VK_RBUTTON))
		&& fCurDashGauge > fDashGauge / 3)
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_RBUTTON);
	}

	if (m_pState_Handler->Get_KeyInput() &&
		5 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
	_float	fRunSpeed = fPlayerSpeed * 2.f;
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();

	pPlayerTransform->Go_Back(fTimeDelta, fRunSpeed, pPlayerNavi);

	IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Damaged", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Damaged_Medium::Exit(void* pArg)
{
	__super::Exit();

	((CPlayer*)m_pState_Handler->Get_Player())->Set_Damaged(1, false);
}

_bool CState_Player_Damaged_Medium::Check_Condition(CState_Handler* pHandler)
{
	return	((CPlayer*)pHandler->Get_Player())->Get_Damaged(1);
}


CState_Player_Damaged_Medium* CState_Player_Damaged_Medium::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Damaged_Medium* pInstance = new CState_Player_Damaged_Medium(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Damaged_Medium");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Damaged_Medium::Clone(void* pArg)
{
	CState_Player_Damaged_Medium* pInstance = new CState_Player_Damaged_Medium(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Damaged_Medium");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Damaged_Medium::Free()
{

}
