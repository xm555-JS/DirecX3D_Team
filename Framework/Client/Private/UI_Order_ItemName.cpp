#include "stdafx.h"
#include "..\Public\UI_Order_ItemName.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Order_ItemName::CUI_Order_ItemName()
	: CUI_Order()
{
}

CUI_Order_ItemName::CUI_Order_ItemName(const CUI_Order_ItemName & rhs)
	: CUI_Order(rhs)
{
}

HRESULT CUI_Order_ItemName::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Order_ItemName::Initialize(void * pArg)
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

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);
	return S_OK;
}

HRESULT CUI_Order_ItemName::Initialize_Pooling(void* pArg)
{
	m_fAlphaRatio = 1.f;
	m_fAccTime = 0.f;


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);
	return S_OK;
}

void CUI_Order_ItemName::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitX + 300.f, 0.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitX, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;

	if (0.55f < m_fAccTime)
	{
		m_fAccTime = 0.55f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicIn(vStartPoint, vTargetPoint, m_fAccTime, 0.55f));
	m_fX = vCurPoint.x;
	m_fAlphaRatio = vCurPoint.y;
}

void CUI_Order_ItemName::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Order_ItemName::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Order_ItemName::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ORDER_ITEMNAME), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Order_ItemName::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));
	return S_OK;
}

CUI_Order_ItemName * CUI_Order_ItemName::Create()
{
	CUI_Order_ItemName*		pInstance = new CUI_Order_ItemName();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Order_ItemName");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Order_ItemName::Clone(void * pArg)
{
	CUI_Order_ItemName*		pInstance = new CUI_Order_ItemName(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Order_ItemName");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Order_ItemName::Free()
{
	__super::Free();
}
