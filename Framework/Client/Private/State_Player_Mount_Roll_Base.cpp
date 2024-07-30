#include "stdafx.h"
#include "..\Public\State_Player_Mount_Roll_Base.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"
#include "Camera_Target_Position.h"

#include "Imgui_Manager.h"
#include "Effect_Manager.h"

CState_Player_Mount_Roll_Base::CState_Player_Mount_Roll_Base(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Mount_Roll_Base::CState_Player_Mount_Roll_Base(const CState_Player_Mount_Roll_Base& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Mount_Roll_Base::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_MOUNT_JUMP_START);					
	m_ForceStates.push_back(STATE_PLAYER_MOUNT_TURN_L);						
	m_ForceStates.push_back(STATE_PLAYER_MOUNT_TURN_R);						
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_BASE_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_GLIDE_UP);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);
	m_ForceStates.push_back(STATE_PLAYER_HAND_IDLE);

	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_DRIVE);
	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_IDLE);						

	return S_OK;
}

HRESULT CState_Player_Mount_Roll_Base::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Mount_Roll_Base::Enter(void* pArg)
{
	MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_BikeBegin.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.25f);
	IMPL_ONCE(m_bSound[0], m_eSoundChannel[2] = MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_BikeIng.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.25f));
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	m_fAccTime = 0.f;
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	CPlayer* pPlayer = (CPlayer*)m_pState_Handler->Get_Player();
	_float fDashGauge = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitInfo())->fDashGauge;
	((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fDashGauge -= fDashGauge / 3;
	m_pPlayer->Set_Target(nullptr);

	EFFECT_EXCUTE("MotorCycle_Dash_M",
		XMMatrixTranslation(0.165f, 0.2f, -1.04f),
		1.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL);
	EFFECT_EXCUTE("MotorCycle_Dash_M",
		XMMatrixTranslation(-0.165f, 0.2f, -1.04f),
		1.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL);

	USEGAMEINSTANCE;
	pGameInstance->Stop_RadialBlur();
	pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 0.25f, 0.25f, 1.f, 0.03f);
}

void CState_Player_Mount_Roll_Base::Tick(_float fTimeDelta)
{
	IMPL_ALARM(m_fSound[0], m_eSoundChannel[1] = MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_BikeIng.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.25f), 28.f, fTimeDelta);
	m_fTimeAcc_Effect0 += fTimeDelta;
	if (0.02f < m_fTimeAcc_Effect0)
	{
		m_fTimeAcc_Effect0 = 0.f;
		EFFECT_POS("MotorCycle_P", m_pState_Handler->Get_Player()->Get_WorldMatrix());
	}

	m_fAccTime += fTimeDelta;

	_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
	_float	fRunSpeed = fPlayerSpeed * 30.f;
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();

	pPlayerTransform->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);

	if (0.2f < m_fAccTime)
		m_bEndCurState = true;

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Mount_Roll_Base::Exit(void* pArg)
{
	MGR(CSound_Manager)->StopSound(m_eSoundChannel[2]);
	MGR(CSound_Manager)->StopSound(m_eSoundChannel[1]);
	m_fKeyPressTimeAcc = 0.f;	
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	__super::Exit();
}

_bool CState_Player_Mount_Roll_Base::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if ((KEY_DOWN(VK_SHIFT) || KEY_DOWN(VK_RBUTTON))
		&& Enable_Use_Dash(pHandler))
	{
		return true;
	}

	return false;
}

CState_Player_Mount_Roll_Base* CState_Player_Mount_Roll_Base::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Mount_Roll_Base* pInstance = new CState_Player_Mount_Roll_Base(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Mount_Roll_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Mount_Roll_Base::Clone(void* pArg)
{
	CState_Player_Mount_Roll_Base* pInstance = new CState_Player_Mount_Roll_Base(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Mount_Roll_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Mount_Roll_Base::Free()
{
}
