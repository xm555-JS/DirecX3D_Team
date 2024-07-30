#include "stdafx.h"
#include "..\Public\UI_Food_Num.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Food_Num::CUI_Food_Num()
	: CUI()
{
}

CUI_Food_Num::CUI_Food_Num(const CUI_Food_Num & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Food_Num::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Food_Num::Initialize(void * pArg)
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

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Food_Num::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Resize_FoodNum(fTimeDelta);
}

void CUI_Food_Num::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Food_Num::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI::Render());
	}
	return S_OK;
}

void CUI_Food_Num::Resize_FoodNum(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(0.001f, 0.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeY, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;
	if (0.5f < m_fAccTime)
	{
		m_fAccTime = 0.5f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::ExpoIn(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));
	m_fSizeY = vCurPoint.x;
	m_fAlphaRatio = vCurPoint.y;
}

HRESULT CUI_Food_Num::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ITEM_NUM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Food_Num::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Food_Num * CUI_Food_Num::Create()
{
	CUI_Food_Num*		pInstance = new CUI_Food_Num();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Food_Num");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Food_Num::Clone(void * pArg)
{
	CUI_Food_Num*		pInstance = new CUI_Food_Num(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Food_Num");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Food_Num::Free()
{
	__super::Free();
}
