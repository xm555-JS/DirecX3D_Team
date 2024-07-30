#include "stdafx.h"
#include "..\Public\UI_Enemy_Indicator.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "Imgui_Manager.h"

CUI_Enemy_Indicator::CUI_Enemy_Indicator()
	: CUI_Solid()
{
}

CUI_Enemy_Indicator::CUI_Enemy_Indicator(const CUI_Enemy_Indicator & rhs)
	: CUI_Solid(rhs)
{
}

HRESULT CUI_Enemy_Indicator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Enemy_Indicator::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());

	if (nullptr != pArg)
	{
		memcpy(&m_tUISolidDesc, pArg, sizeof(UISolidDesc));
	}

	m_iPassIdx = 12;

	m_pTransformCom->Set_Scaled(_float3(1.5f, 1.5f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Enemy_Indicator::Tick(_float fTimeDelta)
{	
	SetUp_Indicator_Dir();
	if (m_tUISolidDesc.pGameObject)
	{
		m_bRender = GAMEINSTANCE->isIn_Frustum_InWorldSpace(m_tUISolidDesc.pGameObject->Get_Pos());

		if (m_tUISolidDesc.pGameObject->Is_Dead())
		{
			GAMEINSTANCE->Add_DeadObject(this);
		}
	}
}

void CUI_Enemy_Indicator::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UISOLID, this);
}

HRESULT CUI_Enemy_Indicator::Render()
{
	if (!m_bRender)
	{
		FAILEDCHECK(CUI_Solid::Render());
		_bool	bGlow = false;
		_float	fGlowPower = 1.f;
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
	}
	return S_OK;
}

void CUI_Enemy_Indicator::On_EventMessege(EVENTDESC tEvent)
{

}

void CUI_Enemy_Indicator::SetUp_Indicator_Dir()
{
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	if (pPlayer && m_tUISolidDesc.pGameObject)
	{
		_float4 vPlayerPos;
		XMStoreFloat4(&vPlayerPos, pPlayer->Get_Pos());
		vPlayerPos.y += 0.4f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&vPlayerPos));
		vPlayerPos.y = 0.f;

		_float4 vEnemyPos;
		XMStoreFloat4(&vEnemyPos, m_tUISolidDesc.pGameObject->Get_Pos());
		vEnemyPos.y = 0.f;

		_vector vUp = XMVector3Normalize(XMLoadFloat4(&vEnemyPos) - XMLoadFloat4(&vPlayerPos)) * m_pTransformCom->Get_Scaled().y;
		_vector vRight = XMVector3Normalize(XMVector3Cross(XMVector3Normalize(vUp), XMVectorSet(0.f, -1.f, 0.f, 0.f))) * m_pTransformCom->Get_Scaled().x;

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
	}
}

HRESULT CUI_Enemy_Indicator::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ENEMY_INDICATOR), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Enemy_Indicator::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI_Solid::SetUp_ShaderResource());
	_float fAlphaRatio = 1.f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &fAlphaRatio, sizeof(_float)));
	
	
	_bool	bGlow = true;
	_float	fGlowPower = 0.5f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));
	return S_OK;
}

CUI_Enemy_Indicator * CUI_Enemy_Indicator::Create()
{
	CUI_Enemy_Indicator*		pInstance = new CUI_Enemy_Indicator();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Enemy_Indicator");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Enemy_Indicator::Clone(void * pArg)
{
	CUI_Enemy_Indicator*		pInstance = new CUI_Enemy_Indicator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Enemy_Indicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Enemy_Indicator::Free()
{
	USEGAMEINSTANCE;
	__super::Free();
}
