#include "stdafx.h"
#include "..\Public\UI_Inven_Icon.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "UI_Item.h"
#include "Item_Manager.h"

CUI_Inven_Icon::CUI_Inven_Icon()
	: CUI_Inventory()
{
}

CUI_Inven_Icon::CUI_Inven_Icon(const CUI_Inven_Icon & rhs)
	: CUI_Inventory(rhs)
{
}

HRESULT CUI_Inven_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Inven_Icon::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));

	// 0 배경  3 UV더하는거  20 21 22 23 24 25 인벤토리

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 10;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

HRESULT CUI_Inven_Icon::Initialize_Pooling(void* pArg)
{
	m_fAccTime = 0.f;
	m_fSizeRatioX = 0.f;
	m_fAlphaRatio = 0.f;
	// 20 배경 3 움직이는거 

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 10;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

void CUI_Inven_Icon::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Create_Inven_Window(fTimeDelta);
	m_fMoveUVSpeed -= fTimeDelta * 0.1f;
	if (3 == m_tUIDesc.iTextureIdx)
	{
		if(-0.04f > m_fMoveUVSpeed)
			m_iPassIdx = 11;
	}

	if (20 == m_tUIDesc.iTextureIdx)
	{
		USEGAMEINSTANCE;
		list<CGameObject*>* pGameObjectlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_ITEM));
		vector<CUI_Item*> vecItems;

		for (auto& pItem : *pGameObjectlist)
		{
			vecItems.push_back((CUI_Item*)pItem);
		}

		if (vecItems.size())
		{
			CItem_Manager::Get_Instance()->SetUp_ItemInven(vecItems);
		}
	}
}

void CUI_Inven_Icon::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Inven_Icon::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Inven_Icon::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_INVENTORY), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Inven_Icon::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	if (3 == m_tUIDesc.iTextureIdx)
	{
		_float2 vUVSpeed = { m_fMoveUVSpeed,0.f };
 		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vMoveUVSpeed", &vUVSpeed, sizeof(_float2)));
	}

	return S_OK;
}

CUI_Inven_Icon * CUI_Inven_Icon::Create()
{
	CUI_Inven_Icon*		pInstance = new CUI_Inven_Icon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Inven_Icon");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Inven_Icon::Clone(void * pArg)
{
	CUI_Inven_Icon*		pInstance = new CUI_Inven_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Inven_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inven_Icon::Free()
{
	__super::Free();
}
