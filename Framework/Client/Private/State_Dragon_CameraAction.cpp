#include "stdafx.h"
#include "..\Public\State_Dragon_CameraAction.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Enemy.h"

#include "Effect_Base.h"

CState_Dragon_CameraAction::CState_Dragon_CameraAction(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_CameraAction::CState_Dragon_CameraAction(const CState_Dragon_CameraAction& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_CameraAction::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_IDLE] = 1;
	return S_OK;
}

HRESULT CState_Dragon_CameraAction::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_CameraAction::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.f;
}

void CState_Dragon_CameraAction::Tick(_float fTimeDelta)
{
	if (Get_IsFinished())
		m_bEndCurState = true;


	if (51 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_EXCUTE("Boss_Dragon_CameraAction", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "tongue_00"));
		/*USEGAMEINSTANCE;
		pGameInstance->Stop_RadialBlur();
		pGameInstance->Stop_DirCA();*/
	}

	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySound(TEXT("Action_Roar.mp3"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 0.5f));

#if 0
	if (30 <= Get_CurKeyFrame())
	{
		m_fTimeAcc_Effect0 += fTimeDelta;
		if (0.1f < m_fTimeAcc_Effect0)
		{
			m_fTimeAcc_Effect0 = 0.f;
			_float fAngle = fRandom(-180.f, 180.f);
			EFFECT_POS("Boss_Dragon_Thunder_Red", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-20.f, 20.f), -10.f, fRandom(-2.f, 2.f), 1.f), XMVectorSet(0.f, XMConvertToRadians(fAngle), 0.f, 0.f), 1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 0.f));
			EFFECT_POS("Boss_Dragon_Thunder_Blue", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-20.f, 20.f), -10.f, fRandom(-2.f, 2.f), 1.f), XMVectorSet(0.f, XMConvertToRadians(fAngle), 0.f, 0.f), 1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 0.f));
		}
	}
#endif // 0

	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_CameraAction::Exit(void* pArg)
{
	USEGAMEINSTANCE;
	pGameInstance->Stop_RadialBlur();
	pGameInstance->Stop_DirCA();
}

_bool CState_Dragon_CameraAction::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Dragon_CameraAction* CState_Dragon_CameraAction::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_CameraAction* pInstance = new CState_Dragon_CameraAction(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_CameraAction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_CameraAction::Clone(void* pArg)
{
	CState_Dragon_CameraAction* pInstance = new CState_Dragon_CameraAction(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_CameraAction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_CameraAction::Free()
{
	__super::Free();
}
