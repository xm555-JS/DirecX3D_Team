#include "stdafx.h"
#include "..\Public\UI_Fin.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Fin::CUI_Fin()
	: CUI()
{
}

CUI_Fin::CUI_Fin(const CUI_Fin & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Fin::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Fin::Initialize(void * pArg)
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

void CUI_Fin::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	m_fAccTime += fTimeDelta;

	_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	if (2.f < m_fAccTime)
	{
		m_fAccTime = 2.f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAccTime, 2.f));
	m_fAlphaRatio = vCurPoint.x;
}

void CUI_Fin::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Fin::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Fin::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_SHORTKEY), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Fin::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Fin * CUI_Fin::Create()
{
	CUI_Fin*		pInstance = new CUI_Fin();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Fin");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Fin::Clone(void * pArg)
{
	CUI_Fin*		pInstance = new CUI_Fin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Fin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Fin::Free()
{
	__super::Free();
}
