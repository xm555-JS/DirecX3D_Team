#include "stdafx.h"
#include "..\Public\UI_Logo_Start.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Logo_Start::CUI_Logo_Start()
	: CUI()
{
}

CUI_Logo_Start::CUI_Logo_Start(const CUI_Logo_Start & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Logo_Start::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Logo_Start::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 4;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Logo_Start::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Resizing(fTimeDelta);	
}

void CUI_Logo_Start::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Logo_Start::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Logo_Start::Resizing(_float fTimeDelta)
{
	m_fAccTime += m_fDir * fTimeDelta;

	if (1.f < m_fAccTime)
	{
		m_fAccTime = 1.f;
		m_fDir *= -1.f;
	}
	else if (0.f > m_fAccTime)
	{
		m_fAccTime = 0.f;
		m_fDir *= -1.f;
	}

	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX * 1.08f, m_tUIDesc.fInitSizeY * 1.08f, 1.f, 1.f);
	_float4 vCurPoint;

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicIn(vStartPoint, vTargetPoint, m_fAccTime, 1.f));
	m_fSizeX = vCurPoint.x;
	m_fSizeY = vCurPoint.y;
}

HRESULT CUI_Logo_Start::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_LOGO), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Logo_Start::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Logo_Start * CUI_Logo_Start::Create()
{
	CUI_Logo_Start*		pInstance = new CUI_Logo_Start();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Logo_Start");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Logo_Start::Clone(void * pArg)
{
	CUI_Logo_Start*		pInstance = new CUI_Logo_Start(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Logo_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Logo_Start::Free()
{
	__super::Free();
}
