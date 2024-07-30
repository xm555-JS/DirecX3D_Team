#include "stdafx.h"
#include "Effect_Point.h"

#include "GameInstance.h"

CEffect_Point::CEffect_Point()
{
}

CEffect_Point::CEffect_Point(const CEffect_Point& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Point::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Point::Initialize(void* pArg)
{
	FAILEDCHECK(__super::Initialize(pArg));
	FAILEDCHECK(SetUp_Components());

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(10.f, 0.f, 10.f, 1.f));

	GAMEINSTANCE->Add_TickGroup(TG_EFFECT, this);
	GAMEINSTANCE->Add_LateTickGroup(LTG_EFFECT, this);

	return S_OK;
}

HRESULT CEffect_Point::Initialize_Pooling(void* pArg)
{
	FAILEDCHECK(__super::Initialize(pArg));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(10.f, 0.f, 10.f, 1.f));

	GAMEINSTANCE->Add_TickGroup(TG_EFFECT, this);
	GAMEINSTANCE->Add_LateTickGroup(LTG_EFFECT, this);

	return S_OK;
}

void CEffect_Point::Tick(_float fTimeDelta)
{
	if (nullptr == m_pVIBufferCom)
		return;

	m_pVIBufferCom->Update(fTimeDelta);
}

void CEffect_Point::LateTick(_float fTimeDelta)
{
	//if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_EFFECT, this);
	}

}

HRESULT CEffect_Point::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CEffect_Point::SetUp_Components()
{
	USEGAMEINSTANCE;
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXPOINTINSTANCE), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_CURSOR), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_POINT_INSTANCE), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CEffect_Point::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	USEGAMEINSTANCE;

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4));

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	return S_OK;
}

CEffect_Point* CEffect_Point::Create()
{
	CEffect_Point* pInstance = new CEffect_Point();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Point::Clone(void* pArg)
{
	CEffect_Point* pInstance = new CEffect_Point(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Point::Free()
{
	__super::Free();
}
