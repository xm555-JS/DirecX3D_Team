#include "stdafx.h"
#include "..\Public\State_Player_Glide_Stop.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"
#include "Effect_Base.h"

CState_Player_Glide_Stop::CState_Player_Glide_Stop(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Glide_Stop::CState_Player_Glide_Stop(const CState_Player_Glide_Stop& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Glide_Stop::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_JUMP_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_GLIDE_LOOP);			//STATE_PLAYER_GLIDE_LOOP_F,						//151
	m_ReserveStates.push_back(STATE_PLAYER_GLIDE_END);				//STATE_PLAYER_GLIDE_END,							//153

	return S_OK;
}

HRESULT CState_Player_Glide_Stop::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Glide_Stop::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);

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

void CState_Player_Glide_Stop::Tick(_float fTimeDelta)
{

	Set_PlayerLook(fTimeDelta);

	if (Get_IsFinished())
		m_bEndCurState = true;

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Glide_Stop::Exit(void* pArg)
{
	for (auto& iter : m_listEffect)
		iter->Set_EffectOff();

	m_fKeyPressTimeAcc = 0.f;
	// 만약 Female일 경우
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_Player_Glide_Stop::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_NONE('W') &&
		KEY_NONE('A') &&
		KEY_NONE('S') &&
		KEY_NONE('D'))
	{
		return true;
	}

	return false;
}


CState_Player_Glide_Stop* CState_Player_Glide_Stop::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Glide_Stop* pInstance = new CState_Player_Glide_Stop(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Glide_Stop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Glide_Stop::Clone(void* pArg)
{
	CState_Player_Glide_Stop* pInstance = new CState_Player_Glide_Stop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Glide_Stop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Glide_Stop::Free()
{
}
