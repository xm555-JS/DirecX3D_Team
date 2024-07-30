#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Walk_B.h"
#include "GameInstance.h"
#include "GameObject.h"

CState_Enemy_HunterDog_Walk_B::CState_Enemy_HunterDog_Walk_B(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Walk_B::CState_Enemy_HunterDog_Walk_B(const CState_Enemy_HunterDog_Walk_B& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Walk_B::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Walk_B::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Walk_B::Enter(void* pArg)
{
	m_fPlayTime = fRandom(4.f, 4.f);


	m_fSpeed = 0.5f;
	m_fAnimSpeed = 0.35f;
}

void CState_Enemy_HunterDog_Walk_B::Tick(_float fTimeDelta)
{
	{
		if (Is_Damaged())
			m_bEndCurState = true;
	}
	
	if (m_fPlayTime < m_pState_Handler->Get_TimeAcc())
		m_bEndCurState = true;

	
	m_pState_Handler->Get_Owner()->Get_Transform()->LookAt(m_pState_Handler->Get_Player()->Get_Pos());

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HunterDog_Walk_B::Exit(void* pArg)
{

}

_bool CState_Enemy_HunterDog_Walk_B::Check_Condition(CState_Handler* pHandler)
{
	return false;
}


CState_Enemy_HunterDog_Walk_B* CState_Enemy_HunterDog_Walk_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Walk_B* pInstance = new CState_Enemy_HunterDog_Walk_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Walk_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Walk_B::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Walk_B* pInstance = new CState_Enemy_HunterDog_Walk_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Walk_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Walk_B::Free()
{
	__super::Free();
}
