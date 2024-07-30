#include "stdafx.h"
#include "..\Public\State_Player_Skate_Stop_Land.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"
#include "Effect_Base.h"

CState_Player_Skate_Stop_Land::CState_Player_Skate_Stop_Land(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player_Skate(pManager, eState)
{
}

CState_Player_Skate_Stop_Land::CState_Player_Skate_Stop_Land(const CState_Player_Skate_Stop_Land& rhs)
	: CState_Player_Skate(rhs)
{
}

HRESULT CState_Player_Skate_Stop_Land::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_HAND_IDLE);

	m_ReserveStates.push_back(STATE_PLAYER_RUN);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_HAND_IDLE);

	return S_OK;
}

HRESULT CState_Player_Skate_Stop_Land::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Skate_Stop_Land::Enter(void* pArg)
{
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("State_Stop.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

	//198
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = true;
	m_fAnimSpeed = 2.f;
	m_pPlayer->Get_Transform()->Set_Skate(false);
	m_pPlayer->Get_Transform()->Reset_FrontAccTime();

	m_listEffects = *((CState_Player_Skate*)m_pState_Handler->Get_State(STATE_PLAYER_SKATE_CALL))->Get_Effects();

	for (auto& iter : m_listEffects)
		iter->Set_EffectOff();
}

void CState_Player_Skate_Stop_Land::Tick(_float fTimeDelta)
{
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Jump_End.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 1.f));

	Set_PlayerLook(fTimeDelta);

	if (Get_IsFinished())
		m_bEndCurState = true;

	CState_Player_Skate::Tick(fTimeDelta);
}

void CState_Player_Skate_Stop_Land::Exit(void* pArg)
{

	m_fKeyPressTimeAcc = 0.f;
	m_pPlayer->Get_Transform()->Reset_FrontAccTime();
	__super::Exit();
}

_bool CState_Player_Skate_Stop_Land::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	_float4 vPos;
	XMStoreFloat4(&vPos, pHandler->Get_Player()->Get_Pos());

	_float fSpeed = ((CState_Player_Skate*)pHandler->Get_CurState())->Get_RunSpeed();

	CTransform* pPlayerTransform = (CTransform*)pHandler->Get_Player()->Get_Component(TRANSFORM);
	CNavigation* pPlayerNavi = (CNavigation*)pHandler->Get_Player()->Get_Component(NAVIGATION);
	_float			fHightDiff = 0.f;
	pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

	if ((KEY_DOWN('3') || 0 >= fSpeed) && fHightDiff < 1.f)
	{
		return true;
	}

	return false;
}


CState_Player_Skate_Stop_Land* CState_Player_Skate_Stop_Land::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Skate_Stop_Land* pInstance = new CState_Player_Skate_Stop_Land(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Skate_Stop_Land");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Skate_Stop_Land::Clone(void* pArg)
{
	CState_Player_Skate_Stop_Land* pInstance = new CState_Player_Skate_Stop_Land(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Skate_Stop_Land");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Skate_Stop_Land::Free()
{
}
