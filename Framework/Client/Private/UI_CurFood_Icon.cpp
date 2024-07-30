#include "stdafx.h"
#include "..\Public\UI_CurFood_Icon.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Item_Manager.h"
#include "UI_CurFood_Num.h"

CUI_CurFood_Icon::CUI_CurFood_Icon()
	: CUI()
{
}

CUI_CurFood_Icon::CUI_CurFood_Icon(const CUI_CurFood_Icon & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_CurFood_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_CurFood_Icon::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;


	SetUp_UIpos();
	Create_CurFoodNum();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_CurFood_Icon::Tick(_float fTimeDelta)
{
	CUI::Tick(fTimeDelta);
	SetUp_CurIcon();
	CItem_Manager::Get_Instance()->Set_CurItem((ITEM_TYPE)(m_tUIDesc.iTextureIdx + 3));
	SetUp_CurFoodNum();
}

void CUI_CurFood_Icon::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_CurFood_Icon::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_CurFood_Icon::SetUp_CurIcon()
{
	//if (16 != m_tUIDesc.iTextureIdx)
	//{
	//	m_tUIDesc.iTextureIdx = 16;
	//}
	if (0 == CItem_Manager::Get_Instance()->Find_ItemNum((ITEM_TYPE)(m_tUIDesc.iTextureIdx + 3)))
	{
		for (_uint i = ITEM_MUSHROOM; i < ITEM_END; ++i)
		{
			if (0 < CItem_Manager::Get_Instance()->Find_ItemNum((ITEM_TYPE)(i)))
			{
				CItem_Manager::Get_Instance()->Set_CurItem((ITEM_TYPE)(i));
				m_tUIDesc.iTextureIdx = i - 3;
				return;
			}
		}
		m_tUIDesc.iTextureIdx = 16;
	}
}

void CUI_CurFood_Icon::SetUp_CurFoodNum()
{
	if (16 > m_tUIDesc.iTextureIdx)
	{
		m_iFoodNum = CItem_Manager::Get_Instance()->Find_ItemNum((ITEM_TYPE)(m_tUIDesc.iTextureIdx + 3));
	}
	if (0 == m_iFoodNum)
	{
		m_tUIDesc.iTextureIdx = 16;
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
	if (16 == m_tUIDesc.iTextureIdx)
	{
		m_pTensNum->Set_Render(false);
		m_pUnitsNum->Set_Render(false);
	}
}

void CUI_CurFood_Icon::Create_CurFoodNum()
{
	USEGAMEINSTANCE;
	m_pTensNum = (CUI_CurFood_Num*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_CURFOOD_NUM), ID_UINT(UI, UI_CURFOOD_NUM)
		, &UIDESC(m_tUIDesc.fInitX + 11.f, m_tUIDesc.fInitY + 21.f, 25.f, 29.f, 0.f, 0, 500));
	m_pUnitsNum = (CUI_CurFood_Num*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_CURFOOD_NUM), ID_UINT(UI, UI_CURFOOD_NUM)
		, &UIDESC(m_tUIDesc.fInitX + 23.f, m_tUIDesc.fInitY + 21.f, 25.f, 29.f, 0.f, 0, 500));
}

HRESULT CUI_CurFood_Icon::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_FOOD_ICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_CurFood_Icon::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_CurFood_Icon * CUI_CurFood_Icon::Create()
{
	CUI_CurFood_Icon*		pInstance = new CUI_CurFood_Icon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_CurFood_Icon");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_CurFood_Icon::Clone(void * pArg)
{
	CUI_CurFood_Icon*		pInstance = new CUI_CurFood_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_CurFood_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_CurFood_Icon::Free()
{
	__super::Free();
}
