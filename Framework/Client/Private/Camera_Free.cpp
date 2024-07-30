#include "stdafx.h"
#include "..\Public\Camera_Free.h"
#include "GameInstance.h"

#ifdef USE_IMGUI
#include "Imgui_Manager.h"
#include "Window_Imgui.h"
#include "Window_Tool_Effect.h"
#endif

CCamera_Free::CCamera_Free()
	: CCamera()
{
}

CCamera_Free::CCamera_Free(const CCamera_Free& rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		__debugbreak();

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	/* 트랜스폼 컴포넌늩를 추가한다. */
	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	m_fMoveSpeed = 20.f;
	m_fRotateSpeed = XMConvertToRadians(90.f);

	return S_OK;
}

void CCamera_Free::Tick_Camera(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_pTransformCom->Go_Straight(fTimeDelta, m_fMoveSpeed);
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
	{
		m_pTransformCom->Go_Backward(fTimeDelta, m_fMoveSpeed);
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransformCom->Go_Left(fTimeDelta, m_fMoveSpeed);
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransformCom->Go_Right(fTimeDelta, m_fMoveSpeed);
	}

#ifdef USE_IMGUI
	if (CImgui_Manager::Get_Instance()->Get_Window(CImgui_Manager::TOOL_MAP)->Is_Enable() ||
		static_cast<CWindow_Tool_Effect*>(CImgui_Manager::Get_Instance()->Get_Window(CImgui_Manager::TOOL_EFFECT))->IsCameraLock() ||
		CImgui_Manager::Get_Instance()->Get_Window(CImgui_Manager::TOOL_ANIMATION)->Is_Enable())
	{
		m_bIsMouseCanMove = false;
	}
	else
	{
		m_bIsMouseCanMove = true;
	}
#else
	m_bIsMouseCanMove = true;
#endif
	if (m_bIsMouseCanMove && !KEY_TOGGLE(VK_HOME))
	{
		_long		MouseMove = 0;
		if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta, m_fRotateSpeed * MouseMove * 0.5f);
		}

		if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta, m_fRotateSpeed * MouseMove * 0.5f);
		}
	}

	Safe_Release(pGameInstance);

	if (FAILED(Bind_PipeLine()))
		return;
	/*_float4 vPos;
	XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	cout << vPos.x << '\t'<< vPos.z << endl;*/
}

void CCamera_Free::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

CCamera_Free* CCamera_Free::Create()
{
	CCamera_Free* pInstance = new CCamera_Free();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();
}
