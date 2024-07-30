#include "stdafx.h"
#include "..\Public\State_Dragon_Weak_End.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Channel.h"

#include "Enemy_Boss_Dragon.h"

CState_Dragon_Weak_End::CState_Dragon_Weak_End(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Weak_End::CState_Dragon_Weak_End(const CState_Dragon_Weak_End& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Weak_End::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_DOEND);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_DOEND] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Weak_End::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Weak_End::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;

	XMStoreFloat4(&m_vStartLook, m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK));
}

void CState_Dragon_Weak_End::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	if (53 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& 79 > m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		_float fTickperSec = fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();	//1프레임당 시간

		m_pOwner->Get_Transform()->Go_Up(fTickperSec, 2.f);

		_float4 vColor = ((CEnemy_Boss_FrostDragon*)m_pOwner)->Get_Color();
		vColor.w -= fTickperSec / 26.f;

		((CEnemy_Boss_FrostDragon*)m_pOwner)->Set_Color(vColor);

		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySound(TEXT("Fly.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 1.f));
	}


	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Weak_End::Exit(void* pArg)
{
	m_pTargetModel->Set_ModelEasingRatio(0.1f);

	_float4 vColor = ((CEnemy_Boss_FrostDragon*)m_pOwner)->Get_Color();
	vColor.w = 0.f;

	((CEnemy_Boss_FrostDragon*)m_pOwner)->Set_Color(vColor);
}

_bool CState_Dragon_Weak_End::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Dragon_Weak_End* CState_Dragon_Weak_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Weak_End* pInstance = new CState_Dragon_Weak_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Weak_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Weak_End::Clone(void* pArg)
{
	CState_Dragon_Weak_End* pInstance = new CState_Dragon_Weak_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Weak_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Weak_End::Free()
{
	__super::Free();
}
