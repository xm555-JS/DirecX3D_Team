#include "stdafx.h"
#include "..\Public\UI_Order_TenResult.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Order_Manager.h"
#include "Item_Manager.h"

CUI_Order_TenResult::CUI_Order_TenResult()
	: CUI_Order()
{
}

CUI_Order_TenResult::CUI_Order_TenResult(const CUI_Order_TenResult & rhs)
	: CUI_Order(rhs)
{
}

HRESULT CUI_Order_TenResult::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Order_TenResult::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;

	if (ORDER_2GUN == m_tUIDesc.iTextureIdx && COrder_Manager::Get_Instance()->Get_Own_2Gun())
	{
		m_bOverlap = true;
		CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE,5);
	}
	else if (ORDER_CUBE == m_tUIDesc.iTextureIdx && COrder_Manager::Get_Instance()->Get_Own_Cube())
	{
		m_bOverlap = true;
		CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, 5);
	}
	else if (ORDER_SHIELDAXE == m_tUIDesc.iTextureIdx && COrder_Manager::Get_Instance()->Get_Own_ShieldAxe())
	{
		m_bOverlap = true;
		CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, 5);
	}

	SetUp_UIpos();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);

	return S_OK;
}

HRESULT CUI_Order_TenResult::Initialize_Pooling(void* pArg)
{
	m_fAlphaTime = 0.f;
	m_fSizeTime = 0.f;
	m_fAlphaRatio = 1.f;
	m_fWaitTime = 0.f;
	m_fAccTime = 0.f;
	m_bRender = false;
	m_bOverlap = false;
	RESET_ONCE(m_bCreateButton);
	RESET_ONCE(m_bCreateConverse);

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;

	if (ORDER_2GUN == m_tUIDesc.iTextureIdx && COrder_Manager::Get_Instance()->Get_Own_2Gun())
	{
		m_bOverlap = true;
		CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, 5);
	}
	else if (ORDER_CUBE == m_tUIDesc.iTextureIdx && COrder_Manager::Get_Instance()->Get_Own_Cube())
	{
		m_bOverlap = true;
		CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, 5);
	}
	else if (ORDER_SHIELDAXE == m_tUIDesc.iTextureIdx && COrder_Manager::Get_Instance()->Get_Own_ShieldAxe())
	{
		m_bOverlap = true;
		CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, 5);
	}

	SetUp_UIpos();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);	

	return S_OK;
}

void CUI_Order_TenResult::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	m_fWaitTime += fTimeDelta;

	if (m_tUIDesc.iIdx * 0.1f < m_fWaitTime)
	{
		m_bRender = true;
		_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX * 0.05f, m_tUIDesc.fInitSizeY * 0.01f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 1.f, 1.f);

		_float4 vCurPoint;

		m_fAccTime += fTimeDelta;
		if (0.4f < m_fAccTime)
		{
			USEGAMEINSTANCE;
			m_fAccTime = 0.4f;
			IMPL_ONCE(m_bCreateConverse,
				//MGR(CSound_Manager)->PlaySound(TEXT("UI_Order_TenResult.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
				if (m_bOverlap)
				{
					pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
						&CUI::UIDESC(m_fX, m_fY + 100.f, 90.f, 50.f, 0.f, 6, 300));
					pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
						&CUI::UIDESC(m_fX, m_fY + 160.f, 60.f, 60.f, 0.f, 8, 300));
				});

			if (ORDER_2GUN == m_tUIDesc.iTextureIdx)
			{
				COrder_Manager::Get_Instance()->Set_Own_2Gun(true);
			}
			else if (ORDER_CUBE == m_tUIDesc.iTextureIdx)
			{
				COrder_Manager::Get_Instance()->Set_Own_Cube(true);
			}
			else if (ORDER_SHIELDAXE == m_tUIDesc.iTextureIdx)
			{
				COrder_Manager::Get_Instance()->Set_Own_ShieldAxe(true);
			}

			if (9 == m_tUIDesc.iIdx)
			{
				IMPL_ONCE(m_bCreateButton, 
					Load_UIData("Order_ResultButton"));
			}
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::ExpoOut(vStartPoint, vTargetPoint, m_fAccTime, 0.4f));

		m_fSizeX = vCurPoint.x;
		m_fSizeY = vCurPoint.y;
	}
}

void CUI_Order_TenResult::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Order_TenResult::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI::Render());
	}

	return S_OK;
}

HRESULT CUI_Order_TenResult::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ORDER_TENRESULT), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Order_TenResult::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));
	return S_OK;
}

CUI_Order_TenResult * CUI_Order_TenResult::Create()
{
	CUI_Order_TenResult*		pInstance = new CUI_Order_TenResult();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Order_TenResult");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Order_TenResult::Clone(void * pArg)
{
	CUI_Order_TenResult*		pInstance = new CUI_Order_TenResult(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Order_TenResult");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Order_TenResult::Free()
{
	__super::Free();
}
