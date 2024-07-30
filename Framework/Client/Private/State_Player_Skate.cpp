#include "stdafx.h"
#include "..\Public\State_Player_Skate.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_Player_Skate::CState_Player_Skate(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Skate::CState_Player_Skate(const CState_Player_Skate& rhs)
	: CState_Player(rhs)
{
}

void CState_Player_Skate::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	if (nullptr != pArg)
		memcpy(&m_fRunSpeed, pArg, sizeof(_float));

	m_fForce = 12.f;

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

	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsGlow(true);
}

void CState_Player_Skate::Tick(_float fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;
	//Set_PlayerLook(fTimeDelta);

	STATE_TYPE	eNextState = CState::Ready_NextState();

	CNavigation* pPlayerNavigation = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(NAVIGATION);
	if (STATE_END != eNextState)
	{
		CTransform* pPlayerTrasform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
		pPlayerTrasform->Reset_RootBoneDiff();

		m_pState_Handler->Change_State(eNextState, &m_fRunSpeed);

		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, "Root_Bone", &m_RootBoneMatrix);
		m_pTargetModel->Set_ModelEasingRatio(0.2f);
	}
	else
	{
		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, "Root_Bone", &m_RootBoneMatrix);
	}

	CHierarchyNode* pHierarchyNode = m_pTargetModel->Find_HierarcyNode("Root_Bone");
	XMStoreFloat4x4(&m_RootBoneMatrix,/* pHierarchyNode->Get_OffsetMatrix() **/ pHierarchyNode->Get_RealCombinedMatrix() * XMLoadFloat4x4(&m_pTargetModel->Get_TransformMatrix())/*pHierarchyNode->Get_TransformMatrix()*/);

	m_pState_Handler->Get_Player()->Get_Transform()->Set_RootBoneMatrix(m_RootBoneMatrix, pPlayerNavigation);
}

void CState_Player_Skate::Exit(void* pArg)
{
	__super::Exit();
}

void CState_Player_Skate::Free()
{
}
