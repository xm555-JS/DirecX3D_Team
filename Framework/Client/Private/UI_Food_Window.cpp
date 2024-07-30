#include "stdafx.h"
#include "..\Public\UI_Food_Window.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Item_Manager.h"

CUI_Food_Window::CUI_Food_Window()
	: CUI()
{
}

CUI_Food_Window::CUI_Food_Window(const CUI_Food_Window & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Food_Window::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Food_Window::Initialize(void * pArg)
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

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Food_Window::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Create_FoodInven_Window(fTimeDelta);
}

void CUI_Food_Window::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Food_Window::Render()
{
	FAILEDCHECK(CUI::Render());
	if (0 == m_tUIDesc.iTextureIdx && 0.4f < m_fAccTime)
	{
		USEGAMEINSTANCE;
		pGameInstance->Render_Font(TEXT("Font_Hancomdodum_Bold"), TEXT("간편 사용"), m_tUIDesc.vFontPos, XMLoadFloat4(&m_tUIDesc.vFontColor), m_tUIDesc.fFontSize);
	}
	return S_OK;
}

void CUI_Food_Window::Create_FoodInven_Window(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(0.001f, 0.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeY, 1.f, 1.f, 1.f);
	_float4 vCurPoint;
	
	m_fAccTime += fTimeDelta;
	if (0.5f < m_fAccTime)
	{
		m_fAccTime = 0.5f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::sinfInOut(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));
	m_fSizeY = vCurPoint.x;
	m_fAlphaRatio = vCurPoint.y;
}

HRESULT CUI_Food_Window::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_FOOD_INVEN), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Food_Window::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Food_Window * CUI_Food_Window::Create()
{
	CUI_Food_Window*		pInstance = new CUI_Food_Window();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Food_Window");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Food_Window::Clone(void * pArg)
{
	CUI_Food_Window*		pInstance = new CUI_Food_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Food_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Food_Window::Free()
{
	__super::Free();
}
