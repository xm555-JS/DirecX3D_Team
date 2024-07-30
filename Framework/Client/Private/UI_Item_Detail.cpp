#include "stdafx.h"
#include "..\Public\UI_Item_Detail.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Item_Detail::CUI_Item_Detail()
	: CUI_Inventory()
{
}

CUI_Item_Detail::CUI_Item_Detail(const CUI_Item_Detail & rhs)
	: CUI_Inventory(rhs)
{
}

HRESULT CUI_Item_Detail::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Item_Detail::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;
	m_fAlphaRatio = 1.f;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

HRESULT CUI_Item_Detail::Initialize_Pooling(void* pArg)
{
	m_fAccTime = 0.f;
	m_fSizeRatioX = 0.f;
	m_fAlphaRatio = 0.f;
	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;
	m_fAlphaRatio = 1.f;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

void CUI_Item_Detail::Tick(_float fTimeDelta)
{	
    CUI::Tick(fTimeDelta);
	if (0 == m_tUIDesc.iTextureIdx)
	{
		m_fAlphaRatio = 0.6f;
	}
	else
	{
		m_fAccTime += fTimeDelta;
		_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(1.f, 0.f, 1.f, 1.f);

		if (0.2f < m_fAccTime)
		{
			m_fAccTime = 0.2f;
		}

		_float4 vCurPoint;
		XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAccTime, 0.2f));
		m_fAlphaRatio = vCurPoint.x;
	}
}

void CUI_Item_Detail::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Item_Detail::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Item_Detail::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ITEM_DETAIL), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Item_Detail::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Item_Detail * CUI_Item_Detail::Create()
{
	CUI_Item_Detail*		pInstance = new CUI_Item_Detail();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Item_Detail");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Item_Detail::Clone(void * pArg)
{
	CUI_Item_Detail*		pInstance = new CUI_Item_Detail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Item_Detail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Item_Detail::Free()
{
	__super::Free();
}
