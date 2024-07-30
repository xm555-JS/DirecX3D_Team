#include "stdafx.h"
#include "..\Public\State_Dragon_Apear.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

CState_Dragon_Apear::CState_Dragon_Apear(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Apear::CState_Dragon_Apear(const CState_Dragon_Apear& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Apear::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_CAM_ACTION);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_CAM_ACTION] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Apear::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Apear::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fEasingTime = m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_Duration();
	m_fAccTime = 0.f;
	m_fAnimSpeed = 3.f;

	XMStoreFloat4(&m_vStartLook, m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK));

	// 여기서 카메라 액션 시작.
	USEGAMEINSTANCE;
	pGameInstance->Play_ActionCamera(TEXT("Dragon_CameraAction"), 2.f);
	/*pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 4.5f, 4.5f, 1.f, 0.01f, false, true);
	pGameInstance->Add_DirCA(_float2(1.f, -1.f), _float4(0.015f,0.f,0.0075f,0.f), 4.5f, 4.5f, false, true);*/
	/*pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 4.f, 0.5f, 1.f, 0.025f);
	pGameInstance->Add_DirCA(_float2(1.f, -1.f), _float4(0.015f, 0.f, 0.0075f, 0.f), 4.f, 0.5f);*/
}

void CState_Dragon_Apear::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (20 < Get_CurKeyFrame())
	{
		pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 4.f, 0.5f, 1.f, 0.025f);
		pGameInstance->Add_DirCA(_float2(1.f, -1.f), _float4(0.015f, 0.f, 0.0075f, 0.f), 4.f, 0.5f);
	}

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vLook = XMLoadFloat4(&vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float	fCurTime = min(1.f, m_fAccTime / m_fEasingTime);

	//vCurLook
	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime);

	//_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION);
	m_pOwnerTranformCom->YFixedLook(vFinalLook);

	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Apear::Exit(void* pArg)
{
	((CEnemy_Boss_FrostDragon*)m_pOwner)->Set_PhaseStart();
	m_pTargetModel->Set_ModelEasingRatio(0.2f);	
}

_bool CState_Dragon_Apear::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Dragon_Apear* CState_Dragon_Apear::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Apear* pInstance = new CState_Dragon_Apear(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Apear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Apear::Clone(void* pArg)
{
	CState_Dragon_Apear* pInstance = new CState_Dragon_Apear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Apear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Apear::Free()
{
	__super::Free();
}
