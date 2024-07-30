#include "stdafx.h"
#include "..\Public\State_Dragon_DoEnd.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"
#include "Imgui_Manager.h"
#include "Easing_Utillity.h"

CState_Dragon_DoEnd::CState_Dragon_DoEnd(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_DoEnd::CState_Dragon_DoEnd(const CState_Dragon_DoEnd& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_DoEnd::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_RUNEND);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_RUNEND] = 1;

	return S_OK;
}

HRESULT CState_Dragon_DoEnd::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_DoEnd::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
	m_fEasingTime = 0.f;
	_uint iRand = 1;//Random(0,2);

	m_vCenter = _float4(100.f, 165.f, 435.27, 1.f);
	
	_float4 vPos;

	switch (iRand)
	{
	case 0:
		vPos = _float4(121.20f, 152.f, 448.45f, 1.f);
		break;
	case 1:
		vPos = _float4(78.38f, 152.f, 448.92f, 1.f);
		break;
	case 2:
		vPos = _float4(99.71f, 152.f, 411.89, 1.f);
		break;
	default:
		break;
	}

	_vector vDir = XMLoadFloat4(&vPos) - XMLoadFloat4(&m_vCenter);

	XMStoreFloat4(&m_vPos, XMLoadFloat4(&m_vCenter) + XMVector3Normalize(vDir) * 30.f);
	XMStoreFloat4(&m_vTargetPos, XMVectorSet(m_vPos.x, 173.5f, m_vPos.z, 1.f));
	XMStoreFloat4(&m_vFinalPos, XMVectorSet(m_vPos.x, 163.5f, m_vPos.z, 1.f));

	m_pOwnerTranformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vPos));
	m_pOwnerTranformCom->YFixedLookAt(XMLoadFloat4(&m_vCenter));

	//Set_CurKeyFrame(35);
	Set_CurKeyFrame(600);

	//юс╫ц
	_float4 vColor = ((CEnemy_Boss_FrostDragon*)m_pOwner)->Get_Color();
	vColor.w = 1.f;

	((CEnemy_Boss_FrostDragon*)m_pOwner)->Set_Color(vColor);
}

void CState_Dragon_DoEnd::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	//_float4 vColor = ((CEnemy_Boss_FrostDragon*)m_pOwner)->Get_Color();

	if (625 < Get_CurKeyFrame())
	{
		m_bEndCurState = true;
	}
	
	//if (vPos.y < 163.5f)
	// 610
	//	m_pOwnerTranformCom->Go_Up(fTimeDelta, DEBUGFLOAT);

	if (610 > Get_CurKeyFrame())
	{
		if (m_fAccTime < 0.5f)
			m_fAccTime += fTimeDelta;

		_vector vPos = CEasing_Utillity::sinfInOut(XMLoadFloat4(&m_vPos), XMLoadFloat4(&m_vTargetPos), m_fAccTime, 0.5f);
		m_pOwnerTranformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	}
	else
	{
		if (m_fEasingTime < 0.4f)
			m_fEasingTime += fTimeDelta;

		_vector vPos = CEasing_Utillity::sinfInOut(XMLoadFloat4(&m_vTargetPos), XMLoadFloat4(&m_vFinalPos), m_fEasingTime, 0.4f);
		m_pOwnerTranformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	}

	STATE_TYPE	eNextState = CState_AI::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pOwnerTranformCom->Reset_RootBoneDiff();

		m_pState_Handler->Change_State(eNextState);

		m_pTargetModel->Set_RootEasing(true);
	}

	m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, "ROOT");
}

void CState_Dragon_DoEnd::Exit(void* pArg)
{
	m_pTargetModel->Set_ModelEasingRatio(0.2f);
}

_bool CState_Dragon_DoEnd::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Dragon_DoEnd* CState_Dragon_DoEnd::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_DoEnd* pInstance = new CState_Dragon_DoEnd(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_DoEnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_DoEnd::Clone(void* pArg)
{
	CState_Dragon_DoEnd* pInstance = new CState_Dragon_DoEnd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_DoEnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_DoEnd::Free()
{
	__super::Free();
}
