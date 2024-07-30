#include "stdafx.h"
#include "..\Public\State_Player_Walk.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Unit.h"
#include "Camera.h"
#include "Player.h"
#include "Weapon.h"

#include "Imgui_Manager.h"

CState_Player_Walk::CState_Player_Walk(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Walk::CState_Player_Walk(const CState_Player_Walk& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Walk::Initialize_Prototype()
{
	//m_ForceStates.push_back(STATE_PLAYER_ATTACK_BASE_CHARGE);
	//m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	//m_ForceStates.push_back(STATE_PLAYER_ATTACK);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_LOW);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_MEDIUM);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_MOUNT_UP);
	m_ForceStates.push_back(STATE_PLAYER_GLIDE_UP);
	m_ForceStates.push_back(STATE_PLAYER_SKATE_CALL);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_A);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	//m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ReserveStates.push_back(STATE_PLAYER_RUN);
	m_ReserveStates.push_back(STATE_PLAYER_HAND_IDLE);

	return S_OK;
}

HRESULT CState_Player_Walk::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Walk::Enter(void* pArg)
{
	if(m_pState_Handler->Get_PreStateType()== STATE_PLAYER_JUMP_FALL)
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Jump_End.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

	__super::Enter(pArg);

	m_fAnimSpeed = 2.f;
	m_bEndCurState = true;
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL())
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsDissolve(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsUsing(false);
	}

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR())
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsDissolve(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsUsing(false);
	}
}

void CState_Player_Walk::Tick(_float fTimeDelta)
{
	Set_PlayerLook(fTimeDelta);



	USEGAMEINSTANCE;
	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D'))
	{
		_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
		_float	fRunSpeed = fPlayerSpeed * 4.f;
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		//CTransform* pCurCameraTransform = pGameInstance->Get_CurrentCamera()->Get_Transform();
		CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();

		pPlayerTransform->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);

	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;


	IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Walk.mp3"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.8f),0.3f, fTimeDelta);


	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Walk::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	__super::Exit();
}

_bool CState_Player_Walk::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if ((KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D')) &&
		((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput())
	{
		_float4 vPos;
		XMStoreFloat4(&vPos, pHandler->Get_Player()->Get_Pos());
		if (vPos.y < static_cast<CPlayer*>(pHandler->Get_Player())->Get_SwimHeight() + 0.1f)
			return false;

		return true;
	}

	return false;
}

void CState_Player_Walk::EasingTurn(CTransform* pTargetTransform, _fvector vTargetLook)
{
	_vector vLook = pTargetTransform->Get_State(CTransform::STATE_LOOK);
	//_float fRadian = XMVectorGetX(XMVector3AngleBetweenVectors(vLook, vTargetLook));
	//fRadian /= 2.f;
	//pTargetTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f) ,fRadian);

	pTargetTransform->Set_State(CTransform::STATE_LOOK, vLook);


}


CState_Player_Walk* CState_Player_Walk::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Walk* pInstance = new CState_Player_Walk(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Walk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Walk::Clone(void* pArg)
{
	CState_Player_Walk* pInstance = new CState_Player_Walk(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Walk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Walk::Free()
{
}
