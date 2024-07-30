#include "stdafx.h"
#include "..\Public\State_Leviathan_Idle.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Leviathan.h"

CState_Leviathan_Idle::CState_Leviathan_Idle(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Leviathan(pManager, eState)
{
}

CState_Leviathan_Idle::CState_Leviathan_Idle(const CState_Leviathan_Idle& rhs)
	: CState_Leviathan(rhs)
{
}

HRESULT CState_Leviathan_Idle::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_APEAR);
	m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_APEAR] = 1;

	return S_OK;
}

HRESULT CState_Leviathan_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Idle::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Leviathan_Idle::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (static_cast<CEnemy_Boss_Leviathan*>(m_pState_Handler->Get_Owner())->Get_IsApear() || KEY_DOWN('B'))
	{
		pGameInstance->Add_Event(EVENT_LEVIATHAN_APEAR, nullptr, nullptr);
		m_bEndCurState = true;

		MGR(CSound_Manager)->ChangeBGM(TEXT("Leviathan_Battle.mp3"), 0.3f, 0.1f);
	}

	//CState_Boss::Tick_AnimationFix(fTimeDelta, "Bip001", m_pOwnerTranformCom);
	CState_Boss::Tick_AnimationLerp(fTimeDelta, "Bone_ROOT", m_pOwnerTranformCom, XMMatrixRotationY(XMConvertToRadians(0.0f)), XMConvertToRadians(0.f));
	//CState_AI::Tick(fTimeDelta);
	//CState_AI::Tick(fTimeDelta);
}

void CState_Leviathan_Idle::Exit(void* pArg)
{
	__super::Exit(pArg);
}

_bool CState_Leviathan_Idle::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Idle* CState_Leviathan_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Idle* pInstance = new CState_Leviathan_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Idle::Clone(void* pArg)
{
	CState_Leviathan_Idle* pInstance = new CState_Leviathan_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Idle::Free()
{
	__super::Free();
}
