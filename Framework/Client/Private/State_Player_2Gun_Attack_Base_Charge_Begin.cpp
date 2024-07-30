#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Attack_Base_Charge_Begin.h"
#include "GameInstance.h"
#include "Channel.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Camera_Target_Position.h"
#include "GameObject.h"


CState_Player_2Gun_Attack_Base_Charge_Begin::CState_Player_2Gun_Attack_Base_Charge_Begin(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Attack_Base_Charge_Begin::CState_Player_2Gun_Attack_Base_Charge_Begin(const CState_Player_2Gun_Attack_Base_Charge_Begin& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Attack_Base_Charge_Begin::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKFRONT);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKBACK);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKLEFT);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKRIGHT);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);
	return S_OK;
}

HRESULT CState_Player_2Gun_Attack_Base_Charge_Begin::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Attack_Base_Charge_Begin::Enter(void* pArg)
{
	MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_Base_Charge_Begin.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f);

	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 1.5f;

	m_iCurKeyFrame = 0;

	_matrix OffsetMatrix = XMMatrixScaling(0.7f, 0.7f, 0.7f);
	EFFECT_EXCUTE("Player_Gun_Attack_Foot", OffsetMatrix, 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_2Gun_Attack_Base_Charge_Begin::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] += XMVector3Normalize(ColMatrix.r[2]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 3.f);
	m_pColCom->Update(ColMatrix);

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(300.f, 360.f), 10, 30, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Gun_Hit");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2GunHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(300.f, 360.f)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.03f, 0.2f, 0.2f));
	}

	if (16 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_Base_Charge_Begin.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Gun_Attack_Foot2", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
	}

	/*if(nullptr != m_pColCom)
		m_pColCom->Set_ColliderState(true);*/

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}


#if 0
	CCamera_Target_Position* pCamera = ((CCamera_Target_Position*)pGameInstance->Get_CurrentCamera());
	CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();
	_vector vLookDir = { 0.f,0.f,0.f,0.f };
	vLookDir += pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	m_pState_Handler->Set_PlayerLook(XMVector3Normalize(vLookDir));
	m_pState_Handler->Get_Player()->Get_Transform()->YFixedLook(vLookDir);
#endif // 0


	/*if (KEY_PRESSING('W') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('D'))
	{
		m_pState_Handler->Set_KeyInput(true);
	}*/

	/*if (KEY_DOWN(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_LBUTTON);
	}*/


	/*if (m_pState_Handler->Get_KeyInput() &&
		25 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}*/

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;
	
	if (38 < Get_CurKeyFrame() && nullptr == m_pPlayer->Get_Target())
	{

		CCamera_Target_Position* pCamera = dynamic_cast<CCamera_Target_Position*>(pGameInstance->Get_CurrentCamera());
		if (nullptr == pCamera)
			Set_PlayerLook(fTimeDelta);
		else
		{
			_vector vCamRight = pCamera->Get_Transform()->Get_State(CTransform::STATE_RIGHT);

			_vector vLookDir = XMVector3Cross(vCamRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));

			m_pState_Handler->Set_PlayerLook(vLookDir);
			m_pState_Handler->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_RIGHT, vCamRight);
			m_pState_Handler->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_LOOK, vLookDir);
		}
	}
	else
		Set_PlayerLook(fTimeDelta);

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_2Gun_Attack_Base_Charge_Begin::Exit(void* pArg)
{
	/*m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	m_pColCom->Set_ColliderState(false);*/
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_2Gun_Attack_Base_Charge_Begin::Check_Condition(CState_Handler* pHandler)
{
	if (0.3f < pHandler->Get_CurState()->Get_KeyPressTimeAcc())
		return true;

	return false;
}


CState_Player_2Gun_Attack_Base_Charge_Begin* CState_Player_2Gun_Attack_Base_Charge_Begin::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Attack_Base_Charge_Begin* pInstance = new CState_Player_2Gun_Attack_Base_Charge_Begin(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Attack_Base_Charge_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Attack_Base_Charge_Begin::Clone(void* pArg)
{
	CState_Player_2Gun_Attack_Base_Charge_Begin* pInstance = new CState_Player_2Gun_Attack_Base_Charge_Begin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Attack_Base_Charge_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Attack_Base_Charge_Begin::Free()
{
}
