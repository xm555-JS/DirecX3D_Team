#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Attack_Base_Charge_Zoom_Begin.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Camera_Target_Position.h"


CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin::CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin::CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin(const CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_ZOOM);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);
	return S_OK;
}

HRESULT CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	m_iCurKeyFrame = 0;

	//m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
}

void CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}
	if (KEY_UP(VK_LBUTTON))
	{
		m_bEndCurState = true;
	}


	/*if (KEY_PRESSING('W') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('D'))
	{
		m_pState_Handler->Set_KeyInput(true);
	}*/


	CCamera_Target_Position* pCamera = ((CCamera_Target_Position*)pGameInstance->Get_CurrentCamera());
	CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();
	_vector vLookDir = { 0.f,0.f,0.f,0.f };
	vLookDir += pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	m_pState_Handler->Set_PlayerLook(XMVector3Normalize(vLookDir));
	m_pState_Handler->Get_Player()->Get_Transform()->YFixedLook(vLookDir);

	if (KEY_DOWN(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_LBUTTON);
	}

	if (KEY_DOWN(VK_SPACE))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_SPACE);
	}

	/*if (m_pState_Handler->Get_KeyInput() &&
		25 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}*/

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;


	/*_uint iCurKeyFrame = m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame();
	if (m_iCurKeyFrame != iCurKeyFrame)
	{
		m_iCurKeyFrame = iCurKeyFrame;
		if (6 == m_iCurKeyFrame)
		{
			_matrix matPosition = m_pTargetModel->Find_HierarcyNode("Root_Bone")->Get_CombinedMatrix() * XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());

			matPosition.r[0] = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()).r[0];
			matPosition.r[1] = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()).r[1];
			matPosition.r[2] = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()).r[2];
			EFFECT_EXCUTE("Attack_A_M", matPosition);
		}
	}*/

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin::Exit(void* pArg)
{
	/*m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	m_pColCom->Set_ColliderState(false);*/
	__super::Exit();
}

_bool CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin::Check_Condition(CState_Handler* pHandler)
{
	if (0.5f < pHandler->Get_CurState()->Get_KeyPressTimeAcc())
		return true;

	return false;
}


CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin* CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin* pInstance = new CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin::Clone(void* pArg)
{
	CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin* pInstance = new CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin::Free()
{
}
