#include "stdafx.h"
#include "..\Public\State_Player_Glide_Up_End.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"
#include "Effect_Base.h"

CState_Player_Glide_Up_End::CState_Player_Glide_Up_End(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Glide_Up_End::CState_Player_Glide_Up_End(const CState_Player_Glide_Up_End& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Glide_Up_End::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_JUMP_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_GLIDE_BEGIN_F);			//STATE_PLAYER_GLIDE_BEGIN_F,						//151
	m_ReserveStates.push_back(STATE_PLAYER_GLIDE_LOOP);				//STATE_PLAYER_GLIDE_BEGIN_F,						//151

	return S_OK;
}

HRESULT CState_Player_Glide_Up_End::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Glide_Up_End::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	m_pPlayer->Get_Mobility_Glider()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsUsing(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsGlow(true);

	m_pPlayer->Get_Transform()->Set_YFixState(true);
	m_pPlayer->Set_Target(nullptr);

	m_listEffect.clear();
	EFFECT_EXCUTE("Glider_M1",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f)) * XMMatrixTranslation(0.38f, 0.f, -0.1f) * XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-60.f)),
		1.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL,
		"Bone_wp", &m_listEffect);
	EFFECT_EXCUTE("Glider_M1",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f)) * XMMatrixTranslation(-0.38f, 0.f, -0.1f) * XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-60.f)),
		1.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL,
		"Bone_wp", &m_listEffect);
}

void CState_Player_Glide_Up_End::Tick(_float fTimeDelta)
{
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Glider_Base.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
	IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Glider_Base.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f), 0.8f, fTimeDelta);

	Set_PlayerLook(fTimeDelta);

	if (Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Glide_Up_End::Exit(void* pArg)
{
	for (auto& iter : m_listEffect)
		iter->Set_EffectOff();

	m_fKeyPressTimeAcc = 0.f;
	m_pPlayer->Get_Transform()->Set_YFixState(false);
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pPlayer->Get_Transform()->Glide(0.f, pPlayerNavi);

	__super::Exit();
}

_bool CState_Player_Glide_Up_End::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}


CState_Player_Glide_Up_End* CState_Player_Glide_Up_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Glide_Up_End* pInstance = new CState_Player_Glide_Up_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Glide_Up_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Glide_Up_End::Clone(void* pArg)
{
	CState_Player_Glide_Up_End* pInstance = new CState_Player_Glide_Up_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Glide_Up_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Glide_Up_End::Free()
{
}
