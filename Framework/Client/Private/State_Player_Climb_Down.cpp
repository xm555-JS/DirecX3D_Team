#include "stdafx.h"
#include "..\Public\State_Player_Climb_Down.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_Player_Climb_Down::CState_Player_Climb_Down(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Climb_Down::CState_Player_Climb_Down(const CState_Player_Climb_Down& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Climb_Down::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Player_Climb_Down::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Climb_Down::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = true;
	m_fAnimSpeed = 2.f;
}

void CState_Player_Climb_Down::Tick(_float fTimeDelta)
{
	Set_PlayerLook(fTimeDelta);

	//공중에 떠있음//조금씩 떨어짐

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Climb_Down::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	// 만약 Female일 경우
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_Player_Climb_Down::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN(VK_SPACE) && ((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput())
	{
		return true;
	}

	return false;
}


CState_Player_Climb_Down* CState_Player_Climb_Down::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Climb_Down* pInstance = new CState_Player_Climb_Down(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Climb_Down");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Climb_Down::Clone(void* pArg)
{
	CState_Player_Climb_Down* pInstance = new CState_Player_Climb_Down(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Climb_Down");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Climb_Down::Free()
{
}
