#include "stdafx.h"
#include "..\Public\State_Player_Skate_Call.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

#include "Easing_Utillity.h"

//#include "Imgui_Manager.h"

CState_Player_Skate_Call::CState_Player_Skate_Call(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player_Skate(pManager, eState)
{
}

CState_Player_Skate_Call::CState_Player_Skate_Call(const CState_Player_Skate_Call& rhs)
	: CState_Player_Skate(rhs)
{
}

HRESULT CState_Player_Skate_Call::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);

	m_ReserveStates.push_back(STATE_PLAYER_SKATE_RUN);

	return S_OK;
}

HRESULT CState_Player_Skate_Call::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Skate_Call::Enter(void* pArg)
{
	//197
	__super::Enter(pArg);

	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));

	XMStoreFloat4(&m_vPos, m_pPlayer->Get_Pos());
	m_vPos.y = m_pPlayer->Get_SwimHeight() + 0.1f;
	m_pPlayer->Set_Pos(XMLoadFloat4(&m_vPos));
	m_vTargetPos = m_vPos;
	m_vTargetPos.y = 10.4f;
	m_fUpTime = 0.f;

	EFFECT_EXCUTE("Skate_Boat", XMMatrixIdentity(),
		1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "", &m_listEffects);
}

void CState_Player_Skate_Call::Tick(_float fTimeDelta)
{
	Set_PlayerLook(fTimeDelta);
	CNavigation* pPlayerNavi = (CNavigation*)m_pPlayer->Get_Component(ID_UINT(NAVIGATION));
	_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
	_float	fRunSpeed = fPlayerSpeed * 12.f;
	if (10 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("State_Call.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		if (10.5f > XMVectorGetY(m_pPlayer->Get_Pos()))
		{
			m_fUpTime += fTimeDelta * m_fAnimSpeed;
			_float fCurY = XMVectorGetY(CEasing_Utillity::sinfInOut(XMLoadFloat4(&m_vPos),
				XMLoadFloat4(&m_vTargetPos), m_fUpTime, 0.5f));
			m_pPlayer->Get_Transform()->Skate(fCurY, pPlayerNavi);

		}
	}
	else
	{
		m_pPlayer->Get_Transform()->Skate(10.5f, pPlayerNavi);
		_float4 vPos;
		XMStoreFloat4(&vPos, m_pPlayer->Get_Pos());
		_uint i = 0;
	}


	m_pPlayer->Get_Transform()->Go_AccFront_Limit(fTimeDelta, fRunSpeed, 16.f, pPlayerNavi, &m_fRunSpeed);
	//pPlayerTransform->Go_AccFront_Limit(fTimeDelta, fRunSpeed, 16.f, pPlayerNavi, &m_fRunSpeed);

	if(Get_IsFinished())
		m_bEndCurState = true;

	CState_Player_Skate::Tick(fTimeDelta);
}

void CState_Player_Skate_Call::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	// 만약 Female일 경우
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_Player_Skate_Call::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN('3') && ((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput())
	{
		return true;
	}

	return false;
}


CState_Player_Skate_Call* CState_Player_Skate_Call::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Skate_Call* pInstance = new CState_Player_Skate_Call(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Skate_Call");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Skate_Call::Clone(void* pArg)
{
	CState_Player_Skate_Call* pInstance = new CState_Player_Skate_Call(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Skate_Call");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Skate_Call::Free()
{
}
