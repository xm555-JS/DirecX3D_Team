#include "stdafx.h"
#include "..\Public\UI_Order_Item.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Order_Item::CUI_Order_Item()
	: CUI_Order()
{
}

CUI_Order_Item::CUI_Order_Item(const CUI_Order_Item & rhs)
	: CUI_Order(rhs)
{
}

HRESULT CUI_Order_Item::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Order_Item::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 17;


	SetUp_UIpos();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);
	return S_OK;
}

HRESULT CUI_Order_Item::Initialize_Pooling(void* pArg)
{
	m_fAlphaTime = 0.f;
	m_fSizeTime = 0.f;
	m_fMoveTime = 0.f;
	m_fColorTime = 0.f;
	m_fColorRatio = 1.f;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 17;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);
	return S_OK;
}

void CUI_Order_Item::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	
	USEGAMEINSTANCE;
	_vector vStartPoint = XMVectorSet(0.3f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	m_fColorTime += fTimeDelta;

	if (1.7f < m_fColorTime)
	{
		m_fColorTime = 1.7f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicIn(vStartPoint, vTargetPoint, m_fColorTime, 1.7f));
	m_fColorRatio = vCurPoint.x;
}

void CUI_Order_Item::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Order_Item::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Order_Item::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ORDER_ITEM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Order_Item::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fBright", &m_fColorRatio, sizeof(_float)));
	return S_OK;
}

CUI_Order_Item * CUI_Order_Item::Create()
{
	CUI_Order_Item*		pInstance = new CUI_Order_Item();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Order_Item");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Order_Item::Clone(void * pArg)
{
	CUI_Order_Item*		pInstance = new CUI_Order_Item(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Order_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Order_Item::Free()
{
	__super::Free();
}
