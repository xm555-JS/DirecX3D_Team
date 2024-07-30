#include "stdafx.h"
#include "..\Public\State_Player_Skate_Stop_Water.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"
#include "Easing_Utillity.h"

//#include "Imgui_Manager.h"
#include "Effect_Base.h"

CState_Player_Skate_Stop_Water::CState_Player_Skate_Stop_Water(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player_Skate(pManager, eState)
{
}

CState_Player_Skate_Stop_Water::CState_Player_Skate_Stop_Water(const CState_Player_Skate_Stop_Water& rhs)
	: CState_Player_Skate(rhs)
{
}

HRESULT CState_Player_Skate_Stop_Water::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);

	m_ReserveStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ReserveStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	return S_OK;
}

HRESULT CState_Player_Skate_Stop_Water::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Skate_Stop_Water::Enter(void* pArg)
{

	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("State_Stop.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

	//199
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	m_fDownTime = 0.f;
	XMStoreFloat4(&m_vCurPos, m_pPlayer->Get_Pos());
	m_pPlayer->Get_Transform()->Reset_FrontAccTime();

	m_listEffects = *((CState_Player_Skate*)m_pState_Handler->Get_State(STATE_PLAYER_SKATE_CALL))->Get_Effects();

	for (auto& iter : m_listEffects)
		iter->Set_EffectOff();
}

void CState_Player_Skate_Stop_Water::Tick(_float fTimeDelta)
{
	Set_PlayerLook(fTimeDelta);

	if (Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	USEGAMEINSTANCE;
	_float4 vPos, TargetPos;
	_float fDuration, fFixY;
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));

	if(20 > Get_CurKeyFrame())
	{

		XMStoreFloat4(&vPos, m_pPlayer->Get_Pos());
		vPos.y = m_vCurPos.y;
		m_pPlayer->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&vPos));
	}
	else if (20 <= Get_CurKeyFrame() && 
		40 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Swim_Diving.mp3"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 1.f));

		if (KEY_PRESSING('W') ||
			KEY_PRESSING('A') ||
			KEY_PRESSING('S') ||
			KEY_PRESSING('D'))
		{
			_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
			_float	fRunSpeed = fPlayerSpeed * 4.f;
			//CTransform* pCurCameraTransform = pGameInstance->Get_CurrentCamera()->Get_Transform();
			CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();

			pPlayerTransform->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);

		}
		fDuration = 0.5f;
		if(fDuration > m_fDownTime)
			m_fDownTime += fTimeDelta * m_fAnimSpeed;
		TargetPos = m_vCurPos;
		TargetPos.y = m_pPlayer->Get_SwimHeight();
		fFixY = XMVectorGetY(CEasing_Utillity::sinfInOut(XMLoadFloat4(&m_vCurPos), XMLoadFloat4(&TargetPos), m_fDownTime, fDuration));

		_vector vPos = XMVectorSetY(m_pPlayer->Get_Pos(), fFixY);

		m_pPlayer->Get_Transform()->Skate(fFixY, pPlayerNavi);
		m_pPlayer->Set_Pos(vPos);
	}
	else if (40 <= Get_CurKeyFrame())
	{

		m_pPlayer->Get_Transform()->Skate(m_pPlayer->Get_SwimHeight(), pPlayerNavi);
		//m_pPlayer->Get_Transform()->Swim(static_cast<CPlayer*>(m_pState_Handler->Get_Player())->Get_SwimHeight(), pPlayerNavi);
	}

	CState_Player_Skate::Tick(fTimeDelta);
}

void CState_Player_Skate_Stop_Water::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	m_pPlayer->Get_Transform()->Reset_FrontAccTime();
	m_pPlayer->Get_Transform()->Set_Skate(false);
	__super::Exit();
}

_bool CState_Player_Skate_Stop_Water::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	_float4 vPos;
	XMStoreFloat4(&vPos, pHandler->Get_Player()->Get_Pos());

	_float fSpeed = ((CState_Player_Skate*)pHandler->Get_CurState())->Get_RunSpeed();

	CTransform* pPlayerTransform = (CTransform*)pHandler->Get_Player()->Get_Component(TRANSFORM);
	CNavigation* pPlayerNavi = (CNavigation*)pHandler->Get_Player()->Get_Component(NAVIGATION);
	_float			fHightDiff = 0.f;
	pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

	if ((KEY_DOWN('3') || 0 >= fSpeed) && fHightDiff > 1.f)
	{
		return true;
	}

	return false;
}


CState_Player_Skate_Stop_Water* CState_Player_Skate_Stop_Water::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Skate_Stop_Water* pInstance = new CState_Player_Skate_Stop_Water(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Skate_Stop_Water");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Skate_Stop_Water::Clone(void* pArg)
{
	CState_Player_Skate_Stop_Water* pInstance = new CState_Player_Skate_Stop_Water(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Skate_Stop_Water");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Skate_Stop_Water::Free()
{
}
