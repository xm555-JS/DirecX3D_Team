#include "stdafx.h"
#include "..\Public\UI_Cook_Icon.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "Level_GamePlay.h"
#include "Camera_Target_Position.h"


CUI_Cook_Icon::CUI_Cook_Icon()
	: CUI_Cook_WIndow()
{
}

CUI_Cook_Icon::CUI_Cook_Icon(const CUI_Cook_Icon & rhs)
	: CUI_Cook_WIndow(rhs)
{
}

HRESULT CUI_Cook_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Cook_Icon::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));

	USEGAMEINSTANCE;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;


	SetUp_UIpos();

	

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

HRESULT CUI_Cook_Icon::Initialize_Pooling(void* pArg)
{
	m_fAlphaTime = 0.f;
	m_fAccTime = 0.f;

	USEGAMEINSTANCE;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;


	SetUp_UIpos();

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

void CUI_Cook_Icon::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	if (1 == m_tUIDesc.iTextureIdx || 2 == m_tUIDesc.iTextureIdx)
	{
		Create_Window_Edge(fTimeDelta);
	}
	if (3 == m_tUIDesc.iTextureIdx)
	{
		Create_Foodlist_Window(fTimeDelta);
	}
	if (4 == m_tUIDesc.iTextureIdx || 8 == m_tUIDesc.iTextureIdx)
	{
		Create_Recipe_Window(fTimeDelta);
	}
	if (7 == m_tUIDesc.iTextureIdx)
	{
		m_fAlphaRatio = 0.5f;
	}
}

void CUI_Cook_Icon::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Cook_Icon::Render()
{ 
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Cook_Icon::Create_Window_Edge(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	_vector vStartPoint;
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitY, 1.f, 1.f, 1.f);

	if (1 == m_tUIDesc.iTextureIdx)
	{
		vStartPoint = XMVectorSet(375.f, 1.f, 1.f, 1.f);
	}
	else if (2 == m_tUIDesc.iTextureIdx)
	{
		vStartPoint = XMVectorSet(335.f, 1.f, 1.f, 1.f);
	}
	_float4 vCurPoint;

	if (0.3f < m_fAccTime)
	{
		m_fAccTime = 0.3f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CircularIn(vStartPoint, vTargetPoint, m_fAccTime, 0.3f));
	m_fY = vCurPoint.x;
}

void CUI_Cook_Icon::Create_Foodlist_Window(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	_vector vStartPoint = XMVectorSet(5.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeY, 1.f, 1.f, 1.f);

	_float4 vCurPoint;

	if (0.3f < m_fAccTime)
	{
		m_fAccTime = 0.3f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CircularIn(vStartPoint, vTargetPoint, m_fAccTime, 0.3f));
	m_fSizeY = vCurPoint.x;
}

void CUI_Cook_Icon::Create_Recipe_Window(_float fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;

	// FoodlistWindow 생기고 Alpha값 높히기
	_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);

	_float4 vCurPoint;

	if (0.5f < m_fAlphaTime)
	{
		m_fAlphaTime = 0.5f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::ExpoIn(vStartPoint, vTargetPoint, m_fAlphaTime, 0.5f));
	m_fAlphaRatio = vCurPoint.x;
}

HRESULT CUI_Cook_Icon::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_COOK_ICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Cook_Icon::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Cook_Icon * CUI_Cook_Icon::Create()
{
	CUI_Cook_Icon*		pInstance = new CUI_Cook_Icon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Cook_Icon");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Cook_Icon::Clone(void * pArg)
{
	CUI_Cook_Icon*		pInstance = new CUI_Cook_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Cook_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Cook_Icon::Free()
{
	__super::Free();
}
