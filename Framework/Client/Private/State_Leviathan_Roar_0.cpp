#include "stdafx.h"
#include "..\Public\State_Leviathan_Roar_0.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

#include "Portal.h"
#include "Navigation.h"

CState_Leviathan_Roar_0::CState_Leviathan_Roar_0(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Leviathan(pManager, eState)
{
}

CState_Leviathan_Roar_0::CState_Leviathan_Roar_0(const CState_Leviathan_Roar_0& rhs)
	: CState_Leviathan(rhs)
{
}

HRESULT CState_Leviathan_Roar_0::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR);
	m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR] = 1;

	return S_OK;
}

HRESULT CState_Leviathan_Roar_0::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Roar_0::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 2.f;

	m_pTargetModel->Set_ModelEasingRatio(1.f);

	if (nullptr != m_pOwner->Get_Component(ID_UINT(NAVIGATION)))
		m_pNaviCom = static_cast<CNavigation*>(m_pOwner->Get_Component(ID_UINT(NAVIGATION)));

}

void CState_Leviathan_Roar_0::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//if(700.f < Get_CurKeyFrame())
	//	m_bEndCurState = true;
	if(190 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySound(TEXT("Leviathan_Roar.mp3"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 0.5f));

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	if (308 < Get_CurKeyFrame() && 600 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect1,
			pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 3.5f, 0.08f, 1.f, 0.03f);
			pGameInstance->Add_RadialCA(_float2(0.5f, 0.5f), _float4(0.f, 0.04f, 0.02f, 0.f), 3.5f, 0.1f);
		);

		// 플레이어 밀치기
		CGameObject* pPlayer = m_pState_Handler->Get_Player();
		_vector vPlayerPos = pPlayer->Get_Pos();
		_vector vPos = m_pOwner->Get_Pos();
		_vector vDir = XMVector3Normalize(vPlayerPos - vPos);
		_float	fMoveSpeed = 8.f;
		pPlayer->Get_Transform()->MoveTo(vPlayerPos + vDir * fMoveSpeed, m_pNaviCom);
		//pPlayer->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos * vDir * fTimeDelta * fMoveSpeed);

		if (500 < Get_CurKeyFrame() && 550 > Get_CurKeyFrame())
		{
			IMPL_ONCE(m_bPortal,
				CPortal::PORTALDESC PortalDesc;
				XMStoreFloat4(&PortalDesc.m_vPos, vPlayerPos);
				PortalDesc.m_vTargetPos = _float4(295.f, 0.f, 240.f, 1.f);
				PortalDesc.m_bUseAble = true;
				PortalDesc.m_ePortalType = PORTAL_LEVIATHAN;
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_PORTAL), ID_UINT(MAP, MAP_PORTAL), &PortalDesc);

			);
		}
	}

	_uint iMinFrame_1 = 618;
	_uint iMaxFrame_1 = 700;
	_float fMaxSpeed = 18.f;
	if (iMinFrame_1 < Get_CurKeyFrame() && iMaxFrame_1 > Get_CurKeyFrame())
	{
		m_fAnimSpeed = 1.f;
		_float fMoveSpeed = XMVectorGetX(CEasing_Utillity::sinfInOut(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(fMaxSpeed, 0.f, 0.f, 0.f), Get_CurKeyFrame() - iMinFrame_1, (iMaxFrame_1 - iMinFrame_1) * 0.5f));
		m_pOwnerTranformCom->Go_Front(fTimeDelta, fMoveSpeed);
		m_pOwnerTranformCom->Go_Up(fTimeDelta, fMoveSpeed);
	}


	CState_Boss::Tick_AnimationLerp(fTimeDelta, "Bone_ROOT", m_pOwnerTranformCom, XMMatrixRotationY(XMConvertToRadians(0.0f)));
}

void CState_Leviathan_Roar_0::Exit(void* pArg)
{
	__super::Exit(pArg);

	m_fAnimSpeed = 1.f;
	m_pTargetModel->Set_ModelEasingRatio(0.2f);

	//m_pOwnerTranformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-180.f));
}

_bool CState_Leviathan_Roar_0::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Roar_0* CState_Leviathan_Roar_0::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Roar_0* pInstance = new CState_Leviathan_Roar_0(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Roar_0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Roar_0::Clone(void* pArg)
{
	CState_Leviathan_Roar_0* pInstance = new CState_Leviathan_Roar_0(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Roar_0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Roar_0::Free()
{
	__super::Free();
}
