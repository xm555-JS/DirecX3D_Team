#include "stdafx.h"
#include "..\Public\State_Leviathan_Apear.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

#include "Easing_Utillity.h"
#include "Imgui_Manager.h"
#include "Camera.h"

CState_Leviathan_Apear::CState_Leviathan_Apear(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Leviathan(pManager, eState)
{
}

CState_Leviathan_Apear::CState_Leviathan_Apear(const CState_Leviathan_Apear& rhs)
	: CState_Leviathan(rhs)
{
}

HRESULT CState_Leviathan_Apear::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_ROAR_0);
	m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_ROAR_0] = 1;

	return S_OK;
}

HRESULT CState_Leviathan_Apear::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Apear::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_pTargetModel->Set_ModelEasingRatio(1.f);
	m_fAccTime = 0.f;
	m_fAnimSpeed = 1.f;

}

void CState_Leviathan_Apear::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;


	if (300 < Get_CurKeyFrame())
	{
		m_bEndCurState = true;
	}


	_float fMoveLegthY;
	_uint iMinFrame = 100;
	_uint iMaxFrame = 180;
	_float fMovePower = 3.f;

	//if (KEY_DOWN(VK_SPACE))
	//{
	//	cout << "Check!" << endl;
	//}
	//cout << "CamPos: " << pGameInstance->Get_CamPosition().x << ", " << pGameInstance->Get_CamPosition().y << ", " << pGameInstance->Get_CamPosition().z << " //// CamLook ";
	//COUT_VECTOR_VECTOR(pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[3], pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]);

	if (iMinFrame < Get_CurKeyFrame() && iMaxFrame > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffetCam0,
			//cout << "Start!!!!!!!!!!!!!!!!" << endl;
		pGameInstance->Play_ActionCamera(TEXT("Leviathan_CameraAction"), 1.f);
		);
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySound(TEXT("Leviathan_Apear.mp3"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 0.5f));

		fMoveLegthY = XMVectorGetX(CEasing_Utillity::sinfOut(XMVectorSet(80.f, 0.f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), Get_CurKeyFrame() - iMinFrame, iMaxFrame - iMinFrame));
		m_pOwnerTranformCom->Go_Up(fTimeDelta, fMoveLegthY * fMovePower);
	}
	if (iMaxFrame < Get_CurKeyFrame())
	{
		//cout << "END!!!!!!!!!!!!!!!!" << endl;
	}
	//else if (200 < Get_CurKeyFrame() && 230 > Get_CurKeyFrame())
	//{
	//	m_fAccTime += fTimeDelta;
	//	fMoveLegthY = XMVectorGetX(CEasing_Utillity::SinIn(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(100.f, 0.f, 0.f, 0.f), m_fAccTime, 1.f));
	//	m_pOwnerTranformCom->Go_Up(fTimeDelta, fMoveLegthY);
	//}


	//CState_Boss::Tick_AnimationFix(fTimeDelta, "Bip001", m_pOwnerTranformCom);
	//CState_Boss::Tick_AnimationFix(fTimeDelta, "Bip001", m_pOwnerTranformCom);
	CState_Boss::Tick_AnimationLerp(fTimeDelta, "Bone_ROOT", m_pOwnerTranformCom, XMMatrixRotationY(XMConvertToRadians(180.0f)));
	//CState_AI::Tick(fTimeDelta);
}

void CState_Leviathan_Apear::Exit(void* pArg)
{
	__super::Exit(pArg);

	m_fAnimSpeed = 1.f;
	m_pTargetModel->Set_ModelEasingRatio(0.2f);

	//m_pOwnerTranformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-180.f));
}

_bool CState_Leviathan_Apear::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Apear* CState_Leviathan_Apear::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Apear* pInstance = new CState_Leviathan_Apear(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Apear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Apear::Clone(void* pArg)
{
	CState_Leviathan_Apear* pInstance = new CState_Leviathan_Apear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Apear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Apear::Free()
{
	__super::Free();
}
