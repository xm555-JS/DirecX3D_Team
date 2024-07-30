#include "stdafx.h"
#include "..\Public\UI_Enemy_Rank.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Unit.h"

CUI_Enemy_Rank::CUI_Enemy_Rank()
	: CUI_Solid()
{
}

CUI_Enemy_Rank::CUI_Enemy_Rank(const CUI_Enemy_Rank & rhs)
	: CUI_Solid(rhs)
{
}

HRESULT CUI_Enemy_Rank::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Enemy_Rank::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());

	if (nullptr != pArg)
	{
		memcpy(&m_tUISolidDesc, pArg, sizeof(UISolidDesc));
	}

	m_pTransformCom->Set_Scaled(_float3(3.f, 3.f, 1.f));


	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Enemy_Rank::Tick(_float fTimeDelta)
{	
	SetUp_Pos();
	Billboard();
	if (m_tUISolidDesc.pGameObject)
	{
		if ((CUnit*)m_tUISolidDesc.pGameObject->Is_Dead())
		{
			GAMEINSTANCE->Add_DeadObject(this);
		}
	}
}

void CUI_Enemy_Rank::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Enemy_Rank::Render()
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

void CUI_Enemy_Rank::On_EventMessege(EVENTDESC tEvent)
{

}

void CUI_Enemy_Rank::SetUp_Pos()
{
	if (m_tUISolidDesc.pGameObject)
	{
		_float4x4	ViewMatrixInv;
		XMStoreFloat4x4(&ViewMatrixInv, XMMatrixInverse(nullptr, XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))));
		_vector vCamLook = XMLoadFloat4((_float4*)&ViewMatrixInv.m[2][0]);

		CTransform* pTransform = m_tUISolidDesc.pGameObject->Get_Transform();
		_vector vPos = pTransform->Get_State(CTransform::STATE_TRANSLATION);
		_vector vUp = pTransform->Get_State(CTransform::STATE_UP);
		_vector vRight = pTransform->Get_State(CTransform::STATE_RIGHT);
		
		m_vCurScale = ((CUI_Solid*)m_tUISolidDesc.pGameObject)->Get_CurScale();

		vPos -= XMVector3Normalize(vUp) * m_vCurScale.x * 0.02;
		vPos -= XMVector3Normalize(vRight) * m_vCurScale.y / 0.115 ;
		vPos -= XMVector3Normalize(vCamLook) * 0.01f;
		
		m_vCurScale.x *= 0.7f;
		m_vCurScale.y = m_vCurScale.x / 2.f;


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

		if (((CUI_Solid*)m_tUISolidDesc.pGameObject)->Get_bRender())
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

HRESULT CUI_Enemy_Rank::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ENEMY_RANK), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Enemy_Rank::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI_Solid::SetUp_ShaderResource());

	if (1 == m_tUISolidDesc.iTextureIdx)
	{
		_bool	bGlow = true;
		_float	fGlowPower = 0.9f;
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));
	}

	return S_OK;
}

CUI_Enemy_Rank * CUI_Enemy_Rank::Create()
{
	CUI_Enemy_Rank*		pInstance = new CUI_Enemy_Rank();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Enemy_Rank");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Enemy_Rank::Clone(void * pArg)
{
	CUI_Enemy_Rank*		pInstance = new CUI_Enemy_Rank(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Enemy_Rank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Enemy_Rank::Free()
{
	USEGAMEINSTANCE;
	__super::Free();
}
