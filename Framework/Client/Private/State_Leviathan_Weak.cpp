#include "stdafx.h"
#include "..\Public\State_Leviathan_Weak.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

CState_Leviathan_Weak::CState_Leviathan_Weak(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Leviathan_Weak::CState_Leviathan_Weak(const CState_Leviathan_Weak& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Leviathan_Weak::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR);
	m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR] = 1;

	return S_OK;
}

HRESULT CState_Leviathan_Weak::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Weak::Enter(void* pArg)
{
	__super::Enter(pArg);

	GAMEINSTANCE->Play_ActionCamera(TEXT("Leviathan_Damaged"), 1.f);
}

void CState_Leviathan_Weak::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (Get_IsFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Leviathan_Weak::Exit(void* pArg)
{
	__super::Exit(pArg);
}

_bool CState_Leviathan_Weak::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	list<CGameObject*>* pGameObjs = pGameInstance->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL));

	if(!m_bFirst)
	{
		m_iPreSize = (pGameObjs->size());
		m_iCurSize = (pGameObjs->size());
		m_bFirst = true;
		return false;
	}
	else
	{
		m_iCurSize = (pGameObjs->size());
		
		if (m_iPreSize != m_iCurSize)
		{
			m_iPreSize = m_iCurSize;
			return true;
		}
	}

	return false;
}

CState_Leviathan_Weak* CState_Leviathan_Weak::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Weak* pInstance = new CState_Leviathan_Weak(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Weak");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Weak::Clone(void* pArg)
{
	CState_Leviathan_Weak* pInstance = new CState_Leviathan_Weak(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Weak");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Weak::Free()
{
	__super::Free();
}
