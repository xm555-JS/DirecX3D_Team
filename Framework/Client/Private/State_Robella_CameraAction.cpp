#include "stdafx.h"
#include "..\Public\State_Robella_CameraAction.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Enemy.h"

#include "Effect_Base.h"

CState_Robella_CameraAction::CState_Robella_CameraAction(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_CameraAction::CState_Robella_CameraAction(const CState_Robella_CameraAction& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_CameraAction::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_IDLE] = 1;
	return S_OK;
}

HRESULT CState_Robella_CameraAction::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_CameraAction::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 0.5f;


	// 여기서 카메라 액션 시작.
	USEGAMEINSTANCE;
	pGameInstance->Play_ActionCamera(TEXT("Robella_CameraAction"), 2.f);
	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
}

void CState_Robella_CameraAction::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vLook = XMLoadFloat4(&vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float	fCurTime = min(1.f, m_fAccTime / 2.f);

	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime * fCurTime);

	_float fDgree = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(vLook, vFinalLook)));

	m_pOwnerTranformCom->YFixedLook(vFinalLook);

	if (10.f < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect1,
			pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 3.f, 3.f, 1.f, 0.02f);
			pGameInstance->Add_RadialCA(_float2(0.5f, 0.5f), _float4(0.f, 0.03f, 0.015f, 0.f), 3.f, 3.f);
		);
	}
	if (57.f < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SoundWave1.mp3"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}

	if (63.f < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_EXCUTE("Boss_Robella_SoundWave_Attack", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone_eyes");
			pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 1.2f, 0.4f, 1.f, 0.02f);
			pGameInstance->Add_RadialCA(_float2(0.5f, 0.5f), _float4(0.f, 0.035f, 0.0175f, 0.f), 1.2f, 0.4f);
		);
	
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SoundWave1.mp3"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}
	if (Get_IsFinished())
	{
		m_bIsUsed = true;
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);

}

void CState_Robella_CameraAction::Exit(void* pArg)
{
}

_bool CState_Robella_CameraAction::Check_Condition(CState_Handler* pHandler)
{
	if (m_bIsUsed)
		return false;

	return true;
}

CState_Robella_CameraAction* CState_Robella_CameraAction::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_CameraAction* pInstance = new CState_Robella_CameraAction(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_CameraAction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_CameraAction::Clone(void* pArg)
{
	CState_Robella_CameraAction* pInstance = new CState_Robella_CameraAction(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_CameraAction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_CameraAction::Free()
{
	__super::Free();
}
