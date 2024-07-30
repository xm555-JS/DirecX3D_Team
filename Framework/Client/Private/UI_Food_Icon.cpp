#include "stdafx.h"
#include "..\Public\UI_Food_Icon.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Item_Manager.h"
#include "UI_CurFood_Icon.h"
#include "UI_Food_Num.h"

CUI_Food_Icon::CUI_Food_Icon()
	: CUI()
{
}

CUI_Food_Icon::CUI_Food_Icon(const CUI_Food_Icon & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Food_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Food_Icon::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;

	SetUp_UIpos();
	Create_FoodNum();
	USEGAMEINSTANCE;

	m_tUIDesc.iTextureIdx = 8;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Food_Icon::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	Resize_FoodIcon(fTimeDelta);
	SetUp_CurFood_Icon();	
	SetUp_FoodNum();
}

void CUI_Food_Icon::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);

}

HRESULT CUI_Food_Icon::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Food_Icon::Resize_FoodIcon(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(0.001f, 0.f, m_tUIDesc.fInitY + 40.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeY, 1.f, m_tUIDesc.fInitY, 1.f);
	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;
	if (0.5f < m_fAccTime)
	{
		m_fAccTime = 0.5f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::ExpoIn(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));
	m_fSizeY = vCurPoint.x;
	m_fAlphaRatio = vCurPoint.y;
	m_fY = vCurPoint.z;
}

void CUI_Food_Icon::Create_FoodNum()
{
	USEGAMEINSTANCE;
	m_pTensNum = (CUI_Food_Num*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_FOOD_NUM), ID_UINT(UI, UI_FOOD_NUM)
		, &UIDESC(m_tUIDesc.fInitX + 12.f, m_tUIDesc.fInitY + 23.f, 33.f, 38.f, 0.f, 0, 630));
	m_pUnitsNum = (CUI_Food_Num*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_FOOD_NUM), ID_UINT(UI, UI_FOOD_NUM)
		, &UIDESC(m_tUIDesc.fInitX + 26.f, m_tUIDesc.fInitY + 23.f, 33.f, 38.f, 0.f, 0, 630));
}

void CUI_Food_Icon::SetUp_FoodNum()
{
	if (19 > m_tUIDesc.iTextureIdx)
	{
		m_iFoodNum = CItem_Manager::Get_Instance()->Find_ItemNum((ITEM_TYPE)(m_tUIDesc.iTextureIdx));
	}
	if (0 == m_iFoodNum)
	{
		m_tUIDesc.iTextureIdx = 19;
	}

	if (m_pUnitsNum && m_pTensNum)
	{
		m_pTensNum->Set_iTextureIdx(m_iFoodNum / 10);
		m_pUnitsNum->Set_iTextureIdx(m_iFoodNum % 10);
		if (0 == m_iFoodNum / 10)
		{
			m_pTensNum->Set_Render(false);
			if (1 == m_iFoodNum % 10)
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
	if (19 == m_tUIDesc.iTextureIdx)
	{
		m_pTensNum->Set_Render(false);
		m_pUnitsNum->Set_Render(false);
	}
}

void CUI_Food_Icon::SetUp_CurFood_Icon()
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
			CUI_CurFood_Icon* pCurFood = (CUI_CurFood_Icon*)pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_CURFOOD_ICON));
			if (pCurFood)
			{
				pCurFood->Set_iTextureIdx(m_tUIDesc.iTextureIdx - 3);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
			}
		}
	}
}

HRESULT CUI_Food_Icon::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ITEM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Food_Icon::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Food_Icon * CUI_Food_Icon::Create()
{
	CUI_Food_Icon*		pInstance = new CUI_Food_Icon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Food_Icon");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Food_Icon::Clone(void * pArg)
{
	CUI_Food_Icon*		pInstance = new CUI_Food_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Food_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Food_Icon::Free()
{
	__super::Free();
}
