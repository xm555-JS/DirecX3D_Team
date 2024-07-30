#include "stdafx.h"
#include "..\Public\UI_Quest_MArk.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

CUI_Quest_Mark::CUI_Quest_Mark()
	: CUI_Quest()
{
}

CUI_Quest_Mark::CUI_Quest_Mark(const CUI_Quest_Mark & rhs)
	: CUI_Quest(rhs)
{
}

HRESULT CUI_Quest_Mark::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Quest_Mark::Initialize(void * pArg)
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

void CUI_Quest_Mark::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	
	// 위치 테스트 할때
	// m_fX = DEBUGFLOAT4(0).x;
}

void CUI_Quest_Mark::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Quest_Mark::Render()
{
	FAILEDCHECK(CUI::Render());
	GAMEINSTANCE->Render_Font(TEXT("Font_Hancomdodum_Bold"), m_strDefault_FontText.c_str(), m_tUIDesc.vFontPos, XMLoadFloat4(&m_tUIDesc.vFontColor), m_tUIDesc.fFontSize);
	return S_OK;
}

HRESULT CUI_Quest_Mark::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_QUEST_MARK), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Quest_Mark::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Quest_Mark * CUI_Quest_Mark::Create()
{
	CUI_Quest_Mark*		pInstance = new CUI_Quest_Mark();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Quest_Mark");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Quest_Mark::Clone(void * pArg)
{
	CUI_Quest_Mark*		pInstance = new CUI_Quest_Mark(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Quest_Mark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Quest_Mark::Free()
{
	__super::Free();
}
