#include "stdafx.h"
#include "..\Public\State_Player_Glide_End.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"
#include "Effect_Base.h"

CState_Player_Glide_End::CState_Player_Glide_End(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Glide_End::CState_Player_Glide_End(const CState_Player_Glide_End& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Glide_End::Initialize_Prototype()
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
	m_ReserveStates.push_back(STATE_PLAYER_RUN);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_HAND_IDLE);						//153

	return S_OK;
}

HRESULT CState_Player_Glide_End::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Glide_End::Enter(void* pArg)
{

	MGR(CSound_Manager)->PlaySound(TEXT("Player_GliderOff.mp3"), CSound_Manager::CHANNEL_PLAYER, 1.f);

	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);

	m_pPlayer->Get_Mobility_Glider()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_Glider()->Set_IsGlow(true);

	m_listEffect.clear();
	EFFECT_EXCUTE("Glider_M3",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f)) * XMMatrixTranslation(0.38f, 0.f, -0.1f) * XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-60.f)),
		4.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL,
		"Bone_wp", &m_listEffect);
	EFFECT_EXCUTE("Glider_M3",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f)) * XMMatrixTranslation(-0.38f, 0.f, -0.1f) * XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-60.f)),
		4.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL,
		"Bone_wp", &m_listEffect);


}

void CState_Player_Glide_End::Tick(_float fTimeDelta)
{
	//IMPL_ALARM(m_fSound[4],MGR(CSound_Manager)->PlaySurroundSound(TEXT("Glider_Base.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f), 1.2f, fTimeDelta);

	Set_PlayerLook(fTimeDelta);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	m_pState_Handler->Set_JumpState(false);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Glide_End::Exit(void* pArg)
{
	for (auto& iter : m_listEffect)
		iter->Set_EffectOff();

	m_fKeyPressTimeAcc = 0.f;
	// 만약 Female일 경우
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_Player_Glide_End::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	CTransform* pPlayerTransform = (CTransform*)pHandler->Get_Player()->Get_Component(TRANSFORM);
	CNavigation* pPlayerNavi = (CNavigation*)pHandler->Get_Player()->Get_Component(NAVIGATION);
	_float			fHightDiff = 0.f;
	pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

	if (fHightDiff < 0.2f)
		return true;

	return false;
}


CState_Player_Glide_End* CState_Player_Glide_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Glide_End* pInstance = new CState_Player_Glide_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Glide_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Glide_End::Clone(void* pArg)
{
	CState_Player_Glide_End* pInstance = new CState_Player_Glide_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Glide_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Glide_End::Free()
{
}
