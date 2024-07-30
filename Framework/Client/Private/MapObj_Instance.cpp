#include "stdafx.h"
//#include "PhysXCollider.h"
//#include "Math_Utillity.h"
//#include "Easing_Utillity.h"
//#include "Cursor.h"
//#include "Sound_Manager.h"
#include "MapObj_Instance.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Imgui_Manager.h"

#include "Light.h"

CMapObj_Instance::CMapObj_Instance()
	: CGameObject()
{
}

CMapObj_Instance::CMapObj_Instance(const CMapObj_Instance& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMapObj_Instance::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapObj_Instance::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(__super::Initialize(&TransformDesc));

	if (nullptr != pArg)
	{
		memcpy(&m_MapObjInstanceDesc, pArg, sizeof(MAPOBJINSTANCEDESC));
	}

	FAILEDCHECK(SetUp_Components());

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);

	m_fDistortionPower = 30.f;

	return S_OK;
}

void CMapObj_Instance::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	if (3 == m_iNumPass)
	{
		m_vUVSpeed.x += -0.2f * fTimeDelta;
		m_vUVSpeed.y += 0.f * fTimeDelta;
	}
}

void CMapObj_Instance::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
}

HRESULT CMapObj_Instance::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS));

		m_pModelCom->Render(i, m_pShaderCom, m_iNumPass, nullptr, m_bIsEnableFrustum);
	}

	return S_OK;
}

HRESULT CMapObj_Instance::Render(void* pArg)
{
	RENDERDESC	Desc;
	memcpy(&Desc, pArg, sizeof(RENDERDESC));

	if (RENDERTYPE_DEPTH == Desc.eType)
	{
		FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));

		CLight* pTargetLight = (CLight*)Desc.lParam;
		pTargetLight->SetUp_DepthComponents(m_pShaderCom);

		m_pShaderCom->Begin(1);

		for (_uint i = 0; i < m_iNumMeshContainers; ++i)
		{
			FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));
			FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS));

			m_pModelCom->Render(i, m_pShaderCom, 1, nullptr, false);
		}
	}

	return S_OK;
}

HRESULT CMapObj_Instance::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODELINSTANCE), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	ID_UINT		ModelID;
	ModelID = m_MapObjInstanceDesc.ModelID;

	//if (//ModelID.tNums.Num1 == ID_UINT(MODEL_STATIC, MODEL_STRUCTS_DESERT).tNums.Num1
	//	 ModelID == ID_UINT(MODEL_STATIC, MODEL_STRUCTS_DESERT, (enum)93) || ModelID == ID_UINT(MODEL_STATIC, MODEL_STRUCTS_DESERT, (_uint)61)
	//	|| ModelID == ID_UINT(MODEL_STATIC, MODEL_STRUCTS_DESERT, (_uint)110))
	//	m_iNumPass = 2;

	if (ModelID.tNums.Num1 == ID_UINT(MODEL_STATIC_INSTANCE, MODEL_STRUCTS_DESERT).tNums.Num1)
	{
		if (ModelID.tNums.Num2 == 5
			|| ModelID.tNums.Num2 == 3
			|| ModelID.tNums.Num2 == 6
			|| ModelID.tNums.Num2 == 1
			|| ModelID.tNums.Num2 == 10)
			m_iNumPass = 2;
	}

	if (ModelID.tNums.Num1 == ID_UINT(MODEL_STATIC_INSTANCE, MODEL_STRUCTS_ALL).tNums.Num1)
	{

		if (73 <= ModelID.tNums.Num2 && 86 >= ModelID.tNums.Num2)
		{
			m_bIsEnableFrustum = false;
			m_iNumPass = 2;
		}
		m_bIsEnableFrustum = false;
	}

	if (ModelID.tNums.Num1 == ID_UINT(MODEL_STATIC_INSTANCE, MODEL_GRASS).tNums.Num1
		|| ModelID.tNums.Num1 == ID_UINT(MODEL_STATIC_INSTANCE, MODEL_GRASS_DESERT).tNums.Num1)
		m_iNumPass = 3;

	if (ModelID.tNums.Num1 == ID_UINT(MODEL_STATIC_INSTANCE, MODEL_TREE).tNums.Num1)
	{
		if (ModelID.tNums.Num2 == 66
			|| ModelID.tNums.Num2 == 67)
			m_iNumPass = 3;
		if (82 == ModelID.tNums.Num2) // 사막 나무
		{
			m_bIsEnableFrustum = false;
		}
	}
	if (ModelID.tNums.Num1 == ID_UINT(MODEL_STATIC_INSTANCE, MODEL_ROOM).tNums.Num1)
	{
		m_iNumPass = 0;
	}
	if (ModelID.tNums.Num1 == ID_UINT(MODEL_STATIC_INSTANCE, MODEL_ROCK).tNums.Num1 ||
		ModelID.tNums.Num1 == ID_UINT(MODEL_STATIC_INSTANCE, MDOEL_GRAY_ROCK).tNums.Num1 ||
		ModelID.tNums.Num1 == ID_UINT(MODEL_STATIC_INSTANCE, MODEL_ROCK_ALL).tNums.Num1)
	{
		m_bIsEnableFrustum = false;
	}

	/* For.Com_Model*/
	FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ModelID, ID_UINT(MODEL_STATIC_INSTANCE), (CComponent**)&m_pModelCom, &m_MapObjInstanceDesc.NumModel));
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

#if defined(_NOISE_TEX)
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE, TEXTURE_EFFECT_NOISE), (CComponent**)&m_pNoiseTexture));
#endif // _EFFECT

#ifdef _EFFECT

	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_MASK), ID_UINT(TEXTURE, TEXTURE_EFFECT_MASK), (CComponent**)&m_pTextureCom));

#endif // _EFFECT

	return S_OK;
}

HRESULT CMapObj_Instance::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

#if defined(_NOISE_TEX)
	FAILEDCHECK(m_pNoiseTexture->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture", 675));
#endif // _EFFECT


	//_float fAlphaRange = DEBUGFLOAT;
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRange", &fAlphaRange, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float)));

#ifdef _EFFECT

	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 351));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fDistortionStrength", &m_fDistortionPower, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vUVSpeed_Mask", &m_vUVSpeed, sizeof(_float2)));

#endif // _EFFECT

	return S_OK;
}
void CMapObj_Instance::Input_Key(_float fTimeDelta)
{
}

HRESULT CMapObj_Instance::SetUp_Model(ID_UINT iPrototypeID)
{
	/* For.Com_Model */
	FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, iPrototypeID, ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}

CMapObj_Instance* CMapObj_Instance::Create()
{
	CMapObj_Instance* pInstance = new CMapObj_Instance();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMapObj_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMapObj_Instance::Clone(void* pArg)
{
	CMapObj_Instance* pInstance = new CMapObj_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMapObj_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapObj_Instance::Free()
{
	__super::Free();
}
