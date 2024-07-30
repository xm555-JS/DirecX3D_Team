#include "stdafx.h"
#include "..\Public\State_Player_Glide_Up.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"
#include "Effect_Base.h"

CState_Player_Glide_Up::CState_Player_Glide_Up(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Glide_Up::CState_Player_Glide_Up(const CState_Player_Glide_Up& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Glide_Up::Initialize_Prototype()
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
	m_ReserveStates.push_back(STATE_PLAYER_GLIDE_UP_END);				//STATE_PLAYER_GLIDE_STOP,						//152

	return S_OK;
}

HRESULT CState_Player_Glide_Up::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Glide_Up::Enter(void* pArg)
{
	
	MGR(CSound_Manager)->PlaySound(TEXT("Player_GliderOn.mp3"), CSound_Manager::CHANNEL_PLAYER);

	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 1.5f;
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

	m_pPlayer->Get_Mobility_Glider()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsUsing(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsGlow(true);

	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsGlow(true);

	m_listEffect.clear();
	EFFECT_EXCUTE("Glider_M2",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f)) * XMMatrixTranslation(0.38f, 0.f, -0.1f) * XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-60.f)),
		1.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL,
		"Bone_wp", &m_listEffect);
	EFFECT_EXCUTE("Glider_M2",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f)) * XMMatrixTranslation(-0.38f, 0.f, -0.1f) * XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-60.f)),
		1.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL,
		"Bone_wp", &m_listEffect);
}

void CState_Player_Glide_Up::Tick(_float fTimeDelta)
{
	IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Glider_Base.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f), 0.8f, fTimeDelta);

	USEGAMEINSTANCE;

	m_fTimeAcc_Effect0 += fTimeDelta;
	if (0.02f < m_fTimeAcc_Effect0)
	{
		m_fTimeAcc_Effect0 = 0.f;
		EFFECT_POS("Dash_P", m_pState_Handler->Get_Player()->Get_WorldMatrix());
	}

	if (58 < Get_CurKeyFrame())
		m_bEndCurState = true;

	if (23 < Get_CurKeyFrame() && !m_bJump)
	{
		m_fAnimSpeed = 0.5f;//0.5
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		m_pState_Handler->Get_Player()->Get_Transform()->Jump(20.f, pPlayerNavi, 7.f);//20 7
		m_bJump = true;
	}

	Set_PlayerLook(fTimeDelta);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Glide_Up::Exit(void* pArg)
{
	for (auto& iter : m_listEffect)
		iter->Set_EffectOff();

	m_fAnimSpeed = 1.f;
	m_pTargetModel->Set_ModelEasingRatio(0.1f);
	m_bJump = false;
	m_pPlayer->Get_Transform()->Set_Gravity(15.f);
	__super::Exit();
}

_bool CState_Player_Glide_Up::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	//키 조정해야함
	if (KEY_DOWN('2'))
	{
		return true;
	}

	return false;
}


CState_Player_Glide_Up* CState_Player_Glide_Up::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Glide_Up* pInstance = new CState_Player_Glide_Up(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Glide_Up");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Glide_Up::Clone(void* pArg)
{
	CState_Player_Glide_Up* pInstance = new CState_Player_Glide_Up(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Glide_Up");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Glide_Up::Free()
{
}
