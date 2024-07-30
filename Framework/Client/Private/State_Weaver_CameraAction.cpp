#include "stdafx.h"
#include "State_Weaver_CameraAction.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Enemy.h"

#include "Imgui_Manager.h"

CState_Weaver_CameraAction::CState_Weaver_CameraAction(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_CameraAction::CState_Weaver_CameraAction(const CState_Weaver_CameraAction& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_CameraAction::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;
	return S_OK;
}

HRESULT CState_Weaver_CameraAction::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_CameraAction::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.f;


	// 여기서 카메라 액션 시작.
	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Weaver_Ready.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
}

void CState_Weaver_CameraAction::Tick(_float fTimeDelta)
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
	// 30 에 기지개
	if (28 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bOnce1,
			pGameInstance->Add_DirCA(_float2(1.f, -1.f), { 0.f,0.02f,0.01f,0.f }, 1.f, 1.f, false, true);
			pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 1.f, 1.f, 1.f, 0.03f);
			);	
	}

	if (73 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bOnce2,
		pGameInstance->Stop_DirCA();
		pGameInstance->Stop_RadialBlur();
		);
	}


	if (100 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bOnce0,
			m_fAnimSpeed = 0.5f);
	}
	if (Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);

}

void CState_Weaver_CameraAction::Exit(void* pArg)
{
}

_bool CState_Weaver_CameraAction::Check_Condition(CState_Handler* pHandler)
{

	return true;
}

CState_Weaver_CameraAction* CState_Weaver_CameraAction::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_CameraAction* pInstance = new CState_Weaver_CameraAction(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_CameraAction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_CameraAction::Clone(void* pArg)
{
	CState_Weaver_CameraAction* pInstance = new CState_Weaver_CameraAction(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_CameraAction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_CameraAction::Free()
{
	__super::Free();
}
