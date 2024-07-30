#include "stdafx.h"
#include "..\Public\UI_Inven_Button.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "GameObject.h"

CUI_Inven_Button::CUI_Inven_Button()
	: CUI_Inventory()
{
}

CUI_Inven_Button::CUI_Inven_Button(const CUI_Inven_Button & rhs)
	: CUI_Inventory(rhs)
{
}

HRESULT CUI_Inven_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Inven_Button::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 10;
	m_iInvenButton = m_tUIDesc.iTextureIdx;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

HRESULT CUI_Inven_Button::Initialize_Pooling(void* pArg)
{
	m_fAccTime = 0.f;
	m_fSizeRatioX = 0.f;
	m_fAlphaRatio = 0.f;
	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 10;
	m_iInvenButton = m_tUIDesc.iTextureIdx;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

void CUI_Inven_Button::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Interact_Button(fTimeDelta);
	Create_Inven_Window(fTimeDelta);
}

void CUI_Inven_Button::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Inven_Button::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Inven_Button::Interact_Button(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	RECT rcSelect;
	SetRect(&rcSelect, m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f
		, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f);

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rcSelect, ptMouse))
	{
		if (KEY_DOWN(VK_LBUTTON))
		{
			switch (m_iInvenButton)
			{
			case INVENB_EXIT:
				Pooling_Inven_Objects();
				pGameInstance->Able_TickGroup(TG_UI);
				pGameInstance->Able_LateTickGroup(LTG_UI);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Close.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
				break;
			}
		}
	}

}

void CUI_Inven_Button::Pooling_Inven_Objects()
{
	USEGAMEINSTANCE;
	list<CGameObject*>* pGameObjects = nullptr;

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_INVEN_ICON));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_INVEN_BUTTON));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_ITEM));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_ITEM_NUM));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_LOADING_SCREEN), ID_UINT(UI, UI_LOADING_SCREEN),
		&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 16, 5, { 0.f,0.f }, this));
}

HRESULT CUI_Inven_Button::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_INVENTORY), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Inven_Button::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Inven_Button * CUI_Inven_Button::Create()
{
	CUI_Inven_Button*		pInstance = new CUI_Inven_Button();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Inven_Button");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Inven_Button::Clone(void * pArg)
{
	CUI_Inven_Button*		pInstance = new CUI_Inven_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Inven_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inven_Button::Free()
{
	__super::Free();
}
