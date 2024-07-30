#include "stdafx.h"
#include "..\Public\UI_Minimap_Arrow.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "Level_GamePlay.h"
#include "Camera.h"


CUI_Minimap_Arrow::CUI_Minimap_Arrow()
	: CUI()
{
}

CUI_Minimap_Arrow::CUI_Minimap_Arrow(const CUI_Minimap_Arrow & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Minimap_Arrow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Minimap_Arrow::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;
	
	USEGAMEINSTANCE;

	m_pPlayer = (CPlayer*)PLAYERPTR;
	//m_pCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();

	SetUp_UIpos();


	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Minimap_Arrow::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	if (2 == m_tUIDesc.iTextureIdx)
	{
		SetUp_PlayerArrow();
	}
	if (3 == m_tUIDesc.iTextureIdx)
	{
		SetUp_CameraArrow();
	}
}

void CUI_Minimap_Arrow::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Minimap_Arrow::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Minimap_Arrow::SetUp_PlayerArrow()
{
	if (!m_pPlayer)
		return;

	CTransform* pPlayerTransform = (CTransform*)m_pPlayer->Get_Component(TRANSFORM);
	_vector vPlayerLook = pPlayerTransform->Get_State(CTransform::STATE_LOOK);

	m_tUIDesc.fAngle = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(XMVectorSet(0.f,0.f,1.f,0.f), vPlayerLook)));

	m_tUIDesc.fAngle *= m_fAngleDir;

	_float4 vLook;
	XMStoreFloat4(&vLook, vPlayerLook);

	if (0.f < vLook.x)
	{
		m_fAngleDir = -1.f;
	}
	else if (0.f > vLook.x)
	{
		m_fAngleDir = 1.f;
	}
}

void CUI_Minimap_Arrow::SetUp_CameraArrow()
{
	_matrix ViewMatrixInv;
	ViewMatrixInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW)));

	_vector vCameraLook = ViewMatrixInv.r[2];

	m_tUIDesc.fAngle = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(XMVectorSet(0.f, 0.f, 1.f, 0.f), vCameraLook)));

	m_tUIDesc.fAngle *= m_fAngleDir;

	_float4 vLook;
	XMStoreFloat4(&vLook, vCameraLook);

	if (0.f < vLook.x)
	{
		m_fAngleDir = -1.f;
	}
	else if (0.f > vLook.x)
	{
		m_fAngleDir = 1.f;
	}

}

HRESULT CUI_Minimap_Arrow::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_MINIMAP), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Minimap_Arrow::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Minimap_Arrow * CUI_Minimap_Arrow::Create()
{
	CUI_Minimap_Arrow*		pInstance = new CUI_Minimap_Arrow();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Minimap_Arrow");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Minimap_Arrow::Clone(void * pArg)
{
	CUI_Minimap_Arrow*		pInstance = new CUI_Minimap_Arrow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Minimap_Arrow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minimap_Arrow::Free()
{
	__super::Free();
}
