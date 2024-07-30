#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Attack_Base_Charge_Zoom.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Camera_Target_Position_Lerp.h"



CState_Player_2Gun_Attack_Base_Charge_Zoom::CState_Player_2Gun_Attack_Base_Charge_Zoom(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Attack_Base_Charge_Zoom::CState_Player_2Gun_Attack_Base_Charge_Zoom(const CState_Player_2Gun_Attack_Base_Charge_Zoom& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Attack_Base_Charge_Zoom::Initialize_Prototype()
{
	//m_ForceStates.push_back(STATE_PLAYER_2GUN_SKILL_NORMAL);
	///*m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_B);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_2Gun_Attack_Base_Charge_Zoom::Initialize(void* pArg)
{
	USEGAMEINSTANCE;

	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vEye = _float4(0.0f, 0.f, -1.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = g_fNear;
	CameraDesc.fFar = g_fFar;
	m_Cameras_Current = static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_TARGET_POSITION_LERP), &CameraDesc));
	NULLCHECK(m_Cameras_Current);


	return S_OK;
}

void CState_Player_2Gun_Attack_Base_Charge_Zoom::Enter(void* pArg)
{
	__super::Enter(pArg);
	USEGAMEINSTANCE;
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 1.5f;

	m_iCurKeyFrame = 0;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));


	m_pState_Handler->Get_Player()->Get_Transform()->LookTo(pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]);
	m_Cameras_Last = pGameInstance->Get_CurrentCamera();

	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vEye = pGameInstance->Get_CamPosition();
	XMStoreFloat4(&CameraDesc.vAt, m_pState_Handler->Get_Player()->Get_Transform()->Get_WorldMatrix().r[2]);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = g_fNear;
	CameraDesc.fFar = g_fFar;
	((CCamera_Target_Position_Lerp*)m_Cameras_Current)->Set_CameraDesc(CameraDesc);
	((CCamera_Target_Position_Lerp*)m_Cameras_Current)->Set_TargetObject(m_pState_Handler->Get_Player());
	((CCamera_Target_Position_Lerp*)m_Cameras_Current)->Set_Offset(XMVectorSet(0.5f, 1.5f, -1.3f, 0.f));
	((CCamera_Target_Position_Lerp*)m_Cameras_Current)->Set_PosLerpRate(1.f);
	((CCamera_Target_Position_Lerp*)m_Cameras_Current)->Set_LookLerpRate(1.f);

	m_Cameras_Current->Get_Transform()->LookTo(m_pState_Handler->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
	_vector vCamOffset = m_pState_Handler->Get_Player()->Get_Transform()->Get_WorldMatrix().r[0] * 0.5f + m_pState_Handler->Get_Player()->Get_Transform()->Get_WorldMatrix().r[1] * 1.5f + m_pState_Handler->Get_Player()->Get_Transform()->Get_WorldMatrix().r[2] * -1.3f;
	m_Cameras_Current->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, m_pState_Handler->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION) + vCamOffset);
	pGameInstance->Set_NextCamera(m_Cameras_Current, 0.15f);

}

void CState_Player_2Gun_Attack_Base_Charge_Zoom::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//pGameInstance->Get_CurrentCamera()->Tick_Camera(fTimeDelta);
	if (nullptr != m_pColCom)
		m_pColCom->Set_ColliderState(true);

	m_fTimeAcc += fTimeDelta;
	if (0.15f < m_fTimeAcc)
	{
		MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_A_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(),0.5f);
		_matrix OffsetMatrix = XMMatrixIdentity();
		OffsetMatrix.r[3] = XMVectorSet(-0.16f, 1.2f, 1.1f, 1.f);
		_matrix TargetMatrix = OffsetMatrix * XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
		_float4	vTargetPos;
		XMStoreFloat4(&vTargetPos, TargetMatrix.r[3]);
		pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ID_UINT(PLAYER, WEAPON_2GUN_BULLET_ZOOM), ID_UINT(PLAYER, WEAPON_2GUN_BULLET_ZOOM), &vTargetPos);
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

	m_fTimeStamina += fTimeDelta;
	if (0.3f < m_fTimeStamina)
	{
		m_fTimeStamina = 0.f;
		Use_StaminaGauge(0.025f);
	}


	if (!Enable_Use_Stamina(m_pState_Handler))
	{
		m_bEndCurState = true;
	}

	if (KEY_UP(VK_LBUTTON))
	{
		m_bEndCurState = true;
	}


	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D'))
	{
		_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
		_float	fRunSpeed = fPlayerSpeed * 3.f;
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		//CTransform* pCurCameraTransform = pGameInstance->Get_CurrentCamera()->Get_Transform();
		CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();

		if (KEY_PRESSING('W'))
		{
			pPlayerTransform->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);
		}
		if (KEY_PRESSING('S'))
		{
			pPlayerTransform->Go_Backward(fTimeDelta, fRunSpeed, pPlayerNavi);
		}
		if (KEY_PRESSING('A'))
		{
			pPlayerTransform->Go_Left(fTimeDelta, fRunSpeed, pPlayerNavi);
		}
		if (KEY_PRESSING('D'))
		{
			pPlayerTransform->Go_Right(fTimeDelta, fRunSpeed, pPlayerNavi);
		}
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;


	CCamera_Target_Position_Lerp* pCamera = ((CCamera_Target_Position_Lerp*)pGameInstance->Get_CurrentCamera());
	_vector vCamRight = pCamera->Get_Transform()->Get_State(CTransform::STATE_RIGHT);

	_vector vLookDir = XMVector3Cross(vCamRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));

	m_pState_Handler->Set_PlayerLook(vLookDir);
	m_pState_Handler->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_RIGHT, vCamRight);
	m_pState_Handler->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_LOOK, vLookDir);
	m_pState_Handler->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_UP, XMVector3Cross(vLookDir, vCamRight));

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_2Gun_Attack_Base_Charge_Zoom::Exit(void* pArg)
{
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));


	m_Cameras_Last->Get_Transform()->LookTo(m_pState_Handler->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_LOOK));

	_float3 vOffset = ((CCamera_Target_Position_Lerp*)m_Cameras_Last)->Get_Offset();
	_vector vCamOffset = m_pState_Handler->Get_Player()->Get_Transform()->Get_WorldMatrix().r[0] * vOffset.x + m_pState_Handler->Get_Player()->Get_Transform()->Get_WorldMatrix().r[1] * vOffset.y + m_pState_Handler->Get_Player()->Get_Transform()->Get_WorldMatrix().r[2] * vOffset.z;
	m_Cameras_Last->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, m_pState_Handler->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION) + vCamOffset);
	GAMEINSTANCE->Set_NextCamera(m_Cameras_Last, 0.15f);

	__super::Exit();
}

_bool CState_Player_2Gun_Attack_Base_Charge_Zoom::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_PRESSING(VK_LBUTTON))
	{
		return true;
	}
	return false;
}


CState_Player_2Gun_Attack_Base_Charge_Zoom* CState_Player_2Gun_Attack_Base_Charge_Zoom::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Attack_Base_Charge_Zoom* pInstance = new CState_Player_2Gun_Attack_Base_Charge_Zoom(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Attack_Base_Charge_Zoom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Attack_Base_Charge_Zoom::Clone(void* pArg)
{
	CState_Player_2Gun_Attack_Base_Charge_Zoom* pInstance = new CState_Player_2Gun_Attack_Base_Charge_Zoom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Attack_Base_Charge_Zoom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Attack_Base_Charge_Zoom::Free()
{
}
