#include "stdafx.h"
#include "Cloud_Sky.h"
#include "GameInstance.h"
#include "Light.h"

#include "Imgui_Manager.h"

CCloud_Sky::CCloud_Sky()
	: CGameObject()
{
}

CCloud_Sky::CCloud_Sky(const CCloud_Sky& rhs)
	: CGameObject(rhs)
{
}

HRESULT CCloud_Sky::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCloud_Sky::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(__super::Initialize(&TransformDesc));

	FAILEDCHECK(SetUp_Components());

	FAILEDCHECK(Setup_TransformState());

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);

	return S_OK;
}

void CCloud_Sky::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	USEGAMEINSTANCE;
	_float4 vCamPos = pGameInstance->Get_CamPosition();
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, vCamPos.y + 100.f, 0.f, 1.f));

	


}

void CCloud_Sky::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_CLOUD_SKY, this);
}

HRESULT CCloud_Sky::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	m_pShaderCom->Begin(7);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CCloud_Sky::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEX), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Model*/
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER, BUFFER_RECT), (CComponent**)&m_pVIBufferCom));
	
#if defined(_NOISE_TEX)
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE, TEXTURE_EFFECT_NOISE), (CComponent**)&m_pTextureCom));
#endif // _EFFECT


	return S_OK;
}

HRESULT CCloud_Sky::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	_float	fScale = 20.f;
	//_float	fScale = 30.f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fScale", &fScale, sizeof(_float)));

#if defined(_NOISE_TEX)
	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 643));
#endif // _EFFECT


	return S_OK;
}

HRESULT CCloud_Sky::Setup_TransformState()
{
	//m_pTransformCom->Set_Scaled(_float3(1000.f, 1000.f, 1000.f));
	m_pTransformCom->Set_Scaled(_float3(10000.f, 10000.f, 10000.f));


	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 400.f, 0.f, 1.f));
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-5000.f, 10.5f, -5000.f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));


	return S_OK;
}

CCloud_Sky* CCloud_Sky::Create()
{
	CCloud_Sky* pInstance = new CCloud_Sky();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCloud_Sky");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCloud_Sky::Clone(void* pArg)
{
	CCloud_Sky* pInstance = new CCloud_Sky(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCloud_Sky");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCloud_Sky::Free()
{
	__super::Free();
}
