#include "stdafx.h"
#include "..\Public\State_Player_Mount_JumpStart.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"
#include "Effect_Base.h"

CState_Player_Mount_JumpStart::CState_Player_Mount_JumpStart(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Mount_JumpStart::CState_Player_Mount_JumpStart(const CState_Player_Mount_JumpStart& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Mount_JumpStart::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_BASE_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_GLIDE_UP);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_DRIVE_AIR);						//153
	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_DRIVE);						//153
	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_IDLE);						//153
	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_ROLL_AIR);

	return S_OK;
}

HRESULT CState_Player_Mount_JumpStart::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Mount_JumpStart::Enter(void* pArg)
{
	MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_BikeBegin.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.25f);
	IMPL_ONCE(m_bSound[0], m_eSoundChannel[2]= MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_BikeIng.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.25f));

	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 5.f;
	m_pState_Handler->Set_JumpState(true);

	m_pPlayer->Set_Target(nullptr);
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pPlayer->Get_Transform()->Jump(7.f, pPlayerNavi);


	m_EffectList.clear();
	EFFECT_EXCUTE("MotorCycle_M",
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(-30.f), 0.f, 0.f) * XMMatrixTranslation(0.165f, -0.3f, -1.4f),
		1.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL,
		"", &m_EffectList);
	EFFECT_EXCUTE("MotorCycle_M",
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(-30.f), 0.f, 0.f) * XMMatrixTranslation(-0.165f, -0.3f, -1.4f),
		1.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL, "", &m_EffectList);
}

void CState_Player_Mount_JumpStart::Tick(_float fTimeDelta)
{
	IMPL_ALARM(m_fSound[0], m_eSoundChannel[1]= MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_BikeIng.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.25f), 28.f, fTimeDelta);
	m_fTimeAcc_Effect0 += fTimeDelta;
	if (0.02f < m_fTimeAcc_Effect0)
	{
		m_fTimeAcc_Effect0 = 0.f;
		EFFECT_POS("MotorCycle_P", m_pState_Handler->Get_Player()->Get_WorldMatrix());
	}

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
		//CTransform* pCurCameraTransform = pGameInstance->Get_CurrentCamera()->Get_Transform();
		CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();

		//	//EasingTurn(pPlayerTransform, vTargetLook);

		pPlayerTransform->Go_AccFront_Limit(fTimeDelta, fRunSpeed, 16.f, pPlayerNavi);
	}

	if(Get_IsFinished())
		m_bEndCurState = true;

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Mount_JumpStart::Exit(void* pArg)
{
	MGR(CSound_Manager)->StopSound(m_eSoundChannel[2]);
	MGR(CSound_Manager)->StopSound(m_eSoundChannel[1]);
	m_fKeyPressTimeAcc = 0.f;

	for (auto& iter : m_EffectList)
		iter->Set_EffectOff();
	__super::Exit();
}

_bool CState_Player_Mount_JumpStart::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN(VK_SPACE) &&
		false == pHandler->Get_JumpState())
	{
		return true;
	}

	return false;
}


CState_Player_Mount_JumpStart* CState_Player_Mount_JumpStart::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Mount_JumpStart* pInstance = new CState_Player_Mount_JumpStart(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Mount_JumpStart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Mount_JumpStart::Clone(void* pArg)
{
	CState_Player_Mount_JumpStart* pInstance = new CState_Player_Mount_JumpStart(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Mount_JumpStart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Mount_JumpStart::Free()
{
}
