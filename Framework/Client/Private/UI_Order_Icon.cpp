#include "stdafx.h"
#include "..\Public\UI_Order_Icon.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Item_Manager.h"

CUI_Order_Icon::CUI_Order_Icon()
	: CUI_Order()
{
}

CUI_Order_Icon::CUI_Order_Icon(const CUI_Order_Icon & rhs)
	: CUI_Order(rhs)
{
}

HRESULT CUI_Order_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Order_Icon::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;
	if (11 == m_tUIDesc.iTextureIdx)
	{
		m_iPassIdx = 11;
	}

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

HRESULT CUI_Order_Icon::Initialize_Pooling(void* pArg)
{
	m_fAlphaTime = 0.f;
	m_fSizeTime = 0.f;
	m_fMoveTime = 0.f;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;
	if (11 == m_tUIDesc.iTextureIdx)
	{
		m_iPassIdx = 11;
	}

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

void CUI_Order_Icon::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	SetUp_BlackCoreNum();
	Icon_Anim(fTimeDelta);
	
	if (7 == m_tUIDesc.iTextureIdx
		|| 12 == m_tUIDesc.iTextureIdx
		|| 13 == m_tUIDesc.iTextureIdx)
	{
		Create_Change_Pos(fTimeDelta);
	}
	if (8 == m_tUIDesc.iTextureIdx
		|| 10 == m_tUIDesc.iTextureIdx)
	{
		Create_Change_Alpha(fTimeDelta);
		Create_Change_Size(fTimeDelta);
	}

	if (1 == m_tUIDesc.iTextureIdx
		|| 3 == m_tUIDesc.iTextureIdx
		|| 9 == m_tUIDesc.iTextureIdx
		|| 15 == m_tUIDesc.iTextureIdx)
	{
		Create_Change_Alpha(fTimeDelta);
	}
}

void CUI_Order_Icon::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Order_Icon::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Order_Icon::SetUp_BlackCoreNum()
{
	if (1 == m_tUIDesc.iTextureIdx)
	{
		m_iFontNum = CItem_Manager::Get_Instance()->Find_ItemNum(ITEM_BLACKCORE);
		if (100 <= m_iFontNum)
		{
			m_tUIDesc.vFontPos.x = 1205.f;
		}
		else if (10 <= m_iFontNum)
		{
			m_tUIDesc.vFontPos.x = 1216.f;
		}
		else
		{
			m_tUIDesc.vFontPos.x = 1227.f;
		}
	}
}

void CUI_Order_Icon::Icon_Anim(_float fTimeDelta)
{
	if (8 == m_tUIDesc.iTextureIdx && 0.6f <= m_fAlphaTime)
	{
		_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitY, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitY + 40.f, 1.f, 1.f, 1.f);
		_float4 vCurPoint;

		m_fMoveTime += fTimeDelta * m_fDir;

		if (1.3f < m_fMoveTime)
		{
			m_fMoveTime = 1.3f;
			m_fDir *= -1.f;
		}
		if (0.f > m_fMoveTime)
		{
			m_fMoveTime = 0.f;
			m_fDir *= -1.f;
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::QuadIn(vStartPoint, vTargetPoint, m_fMoveTime, 1.3f));
		m_fY = vCurPoint.x;
	}

	if (11 == m_tUIDesc.iTextureIdx)
	{
		m_fMoveUVSpeed += fTimeDelta * 0.02f;
	}
}

HRESULT CUI_Order_Icon::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ORDER_ICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Order_Icon::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));
	_float2 vUVSpeed = {m_fMoveUVSpeed , 0.f};
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vMoveUVSpeed", &vUVSpeed, sizeof(_float2)));
	return S_OK;
}

CUI_Order_Icon * CUI_Order_Icon::Create()
{
	CUI_Order_Icon*		pInstance = new CUI_Order_Icon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Order_Icon");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Order_Icon::Clone(void * pArg)
{
	CUI_Order_Icon*		pInstance = new CUI_Order_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Order_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Order_Icon::Free()
{
	__super::Free();
}
