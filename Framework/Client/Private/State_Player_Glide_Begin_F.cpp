#include "stdafx.h"
#include "..\Public\State_Player_Glide_Begin_F.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"
#include "Effect_Base.h"

CState_Player_Glide_Begin_F::CState_Player_Glide_Begin_F(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Glide_Begin_F::CState_Player_Glide_Begin_F(const CState_Player_Glide_Begin_F& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Glide_Begin_F::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_JUMP_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_FALL);

	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_GLIDE_LOOP_F);			//STATE_PLAYER_GLIDE_LOOP_F,						//151
	m_ReserveStates.push_back(STATE_PLAYER_GLIDE_LOOP);				//STATE_PLAYER_GLIDE_STOP,						//152
	m_ReserveStates.push_back(STATE_PLAYER_GLIDE_END);				//STATE_PLAYER_GLIDE_END,							//153

	return S_OK;
}

HRESULT CState_Player_Glide_Begin_F::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Glide_Begin_F::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);
	m_pState_Handler->Get_Player()->Get_Transform()->Glide(0.f, pPlayerNavi);
	m_pState_Handler->Get_Player()->Get_Transform()->Set_YFixState(true);

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

void CState_Player_Glide_Begin_F::Tick(_float fTimeDelta)
{
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Glider_Base.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
	IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Glider_Base.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f), 0.8f, fTimeDelta);

	Set_PlayerLook(fTimeDelta);

	if(Get_IsFinished())
		m_bEndCurState = true;

	if (25 < Get_CurKeyFrame())
	{
		_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
		_float	fRunSpeed = fPlayerSpeed * 8.f;
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		//CTransform* pCurCameraTransform = pGameInstance->Get_CurrentCamera()->Get_Transform();
		CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();

		//	//EasingTurn(pPlayerTransform, vTargetLook);

		pPlayerTransform->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);

	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Glide_Begin_F::Exit(void* pArg)
{
	for (auto& iter : m_listEffect)
		iter->Set_EffectOff();

	m_fKeyPressTimeAcc = 0.f;
	m_pTargetModel->Set_ModelEasingRatio(0.1f);
	m_pPlayer->Get_Transform()->Set_YFixState(false);
	__super::Exit();
}

_bool CState_Player_Glide_Begin_F::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if ((KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D')) &&
		((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput())
	{
		return true;
	}

	return false;
}


CState_Player_Glide_Begin_F* CState_Player_Glide_Begin_F::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Glide_Begin_F* pInstance = new CState_Player_Glide_Begin_F(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Glide_Begin_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Glide_Begin_F::Clone(void* pArg)
{
	CState_Player_Glide_Begin_F* pInstance = new CState_Player_Glide_Begin_F(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Glide_Begin_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Glide_Begin_F::Free()
{
}
