#include "stdafx.h"
#include "..\Public\UI_Guide_Hand_Anim.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Guide_Hand_Anim::CUI_Guide_Hand_Anim()
	: CUI()
{
}

CUI_Guide_Hand_Anim::CUI_Guide_Hand_Anim(const CUI_Guide_Hand_Anim & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Guide_Hand_Anim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Guide_Hand_Anim::Initialize(void * pArg)
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

HRESULT CUI_Guide_Hand_Anim::Initialize_Pooling(void* pArg)
{
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

void CUI_Guide_Hand_Anim::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Move_Frame(0.3f, fTimeDelta);

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

void CUI_Guide_Hand_Anim::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Guide_Hand_Anim::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Guide_Hand_Anim::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_HAND_ANIM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Guide_Hand_Anim::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Guide_Hand_Anim * CUI_Guide_Hand_Anim::Create()
{
	CUI_Guide_Hand_Anim*		pInstance = new CUI_Guide_Hand_Anim();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Guide_Hand_Anim");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Guide_Hand_Anim::Clone(void * pArg)
{
	CUI_Guide_Hand_Anim*		pInstance = new CUI_Guide_Hand_Anim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Guide_Hand_Anim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Guide_Hand_Anim::Free()
{
	__super::Free();
}
