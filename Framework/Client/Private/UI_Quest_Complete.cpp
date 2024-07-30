#include "stdafx.h"
#include "..\Public\UI_Quest_Complete.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

CUI_Quest_Complete::CUI_Quest_Complete()
	: CUI_Quest()
{
}

CUI_Quest_Complete::CUI_Quest_Complete(const CUI_Quest_Complete & rhs)
	: CUI_Quest(rhs)
{
}

HRESULT CUI_Quest_Complete::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Quest_Complete::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;
	m_bMoveWithFont = true;


	SetUp_UIpos();

	GAMEINSTANCE->Add_TickGroup(TG_UI, this);
	GAMEINSTANCE->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Quest_Complete::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	m_fAccTime += fTimeDelta * m_fTimeDir;

	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitX + 300.f, 0.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitX, 1.f, 1.f, 1.f);

	_float4 vCurPoint;
	if (0.8f < m_fAccTime)
	{
		m_fAccTime = 0.8f;
	}
	if (0.f > m_fAccTime)
	{
		m_fAccTime = 0.f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fAccTime, 0.8f));
	
	m_fX = vCurPoint.x;
}

void CUI_Quest_Complete::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Quest_Complete::Render()
{
	FAILEDCHECK(CUI::Render());
	return S_OK;
}

HRESULT CUI_Quest_Complete::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_QUEST_COMPLETE), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Quest_Complete::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Quest_Complete * CUI_Quest_Complete::Create()
{
	CUI_Quest_Complete*		pInstance = new CUI_Quest_Complete();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Quest_Complete");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Quest_Complete::Clone(void * pArg)
{
	CUI_Quest_Complete*		pInstance = new CUI_Quest_Complete(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Quest_Complete");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Quest_Complete::Free()
{
	__super::Free();
}
