#include "stdafx.h"
#include "..\Public\UI_Item_Num.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Item_Num::CUI_Item_Num()
	: CUI_Inventory()
{
}

CUI_Item_Num::CUI_Item_Num(const CUI_Item_Num & rhs)
	: CUI_Inventory(rhs)
{
}

HRESULT CUI_Item_Num::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Item_Num::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 10;
	////юс╫ц
	//m_fAlphaRatio = 1.f;
	//m_fSizeRatioX = 1.f;
	//m_bRender = true;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

HRESULT CUI_Item_Num::Initialize_Pooling(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 10;
	m_fAccTime = 0.f;
	m_fSizeRatioX = 0.f; 
	m_bRender = false;
	m_fAlphaRatio = 0.f;
	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

void CUI_Item_Num::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Create_Inven_Window(fTimeDelta);
}

void CUI_Item_Num::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Item_Num::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI::Render());
	}
	return S_OK;
}

HRESULT CUI_Item_Num::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ITEM_NUM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Item_Num::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Item_Num * CUI_Item_Num::Create()
{
	CUI_Item_Num*		pInstance = new CUI_Item_Num();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Item_Num");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Item_Num::Clone(void * pArg)
{
	CUI_Item_Num*		pInstance = new CUI_Item_Num(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Item_Num");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Item_Num::Free()
{
	__super::Free();
}
