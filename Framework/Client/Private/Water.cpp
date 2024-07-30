#include "stdafx.h"
#include "..\Public\Water.h"

#include "GameInstance.h"
#include "Imgui_Manager.h"

CWater::CWater()
	: CGameObject()
{
}

CWater::CWater(const CWater & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWater::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWater::Initialize(void * pArg)
{
	USEGAMEINSTANCE;
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(CGameObject::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());

	FAILEDCHECK(Setup_TransformState());

	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);

	m_fBaseMoveSpeed = 1.f;

	return S_OK;
}

void CWater::Tick(_float fTimeDelta)
{
	Move_WaterSurface(fTimeDelta);

	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, DEBUGVECTOR);
}

void CWater::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	pGameInstance->Add_RenderGroup(RENDER_WATER_SURFACE, this);
}

HRESULT CWater::Render()
{
	NULLCHECK(m_pShaderCom);
	NULLCHECK(m_pVIBufferCom);

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();


	return S_OK;
}


HRESULT CWater::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXWATER), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_NORMAL, TEXTURE_WATER), ID_UINT(TEXTURE, TEXTURE_NORMAL), (CComponent**)&m_pTextureCom));
	//FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_NOISE, TEXTURE_WATER), ID_UINT(TEXTURE, TEXTURE_NOISE), (CComponent**)&m_pDistortionTex));
	//FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CWater::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);
	NULLCHECK(m_pTextureCom);
	//NULLCHECK(m_pDistortionTex);
	

	USEGAMEINSTANCE;
	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	//FAILEDCHECK(m_pShaderCom->Set_ShaderResourceView("g_DepthTexture", pGameInstance->Get_CurRenderer()->Get_SRV(TEXT("Target_Depth"))));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vUVSpeed_0", &m_fUVSpeed_0, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vUVSpeed_1", &m_fUVSpeed_1, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vUVSpeed_2", &m_fUVSpeed_2, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vUVSpeed_3", &m_fUVSpeed_3, sizeof(_float2)));

	//_float		fUVScale_0 = DEBUGFLOAT4(0).x;
	//_float		fUVScale_1 = DEBUGFLOAT4(0).y;
	//_float		fUVScale_2 = DEBUGFLOAT4(0).z;
	//_float		fUVScale_3 = DEBUGFLOAT4(0).w;

	_float		fUVScale_0 = 110.5f;
	_float		fUVScale_1 = 500.f;
	_float		fUVScale_2 = 500.f;
	_float		fUVScale_3 = 1000.f;

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScale_0", &fUVScale_0, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScale_1", &fUVScale_1, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScale_2", &fUVScale_2, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScale_3", &fUVScale_3, sizeof(_float)));


	//_float		fDistortionStrength = DEBUGFLOAT4(0).x;
	//_float		fDistortionPower = DEBUGFLOAT4(0).y;
	//_float		fDistortionStrength = 10.f;
	//_float		fDistortionPower = 10.f;
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fDistortionStrength", &fDistortionStrength, sizeof(_float)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fDistortionPower", &fDistortionPower, sizeof(_float)));
	
	

	_float3		vNormal = { 0.f, 1.f, 0.f };
	_float3		vTangent = { 1.f, 0.f, 0.f };
	_float3		vBinormal = { 0.f, 0.f, -1.f };

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vNormal", &vNormal, sizeof(_float3)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vTangent", &vTangent, sizeof(_float3)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vBinormal", &vBinormal, sizeof(_float3)));


	//FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 1));
	//FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_NormalTexture0", 640));
	//FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_NormalTexture1", 629));
	//FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_NormalTexture2", 638));
	//FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_NormalTexture3", 205));

	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_NormalTexture0", 0));
	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_NormalTexture1", 1));
	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_NormalTexture2", 2));
	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_NormalTexture3", 3));

	//FAILEDCHECK(m_pDistortionTex->Set_ShaderResourceView(m_pShaderCom, "g_DistortionTexture0", 0));
	//FAILEDCHECK(m_pDistortionTex->Set_ShaderResourceView(m_pShaderCom, "g_DistortionTexture1", 1));



	return S_OK;
}

HRESULT CWater::Setup_TransformState()
{
	//m_pTransformCom->Set_Scaled(_float3(1000.f, 1000.f, 1000.f));
	m_pTransformCom->Set_Scaled(_float3(10000.f, 10000.f, 10000.f));


	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 10.5f, 0.f, 1.f));
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-5000.f, 10.5f, -5000.f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

	return S_OK;
}

void CWater::Move_WaterSurface(_float fTimeDelta)
{
	//m_fBaseMoveSpeed = DEBUGFLOAT;
	m_fBaseMoveSpeed = 0.00016f;

	m_fUVSpeed_0.x += fTimeDelta * m_fBaseMoveSpeed;
	m_fUVSpeed_0.y += fTimeDelta * m_fBaseMoveSpeed;

	m_fUVSpeed_1.x += fTimeDelta * m_fBaseMoveSpeed * 1.1f;
	m_fUVSpeed_1.y -= fTimeDelta * m_fBaseMoveSpeed * 1.1f;

	m_fUVSpeed_2.x -= fTimeDelta * m_fBaseMoveSpeed * 1.2f;
	m_fUVSpeed_2.y += fTimeDelta * m_fBaseMoveSpeed * 1.2f;

	m_fUVSpeed_3.x -= fTimeDelta * m_fBaseMoveSpeed * 1.3f;
	m_fUVSpeed_3.y -= fTimeDelta * m_fBaseMoveSpeed * 1.3f;
}

CWater * CWater::Create()
{
	CWater*		pInstance = new CWater();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWater");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CWater::Clone(void * pArg)
{
	CWater*		pInstance = new CWater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWater::Free()
{
	__super::Free();
}
