#include "stdafx.h"
#include "..\Public\UI_Guide_Contents.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Guide_Contents::CUI_Guide_Contents()
	: CUI()
{
}

CUI_Guide_Contents::CUI_Guide_Contents(const CUI_Guide_Contents & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Guide_Contents::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Guide_Contents::Initialize(void * pArg)
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

void CUI_Guide_Contents::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	SetUp_Bright(fTimeDelta);

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

void CUI_Guide_Contents::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Guide_Contents::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Guide_Contents::SetUp_Bright(_float fTimeDelta)
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

HRESULT CUI_Guide_Contents::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_WINDOW), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Guide_Contents::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fBright", &m_fBright, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Guide_Contents * CUI_Guide_Contents::Create()
{
	CUI_Guide_Contents*		pInstance = new CUI_Guide_Contents();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Guide_Contents");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Guide_Contents::Clone(void * pArg)
{
	CUI_Guide_Contents*		pInstance = new CUI_Guide_Contents(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Guide_Contents");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Guide_Contents::Free()
{
	__super::Free();
}
