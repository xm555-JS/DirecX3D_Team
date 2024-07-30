#include "stdafx.h"
#include "..\Public\UI_Alert_Window.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Alert_Window::CUI_Alert_Window()
	: CUI()
{
}

CUI_Alert_Window::CUI_Alert_Window(const CUI_Alert_Window & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Alert_Window::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Alert_Window::Initialize(void * pArg)
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
	m_bMoveWithFont = true;

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_UI_ALERT, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ALERT, this);
	return S_OK;
}

void CUI_Alert_Window::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Appear_Alert_Window(fTimeDelta);
	Disappear_Alert_Window(fTimeDelta);
	m_tUIDesc.vFontColor.w = m_fAlphaRatio;
}

void CUI_Alert_Window::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Alert_Window::Render()
{
	FAILEDCHECK(CUI::Render());
	//_uint i = 0;
	//wchar_t szTemp[MAX_PATH] = L"";
	//swprintf_s(szTemp, TEXT("%d"), i);

	//wstring strTemp = wstring(szTemp);
	GAMEINSTANCE->Render_Font(TEXT("Font_Hancomdodum_Bold"), m_strDefault_FontText.c_str(), m_tUIDesc.vFontPos, XMLoadFloat4(&m_tUIDesc.vFontColor), m_tUIDesc.fFontSize);

	return S_OK;
}

void CUI_Alert_Window::Appear_Alert_Window(_float fTimeDelta)
{
	if (!m_bAppear)
	{
		m_fAppearTime += fTimeDelta;
		_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitX + 100.f, 0.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitX, 1.f, 1.f, 1.f);

		_float4 vCurPoint;
		if (0.2f < m_fAppearTime)
		{
			m_fAppearTime = 0.2f;
			m_bAppear = true;
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::QuinticOut(vStartPoint, vTargetPoint, m_fAppearTime, 0.2f));
		m_fX = vCurPoint.x;
		m_fAlphaRatio = vCurPoint.y;
	}
}

void CUI_Alert_Window::Disappear_Alert_Window(_float fTimeDelta)
{
	if (m_bAppear)
	{
		m_fAppearTime += fTimeDelta;
		_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitY, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitY - 60.f, 0.f, 1.f, 1.f);

		_float4 vCurPoint;
		if (0.5f < m_fDisappearTime)
		{
			m_fDisappearTime = 0.5f;
			GAMEINSTANCE->Add_DeadObject(this);
		}
		if (0.5f < m_fAppearTime)
		{
			m_fDisappearTime += fTimeDelta;
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fDisappearTime, 0.5f));
		m_fY = vCurPoint.x;
		m_fAlphaRatio = vCurPoint.y;
	}
}

HRESULT CUI_Alert_Window::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ALERT_WINDOW), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Alert_Window::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Alert_Window * CUI_Alert_Window::Create()
{
	CUI_Alert_Window*		pInstance = new CUI_Alert_Window();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Alert_Window");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Alert_Window::Clone(void * pArg)
{
	CUI_Alert_Window*		pInstance = new CUI_Alert_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Alert_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Alert_Window::Free()
{
	__super::Free();
}
