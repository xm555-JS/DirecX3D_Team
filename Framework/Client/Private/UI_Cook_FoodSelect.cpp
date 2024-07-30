#include "stdafx.h"
#include "..\Public\UI_Cook_FoodSelect.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"



CUI_Cook_FoodSelect::CUI_Cook_FoodSelect()
	: CUI_Cook_WIndow()
{
}

CUI_Cook_FoodSelect::CUI_Cook_FoodSelect(const CUI_Cook_FoodSelect & rhs)
	: CUI_Cook_WIndow(rhs)
{
}

HRESULT CUI_Cook_FoodSelect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Cook_FoodSelect::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));

	USEGAMEINSTANCE;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;


	SetUp_UIpos();

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

HRESULT CUI_Cook_FoodSelect::Initialize_Pooling(void* pArg)
{
	m_fAlphaTime = 0.f;
	m_fAccTime = 0.f;

	USEGAMEINSTANCE;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;


	SetUp_UIpos();


	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

void CUI_Cook_FoodSelect::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Select_Icon_Anim(fTimeDelta);
}

void CUI_Cook_FoodSelect::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Cook_FoodSelect::Render()
{ 
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Cook_FoodSelect::Select_Icon_Anim(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta * m_fDir;

	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX + 10.f, 1.f, 1.f, 1.f);

	_float4 vCurPoint;

	if (0.5f < m_fAccTime)
	{
		m_fAccTime = 0.5f;
		m_fDir *= -1.f;
	}
	if (0.f > m_fAccTime)
	{
		m_fAccTime = 0.f;
		m_fDir *= -1.f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));
	m_fSizeY = m_fSizeX = vCurPoint.x;
}

HRESULT CUI_Cook_FoodSelect::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_COOK_ICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Cook_FoodSelect::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Cook_FoodSelect * CUI_Cook_FoodSelect::Create()
{
	CUI_Cook_FoodSelect*		pInstance = new CUI_Cook_FoodSelect();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Cook_FoodSelect");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Cook_FoodSelect::Clone(void * pArg)
{
	CUI_Cook_FoodSelect*		pInstance = new CUI_Cook_FoodSelect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Cook_FoodSelect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Cook_FoodSelect::Free()
{
	__super::Free();
}
