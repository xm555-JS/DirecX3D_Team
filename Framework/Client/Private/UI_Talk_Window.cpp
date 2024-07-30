#include "stdafx.h"
#include "..\Public\UI_Talk_Window.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Talk_Window::CUI_Talk_Window()
	: CUI()
{
}

CUI_Talk_Window::CUI_Talk_Window(const CUI_Talk_Window & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Talk_Window::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Talk_Window::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;
	m_fBarSpeed = fRandom(0.2f, 0.7f);

	SetUp_UIpos();

	USEGAMEINSTANCE;

	if (5 == m_tUIDesc.iTextureIdx)
	{
		m_fSizeY = 0.01f;
	}

	pGameInstance->Add_TickGroup(TG_UI_TALK, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_TALK, this);
	return S_OK;
}

void CUI_Talk_Window::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	if (3 == m_tUIDesc.iTextureIdx)
	{
		Talk_Arrow_Animation(fTimeDelta);
	}
	if (5 == m_tUIDesc.iTextureIdx)
	{
		_vector vStartPoint = XMVectorSet(0.01f, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeY, 1.f, 1.f, 1.f);
		_float4 vCurPoint;

		m_fAccTime += fTimeDelta * m_fDir;

		if (m_fBarSpeed < m_fAccTime)
		{
			m_fAccTime = m_fBarSpeed;
			m_fDir = -1.f;
		}
		if (0.f > m_fAccTime)
		{
			m_fAccTime = 0.f;
			m_fDir = 1.f;
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::CircularIn(vStartPoint, vTargetPoint, m_fAccTime, m_fBarSpeed));
		m_fSizeY = vCurPoint.x;
	}
}

void CUI_Talk_Window::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Talk_Window::Render()
{
	FAILEDCHECK(CUI::Render());
	if (3 > m_tUIDesc.iTextureIdx)
	{
		GAMEINSTANCE->Render_Font(TEXT("Font_Hancomdodum_Bold"), m_strDefault_FontText.c_str(), m_tUIDesc.vFontPos, XMLoadFloat4(&m_tUIDesc.vFontColor), m_tUIDesc.fFontSize);
	}
	return S_OK;
}

HRESULT CUI_Talk_Window::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_TALK_WINDOW), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Talk_Window::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

void CUI_Talk_Window::Talk_Arrow_Animation(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitY, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitY + 10.f, 1.f, 1.f, 1.f);

	_float4 vCurPoint;

	m_fAccTime += fTimeDelta * m_fDir;
	if (0.4f < m_fAccTime)
	{
		m_fAccTime = 0.4f;
		m_fDir *= -1.f;
	}
	if (0.f > m_fAccTime)
	{
		m_fAccTime = 0.f;
		m_fDir *= -1.f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicIn(vStartPoint, vTargetPoint, m_fAccTime, 0.4f));
	m_fY = vCurPoint.x;
}

CUI_Talk_Window * CUI_Talk_Window::Create()
{
	CUI_Talk_Window*		pInstance = new CUI_Talk_Window();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Talk_Window");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Talk_Window::Clone(void * pArg)
{
	CUI_Talk_Window*		pInstance = new CUI_Talk_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Talk_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Talk_Window::Free()
{
	__super::Free();
}
