#include "stdafx.h"
#include "..\Public\UI_Guide_Window.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Guide_Window::CUI_Guide_Window()
	: CUI()
{
}

CUI_Guide_Window::CUI_Guide_Window(const CUI_Guide_Window & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Guide_Window::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Guide_Window::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 14;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

HRESULT CUI_Guide_Window::Initialize_Pooling(void* pArg)
{

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 14;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Guide_Window::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	SetUp_Bright(fTimeDelta);
	SetUp_Pos(fTimeDelta);
}

void CUI_Guide_Window::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Guide_Window::Render()
{
	FAILEDCHECK(CUI::Render());
	_float fBright = 1.f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fBright", &fBright, sizeof(_float)));
	if (0 == m_tUIDesc.iTextureIdx)
	{
		GAMEINSTANCE->Render_Font(TEXT("Font_Hancomdodum_Bold"), m_strDefault_FontText.c_str(), m_tUIDesc.vFontPos, XMLoadFloat4(&m_tUIDesc.vFontColor), m_tUIDesc.fFontSize);
	}
	return S_OK;
}

void CUI_Guide_Window::SetUp_Bright(_float fTimeDelta)
{
	if (0 == m_tUIDesc.iTextureIdx)
	{
		m_fAccTime += m_fDir * fTimeDelta;
		if (0.5f < m_fAccTime)
		{
			m_fDir *= -1.f;
			m_fAccTime = 0.5f;
		}
		if (0.f > m_fAccTime)
		{
			m_fDir *= -1.f;
			m_fAccTime = 0.f;
		}

		_vector vStartPoint = XMVectorSet(1.2f, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(1.7f, 1.f, 1.f, 1.f);

		_float4 vCurPoint;

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));
		m_fBright = vCurPoint.x;
	}
}

void CUI_Guide_Window::SetUp_Pos(_float fTimeDelta)
{
	m_fInitTime += fTimeDelta;
	if (0.3f < m_fInitTime)
	{
		m_fInitTime = 0.3f;
	}
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitY + 50.f, 0.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitY, 1.f, 1.f, 1.f);

	_float4 vCurPoint;
	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fInitTime, 0.3f));
	m_fY = vCurPoint.x;
	m_fAlphaRatio = vCurPoint.y;
}

HRESULT CUI_Guide_Window::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_WINDOW), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Guide_Window::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fBright", &m_fBright, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Guide_Window * CUI_Guide_Window::Create()
{
	CUI_Guide_Window*		pInstance = new CUI_Guide_Window();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Guide_Window");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Guide_Window::Clone(void * pArg)
{
	CUI_Guide_Window*		pInstance = new CUI_Guide_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Guide_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Guide_Window::Free()
{
	__super::Free();
}
