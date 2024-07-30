#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Attack_Base_Charge_WalkRight.h"
#include "GameInstance.h"
#include "Channel.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Unit.h"
#include "Navigation.h"
#include "Camera_Target_Position_Lerp.h"
#include "Bullet_Player_2Gun.h"

CState_Player_2Gun_Attack_Base_Charge_WalkRight::CState_Player_2Gun_Attack_Base_Charge_WalkRight(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Attack_Base_Charge_WalkRight::CState_Player_2Gun_Attack_Base_Charge_WalkRight(const CState_Player_2Gun_Attack_Base_Charge_WalkRight& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Attack_Base_Charge_WalkRight::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKFRONT);
	m_ForceStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKBACK);
	m_ForceStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKLEFT);
	m_ForceStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKRIGHT);
	m_ForceStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_2Gun_Attack_Base_Charge_WalkRight::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Attack_Base_Charge_WalkRight::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 1.5f;

	m_iCurKeyFrame = 0;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
}

void CState_Player_2Gun_Attack_Base_Charge_WalkRight::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fTimeAcc += fTimeDelta;

	if (KEY_UP(VK_LBUTTON))
	{
		m_bEndCurState = true;
	}

	m_fTimeStamina += fTimeDelta;
	if (0.3f < m_fTimeStamina)
	{
		m_fTimeStamina = 0.f;
		Use_StaminaGauge(0.025f);
	}

	if (!Enable_Use_Stamina(m_pState_Handler, false))
	{
		m_bEndCurState = true;
	}

	/*if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING('W') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('D'))
	{
		m_pState_Handler->Set_KeyInput(true);
	}*/

	//if (KEY_DOWN(VK_LBUTTON))
	//{
	//	m_pState_Handler->Set_KeyInput(true);
	//	m_pState_Handler->Add_KeyInput(VK_LBUTTON);
	//}


	//if (m_pState_Handler->Get_KeyInput() &&
	//	25 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	//{
	//	m_bEndCurState = true;
	//}
	if (KEY_PRESSING('D'))
	{
		_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
		_float	fRunSpeed = fPlayerSpeed * 1.f;
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		//CTransform* pCurCameraTransform = pGameInstance->Get_CurrentCamera()->Get_Transform();
		CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();
		pPlayerTransform->Go_Right(fTimeDelta, fRunSpeed, pPlayerNavi);
	}

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
	
	if (nullptr != m_pPlayer->Get_Target())
	{
		_vector	vLookDir = { 0.f, 0.f, 0.f, 0.f };
		vLookDir += m_pPlayer->Get_Target()->Get_Pos()
			- m_pPlayer->Get_Pos();
		_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookDir);

		m_pState_Handler->Set_PlayerLook(XMVector3Normalize(vLookDir));
		m_pPlayer->Get_Transform()->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));
		m_pPlayer->Get_Transform()->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLookDir));
	}

	if (0.15f < m_fTimeAcc)
	{
		MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_A_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f);

		_matrix OffsetMatrix = XMMatrixIdentity();
		OffsetMatrix.r[3] = XMVectorSet(-0.16f, 1.2f, 1.1f, 1.f);
		_matrix TargetMatrix = OffsetMatrix * XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
		_float4	vTargetPos;
		XMStoreFloat4(&vTargetPos, TargetMatrix.r[3]);

		CBullet_Player_2Gun::BULLETDESC BulletDesc;
		BulletDesc.vStartPos = vTargetPos;
		XMStoreFloat4(&BulletDesc.vLookAt, m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_LOOK));

		pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ID_UINT(PLAYER, WEAPON_2GUN_BULLET), ID_UINT(PLAYER, WEAPON_2GUN_BULLET), &BulletDesc);
		//pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ID_UINT(PLAYER, WEAPON_2GUN_BULLET), ID_UINT(PLAYER, WEAPON_2GUN_BULLET), &TargetMatrix.r[3]);

		if (m_bToggle)
		{
			_matrix RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRandom(-3.14f, 3.14f)) * XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), 1.571f) * XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), -0.085f);
			EFFECT_POS_ROT("Player_Gun_Attack_A_5", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(-0.16f, 1.2f, 1.1f, 1.f), RotationMatrix, 1.f, XMVectorSet(1.5f, 1.5f, 2.5f, 0.f));
		}
		else
		{
			_matrix RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRandom(-3.14f, 3.14f)) * XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), 1.571f) * XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.085f);
			EFFECT_POS_ROT("Player_Gun_Attack_B_2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.16f, 1.2f, 1.1f, 1.f), RotationMatrix, 1.f, XMVectorSet(1.5f, 1.5f, 2.5f, 0.f));
		}

		m_fTimeAcc = 0;
		m_bToggle = !m_bToggle;
	}

	CCamera_Target_Position_Lerp* pCamera = dynamic_cast<CCamera_Target_Position_Lerp*>(pGameInstance->Get_CurrentCamera());
	if (nullptr != pCamera)
	{
		_vector vCamRight = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_RIGHT);

		_vector vLookDir = XMVector3Cross(vCamRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));

		pCamera->Get_NextTransform()->Set_State(CTransform::STATE_LOOK, vLookDir);
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_2Gun_Attack_Base_Charge_WalkRight::Exit(void* pArg)
{
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	__super::Exit();
}

_bool CState_Player_2Gun_Attack_Base_Charge_WalkRight::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_PRESSING('D') &&
		KEY_PRESSING(VK_LBUTTON) &&
		Enable_Use_Stamina(pHandler))
	{
		return true;
	}
	return false;
}


CState_Player_2Gun_Attack_Base_Charge_WalkRight* CState_Player_2Gun_Attack_Base_Charge_WalkRight::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Attack_Base_Charge_WalkRight* pInstance = new CState_Player_2Gun_Attack_Base_Charge_WalkRight(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Attack_Base_Charge_WalkRight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Attack_Base_Charge_WalkRight::Clone(void* pArg)
{
	CState_Player_2Gun_Attack_Base_Charge_WalkRight* pInstance = new CState_Player_2Gun_Attack_Base_Charge_WalkRight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Attack_Base_Charge_WalkRight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Attack_Base_Charge_WalkRight::Free()
{
}
