#include "stdafx.h"
#include "..\Public\State_Player_Mount_Drive_Air.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"
#include "Camera_Target_Position.h"

//#include "Imgui_Manager.h"
#include "Effect_Base.h"

CState_Player_Mount_Drive_Air::CState_Player_Mount_Drive_Air(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Mount_Drive_Air::CState_Player_Mount_Drive_Air(const CState_Player_Mount_Drive_Air& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Mount_Drive_Air::Initialize_Prototype()
{			
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_JUMP_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_GLIDE_UP);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);
	m_ForceStates.push_back(STATE_PLAYER_HAND_IDLE);

	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_DRIVE);
	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_ROLL_AIR);

	return S_OK;
}

HRESULT CState_Player_Mount_Drive_Air::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Mount_Drive_Air::Enter(void* pArg)
{
	m_eSoundChannel[2]=MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_BikeIng.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.25f);
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	m_bEndCurState = true;
	m_pPlayer->Set_Target(nullptr);

	m_EffectList.clear();
	EFFECT_EXCUTE("MotorCycle_M",
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(-30.f), 0.f, 0.f) * XMMatrixTranslation(0.165f, -0.3f, -1.45f),
		1.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL,
		"", &m_EffectList);
	EFFECT_EXCUTE("MotorCycle_M",
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(-30.f), 0.f, 0.f) * XMMatrixTranslation(-0.165f, -0.3f, -1.45f),
		1.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL, "", &m_EffectList);

	GAMEINSTANCE->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 1.f, 1.f, 1.f, 0.018f, false, true);
}

void CState_Player_Mount_Drive_Air::Tick(_float fTimeDelta)
{
	IMPL_ALARM(m_fSound[0],m_eSoundChannel[1]= MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_BikeIng.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.25f), 28.f, fTimeDelta);

	m_fTimeAcc_Effect0 += fTimeDelta;
	if (0.02f < m_fTimeAcc_Effect0)
	{
		m_fTimeAcc_Effect0 = 0.f;
		EFFECT_POS("MotorCycle_P", m_pState_Handler->Get_Player()->Get_WorldMatrix());
	}

	Set_PlayerLook(fTimeDelta);

	CTransform* pPlayerTransform = (CTransform*)m_pPlayer->Get_Transform();
	CNavigation* pPlayerNavi = (CNavigation*)m_pPlayer->Get_Component(ID_UINT(NAVIGATION));

	USEGAMEINSTANCE;
	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D'))
	{
		_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
		_float	fRunSpeed = fPlayerSpeed * 12.f;

		pPlayerTransform->Go_AccFront_Limit(fTimeDelta, fRunSpeed, 16.f, pPlayerNavi);
	}

	_float			fHightDiff = 0.f;
	pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

	if (fHightDiff < 0.5f)
		m_pState_Handler->Set_JumpState(false);

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Mount_Drive_Air::Exit(void* pArg)
{
	MGR(CSound_Manager)->StopSound(m_eSoundChannel[2]);
	MGR(CSound_Manager)->StopSound(m_eSoundChannel[1]);
	m_fKeyPressTimeAcc = 0.f;
	for (auto& iter : m_EffectList)
		iter->Set_EffectOff();

	GAMEINSTANCE->Stop_RadialBlur();
	__super::Exit();
}

_bool CState_Player_Mount_Drive_Air::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	CTransform* pPlayerTransform = (CTransform*)pHandler->Get_Player()->Get_Component(TRANSFORM);
	CNavigation* pPlayerNavi = (CNavigation*)pHandler->Get_Player()->Get_Component(NAVIGATION);
	_float			fHightDiff = 0.f;
	pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

	if (fHightDiff > 0.5f)
	{
		if ((KEY_PRESSING('W') ||
			KEY_PRESSING('A') ||
			KEY_PRESSING('S') ||
			KEY_PRESSING('D')) &&
			((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput())
		{
			return true;
		}
	}

	return false;
}

void CState_Player_Mount_Drive_Air::Set_PlayerLook(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN('W') ||
		KEY_DOWN('A') ||
		KEY_DOWN('S') ||
		KEY_DOWN('D'))
		m_fPlayerLookTimeAcc = 0.f;

	_float	fLerpTime = 0.1f;
	m_fPlayerLookTimeAcc = fTimeDelta;
	CCamera_Target_Position* pCamera = ((CCamera_Target_Position*)pGameInstance->Get_CurrentCamera());
	CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();
	_vector vCurLook = pPlayerTransform->Get_State(CTransform::STATE_LOOK);
	//_vector vCamLook = pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);

	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D') ||
		nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_Target())
	{
		_vector	vLookDir = { 0.f, 0.f, 0.f, 0.f };

		if (KEY_PRESSING('W'))
			vLookDir += pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);

		if (KEY_PRESSING('A'))
			vLookDir += -pCamera->Get_Transform()->Get_State(CTransform::STATE_RIGHT);

		if (KEY_PRESSING('S'))
			vLookDir += -pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);

		if (KEY_PRESSING('D'))
			vLookDir += pCamera->Get_Transform()->Get_State(CTransform::STATE_RIGHT);

		if (0.01f > XMVectorGetX(XMVector3Length(vLookDir)))
			vLookDir = pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);

		m_pState_Handler->Set_PlayerLook(XMVector3Normalize(vLookDir));
	}

	_float	fCurTime = min(1.f, m_fPlayerLookTimeAcc / fLerpTime);

	//vCurLook
	_vector vFinalLook = XMVectorLerp(vCurLook, XMLoadFloat4(&m_pState_Handler->Get_PlayerLook()), fCurTime);

	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));

	_float			fHightDiff = 0.f;
	pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

	if (fHightDiff < 0.2f)
		pPlayerTransform->UpNaviLook(vFinalLook, pPlayerNavi);
	else
		pPlayerTransform->YFixedLook(vFinalLook);
}


CState_Player_Mount_Drive_Air* CState_Player_Mount_Drive_Air::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Mount_Drive_Air* pInstance = new CState_Player_Mount_Drive_Air(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Mount_Drive_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Mount_Drive_Air::Clone(void* pArg)
{
	CState_Player_Mount_Drive_Air* pInstance = new CState_Player_Mount_Drive_Air(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Mount_Drive_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Mount_Drive_Air::Free()
{
}
