#include "stdafx.h"
#include "Effect_Trail.h"
#include "GameInstance.h"
#include "Easing_Utillity.h"

#include "Model.h"
#include "HIerarchyNode.h"

#include "VIBuffer_Trail.h"

CEffect_Trail::CEffect_Trail()
{
}

CEffect_Trail::CEffect_Trail(const CEffect_Trail& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Trail::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CEffect_Trail::Initialize(void* pArg)
{
	//NULLCHECK(pArg);
	FAILEDCHECK(__super::Initialize());
	FAILEDCHECK(SetUp_Components());
	GAMEINSTANCE->Add_TickGroup(TG_EFFECT, this);
	GAMEINSTANCE->Add_LateTickGroup(LTG_EFFECT, this);

	return S_OK;
}

void CEffect_Trail::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	CGameObject* pPlayer = pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(PLAYER));
	m_stTrailDesc.iNumTrail = 10.f;
	m_stTrailDesc.vLocalSwordLow = { 0.f, 0.f, 0.f, 1.f };
	m_stTrailDesc.vLocalSwordHigh = { 0.f, 0.f, 0.f, 1.f };
	m_stTrailDesc.matOwnerWorld = *pPlayer->Get_WorldMatrix();

	m_pVIBufferTrailCom->Update(fTimeDelta, m_stTrailDesc);
}

void CEffect_Trail::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_EFFECT, this);
}

HRESULT CEffect_Trail::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferTrailCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferTrailCom->Render();

	return S_OK;
}

HRESULT CEffect_Trail::SetUp_Components()
{
	USEGAMEINSTANCE;
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEX), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_MASK), ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_MASK), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	CVIBuffer_Trail::TRAIL_DESC stTrailDesc;
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_TRAIL), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferTrailCom, &stTrailDesc));

	return S_OK;
}

HRESULT CEffect_Trail::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));
	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 0));

	return S_OK;
}

CEffect_Trail* CEffect_Trail::Create()
{
	CEffect_Trail* pInstance = new CEffect_Trail();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Trail::Clone(void* pArg)
{
	CEffect_Trail* pInstance = new CEffect_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Trail::Free()
{
	__super::Free();
}
