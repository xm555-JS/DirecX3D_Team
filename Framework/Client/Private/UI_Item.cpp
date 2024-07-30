#include "stdafx.h"
#include "..\Public\UI_Item.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "UI_Item_Num.h"
#include "Item_Manager.h"

CUI_Item::CUI_Item()
	: CUI_Inventory()
{
}

CUI_Item::CUI_Item(const CUI_Item & rhs)
	: CUI_Inventory(rhs)
{
}

HRESULT CUI_Item::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Item::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 10;


	SetUp_UIpos();
	m_SelectItemlist.clear();
	USEGAMEINSTANCE;
	Create_ItemNum();
	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

HRESULT CUI_Item::Initialize_Pooling(void* pArg)
{
	m_fAccTime = 0.f;
	m_fSizeRatioX = 0.f;
	m_fAlphaRatio = 0.f;
	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 10;


	SetUp_UIpos();
	m_SelectItemlist.clear();
	USEGAMEINSTANCE;
	Create_ItemNum();
	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

void CUI_Item::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	SetUp_ItemNum();
	if (0.4f > m_fAccTime)
	{
		Create_Inven_Window(fTimeDelta);
	}
	else
	{
		Select_Item(fTimeDelta);
	}
}

void CUI_Item::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Item::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Item::Create_ItemDetail()
{
	USEGAMEINSTANCE;
	CUI* pUI = nullptr;
	pUI = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ITEM_DETAIL), ID_UINT(UI, UI_ITEM_DETAIL),
		&CUI::UIDESC(m_fX, m_fY, 85.f, 85.f, 0.f, 19, 50));
	m_SelectItemlist.push_back(pUI);
	pUI = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ITEM_DETAIL), ID_UINT(UI, UI_ITEM_DETAIL),
		&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 0, 49));
	m_SelectItemlist.push_back(pUI);
	pUI = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ITEM_DETAIL), ID_UINT(UI, UI_ITEM_DETAIL),
		&CUI::UIDESC(640.f, 330.f, 390.f, 495.f, 0.f, m_tUIDesc.iTextureIdx, 48, { 0.f,0.f }, nullptr, 0, { 710.f,156 }, { 0.12f,0.12f,0.12f,1.f },0.35f));
	pUI->Set_FontNum(CItem_Manager::Get_Instance()->Find_ItemNum((ITEM_TYPE)(m_tUIDesc.iTextureIdx)));
	pUI->Set_bRenderFontNum(true);
	m_SelectItemlist.push_back(pUI);
	pUI = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ITEM_DETAIL), ID_UINT(UI, UI_ITEM_DETAIL),
		&CUI::UIDESC(640.f, 330.f, 390.f, 495.f, 0.f, 20, 50));
	m_SelectItemlist.push_back(pUI);
}

void CUI_Item::Create_ItemNum()
{
	USEGAMEINSTANCE;
	m_pTensNum = (CUI_Item_Num*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ITEM_NUM), ID_UINT(UI, UI_ITEM_NUM)
		, &UIDESC(m_tUIDesc.fInitX + 11.f, m_tUIDesc.fInitY + 24.f, 33.f, 38.f, 0.f, 0, 99));
	m_pUnitsNum = (CUI_Item_Num*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ITEM_NUM), ID_UINT(UI, UI_ITEM_NUM)
		, &UIDESC(m_tUIDesc.fInitX + 25.f, m_tUIDesc.fInitY + 24.f, 33.f, 38.f, 0.f, 0, 99));
}

void CUI_Item::SetUp_ItemNum()
{
	if (0 < m_tUIDesc.iTextureIdx)
	{
		m_iItemNum = CItem_Manager::Get_Instance()->Find_ItemNum((ITEM_TYPE)(m_tUIDesc.iTextureIdx));
	}
	if (0 == m_iItemNum)
	{
		m_tUIDesc.iTextureIdx = 0;
	}

	if (m_pUnitsNum && m_pTensNum)
	{
		m_pTensNum->Set_iTextureIdx(m_iItemNum / 10);
		m_pUnitsNum->Set_iTextureIdx(m_iItemNum % 10);
		if (0 == m_iItemNum / 10)
		{
			m_pTensNum->Set_Render(false);
			if (1 == m_iItemNum % 10)
			{
				m_pUnitsNum->Set_Render(false);
			}
			else
			{
				m_pUnitsNum->Set_Render(true);
			}
		}
		else
		{
			m_pUnitsNum->Set_Render(true);
			m_pTensNum->Set_Render(true);
		}
	}
	if (0 == m_tUIDesc.iTextureIdx)
	{
		m_pTensNum->Set_Render(false);
		m_pUnitsNum->Set_Render(false);
	}
}

void CUI_Item::Select_Item(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	RECT rcSelect;
	SetRect(&rcSelect, m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f
		, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f);

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX + 10.f, m_tUIDesc.fInitSizeY + 10.f, 1.f, 1.f);
	_float4 vCurPoint;

	if (PtInRect(&rcSelect, ptMouse))
	{
		if (0 != m_tUIDesc.iTextureIdx)
			m_fSizeTime += fTimeDelta;
		if (0.2f < m_fSizeTime)
		{
			m_fSizeTime = 0.2f;
		}
		if (KEY_DOWN(VK_LBUTTON))
		{
			MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
			list<CGameObject*>* pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_ITEM_DETAIL));

			if (!pGameObjects)
			{
				Create_ItemDetail();
			}
			else if (!m_SelectItemlist.size() && 0 != m_tUIDesc.iTextureIdx && !pGameObjects->size())
			{
				Create_ItemDetail();
			}
			else if (0 == m_tUIDesc.iTextureIdx)
			{
				for (auto& pSelectItem : m_SelectItemlist)
				{
					pGameInstance->Add_PoolingObject(pSelectItem);
				}
				m_SelectItemlist.clear();
			}
		}

	}
	else
	{
		m_fSizeTime = 0.f;

		if (KEY_DOWN(VK_LBUTTON))
		{
			if (m_SelectItemlist.size())
			{
				for (auto& pSelectItem : m_SelectItemlist)
				{
					pGameInstance->Add_DeadObject(pSelectItem);
				}
				m_SelectItemlist.clear();
			}
		}
	}
	XMStoreFloat4(&vCurPoint, CEasing_Utillity::sinfInOut(vStartPoint, vTargetPoint, m_fSizeTime, 0.2f));
	m_fSizeX = m_fSizeY = vCurPoint.x;
}

HRESULT CUI_Item::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ITEM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Item::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Item * CUI_Item::Create()
{
	CUI_Item*		pInstance = new CUI_Item();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Item");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Item::Clone(void * pArg)
{
	CUI_Item*		pInstance = new CUI_Item(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Item::Free()
{
	__super::Free();
}
