#include "stdafx.h"
#include "..\Public\UI_Enemy_Name.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "UI_Enemy_Hp_Bar.h"
#include "Imgui_Manager.h"


CUI_Enemy_Name::CUI_Enemy_Name()
	: CUI_Solid()
{
}

CUI_Enemy_Name::CUI_Enemy_Name(const CUI_Enemy_Name & rhs)
	: CUI_Solid(rhs)
{
}

HRESULT CUI_Enemy_Name::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Enemy_Name::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());

	if (nullptr != pArg)
	{
		memcpy(&m_tUISolidDesc, pArg, sizeof(UISolidDesc));
	}

	//m_pTransformCom->Set_Scaled(_float3(1.5f, 0.2f, 1.f));

	m_iPassIdx = 0;


	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Enemy_Name::Tick(_float fTimeDelta)
{		
	SetUp_Pos();
	Billboard();
	if (m_tUISolidDesc.pGameObject)
	{
		if (m_tUISolidDesc.pGameObject->Is_Dead())
		{
			GAMEINSTANCE->Add_DeadObject(this);
		}
	}
}

void CUI_Enemy_Name::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UISOLID, this);
}

HRESULT CUI_Enemy_Name::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI_Solid::Render());

		_bool	bGlow = false;
		_float	fGlowPower = 0.f;
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));
	}
	return S_OK;
}

void CUI_Enemy_Name::SetUp_Pos()
{
	if (m_tUISolidDesc.pGameObject)
	{
		CTransform* pTransform = m_tUISolidDesc.pGameObject->Get_Transform();
		_vector vPos = pTransform->Get_State(CTransform::STATE_TRANSLATION);
		_vector vUp = pTransform->Get_State(CTransform::STATE_UP);

		m_vCurScale = ((CUI_Solid*)m_tUISolidDesc.pGameObject)->Get_CurScale();
		m_vCurScale.x *= 1.3f;
		m_vCurScale.y = m_vCurScale.x / 7.6f;

		vPos += XMVector3Normalize(vUp) * m_vCurScale.x * 0.132;

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

		_float fMaxDist = 15.f;
		if (15.f > ((CUI_Enemy_Hp_Bar*)m_tUISolidDesc.pGameObject)->Get_Dist())
		{
			m_bRender = true;
		}
		else
		{
			m_bRender = false;
		}
		m_pTransformCom->Set_Scaled(m_vCurScale);
	}
}

HRESULT CUI_Enemy_Name::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ENEMY_NAME), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Enemy_Name::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI_Solid::SetUp_ShaderResource());
	_bool	bGlow = true;
	_float	fGlowPower = 0.5f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));

	return S_OK;
}

CUI_Enemy_Name * CUI_Enemy_Name::Create()
{
	CUI_Enemy_Name*		pInstance = new CUI_Enemy_Name();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Enemy_Name");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Enemy_Name::Clone(void * pArg)
{
	CUI_Enemy_Name*		pInstance = new CUI_Enemy_Name(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Enemy_Name");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Enemy_Name::Free()
{
	__super::Free();
}
