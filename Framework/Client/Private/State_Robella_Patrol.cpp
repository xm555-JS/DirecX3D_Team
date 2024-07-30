#include "stdafx.h"
#include "..\Public\State_Robella_Patrol.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_Patrol::CState_Robella_Patrol(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Patrol::CState_Robella_Patrol(const CState_Robella_Patrol& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Patrol::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Robella_Patrol::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Patrol::Enter(void* pArg)
{
	__super::Enter(pArg);

	// 거리 10은 임시 거리임 나중에 게임플레이 레벨에 올려놓고 확인 해봐야함
	m_fAwake_Distance = 10.f;
}

void CState_Robella_Patrol::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	// 플레이어랑 가까워진다면 Awake_State로 변경
	/*if (Get_PlayerPosLength(m_pState_Handler) < m_fAwake_Distance)
	{
		m_bEndCurState = true;
	}*/

	if (KEY_DOWN('N'))
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Patrol::Exit(void* pArg)
{
}

_bool CState_Robella_Patrol::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Robella_Patrol* CState_Robella_Patrol::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Patrol* pInstance = new CState_Robella_Patrol(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Patrol");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Patrol::Clone(void* pArg)
{
	CState_Robella_Patrol* pInstance = new CState_Robella_Patrol(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Patrol");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Patrol::Free()
{
	__super::Free();
}
