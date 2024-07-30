#include "stdafx.h"
#include "..\Public\UI_PlaceName.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Minigame_Manager.h"
#include "Quest_Manager.h"

CUI_PlaceName::CUI_PlaceName()
	: CUI()
{
}

CUI_PlaceName::CUI_PlaceName(const CUI_PlaceName & rhs)
	: CUI(rhs)
{
}

void CUI_PlaceName::Appear_PlaceName(REGION_TYPE eRegionType)
{
	m_fAccTime = 0.f;
	m_fDir = 1.f;

	switch (eRegionType)
	{
	case REGION_VILLAGE:
		m_tUIDesc.iTextureIdx = 0;
		break;

	case REGION_DRAGON:
		m_tUIDesc.iTextureIdx = 1;
		break;

	case REGION_ROBELLA:
		m_tUIDesc.iTextureIdx = 2;
		break;

	case REGION_WEAVER:
		m_tUIDesc.iTextureIdx = 5;
		if (QUEST_KILL_MUTATION == MGR(CQuest_Manager)->Get_CurQuset())
		{
			MGR(CMinigame_Manager)->Notice_Minigame();
		}
		break;

	default:
		break;
	}

}

HRESULT CUI_PlaceName::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_PlaceName::Initialize(void * pArg)
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

	pGameInstance->Add_TickGroup(TG_UI_PLACENAME, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_PLACENAME, this);
	return S_OK;
}

void CUI_PlaceName::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	m_fAccTime += fTimeDelta * m_fDir;

	if (1.3f < m_fAccTime)
	{
		m_fAccTime = 1.3f;
		m_fDir *= -1.f;
	}
	if (0.f > m_fAccTime)
	{
		m_fAccTime = 0.f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::QuarticOut(vStartPoint, vTargetPoint, m_fAccTime, 1.3f));
	m_fAlphaRatio = vCurPoint.x;

	/*USEGAMEINSTANCE;
	if (KEY_DOWN('T'))
	{
		m_fAccTime = 0.f;
		m_fDir = 1.f;
	}*/
}

void CUI_PlaceName::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_PlaceName::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_PlaceName::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_PLACENAME), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_PlaceName::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_PlaceName * CUI_PlaceName::Create()
{
	CUI_PlaceName*		pInstance = new CUI_PlaceName();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_PlaceName");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_PlaceName::Clone(void * pArg)
{
	CUI_PlaceName*		pInstance = new CUI_PlaceName(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_PlaceName");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlaceName::Free()
{
	__super::Free();
}
