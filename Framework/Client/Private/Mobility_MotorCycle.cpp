#include "stdafx.h"
#include "..\Public\Mobility_MotorCycle.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Unit.h"
#include "Imgui_Manager.h"
#include "State_Handler.h"

#include "Light.h"

#include "Effect_Manager.h"

CMobility_MotorCycle::CMobility_MotorCycle()
	: CWeapon()
{

}

CMobility_MotorCycle::CMobility_MotorCycle(const CMobility_MotorCycle& rhs)
	: CWeapon(rhs)
{

}

HRESULT CMobility_MotorCycle::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CMobility_MotorCycle::Initialize(void* pArg)
{
	USEGAMEINSTANCE;

	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(SetUp_State_Handler());

	if (nullptr != pArg)
		m_pTarget = (CGameObject*)pArg;

	m_pState_Handler->Set_Player(m_pTarget);

	m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(90.0f));

	m_vGlowColor = _float4(0.3f, 0.1f, 0.3f, 0.5f);

	return S_OK;
}

void CMobility_MotorCycle::Tick(_float fTimeDelta)
{
	m_pState_Handler->Tick(fTimeDelta);
	__super::Tick(fTimeDelta);
}

HRESULT CMobility_MotorCycle::Render()
{
	NULLCHECK(m_pShaderCom);
	NULLCHECK(m_pModelCom);

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

		m_pModelCom->Render(i, m_pShaderCom, 4, "g_Bones");
	}

	_bool	bUseToon = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bToon", &bUseToon, sizeof(_bool)));

	_float4 vColor = { 1.f, 1.f, 1.f, 1.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSourColor", &vColor, sizeof(_float4)));

	m_vDissolveColor = { 1.f, 1.f, 1.f, 1.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4)));
	_vector vAlphaZeroDiffuse = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_vector vAlphaZeroGlow = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));

	return S_OK;
}

HRESULT CMobility_MotorCycle::Render(void* pArg)
{
	if (m_bUsing)
	{
		RENDERDESC	Desc;
		memcpy(&Desc, pArg, sizeof(RENDERDESC));

		if (RENDERTYPE_DEPTH == Desc.eType)
		{
			_float4x4		WorldMatrixTranspose;

			XMStoreFloat4x4(&WorldMatrixTranspose, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

			FAILEDCHECK(m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrixTranspose, sizeof(_float4x4)));

			CLight* pTargetLight = (CLight*)Desc.lParam;
			pTargetLight->SetUp_DepthComponents(m_pShaderCom);

			//m_pShaderCom->Begin(3);

			for (_uint i = 0; i < m_iNumMeshContainers; ++i)
			{
				FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

				m_pModelCom->Render(i, m_pShaderCom, 3, "g_Bones");
			}
		}
	}

	return S_OK;
}

HRESULT CMobility_MotorCycle::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

#if defined(_NOISE_TEX)
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE), (CComponent**)&m_pDissolveTextureCom));
#endif

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ANIM_WEAPON, MODEL_MOBILITY_MOTORCYCLE), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}

HRESULT CMobility_MotorCycle::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	USEGAMEINSTANCE;

	_float4x4		WorldMatrixTranspose;

	XMStoreFloat4x4(&WorldMatrixTranspose, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrixTranspose, sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	//_bool	bUseToon = true;
	//_bool	bUseRimLight = true;
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bToon", &bUseToon, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsDissolve", &m_bDissolve, sizeof(_bool)));

#if defined(_NOISE_TEX)
	FAILEDCHECK(m_pDissolveTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DissolveNoiseTexture", m_iDissolveTextureNoise));
	FAILEDCHECK(m_pDissolveTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DissolveColorTexture", m_iDissolveTextureColor));
#endif
	m_vDissolveColor = { 1.f, 0.85f, 0.2f, 0.2f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fDissolveStrength", &m_fDissolveStrength, sizeof(_float)));
	_float fTimeAccDissove = 0.f;
	fTimeAccDissove = (m_fTimeAccDissolve / m_fTimeDurationDissolve) * (1.f + m_fDissolveStrength); // 디졸드 지속하고 싶은 시간 만큼 나눠줌.
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fTimeAccDissolve", &fTimeAccDissove, sizeof(_float)));


	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &m_bGlow, sizeof(_bool)));
	//	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vGlowColor", &m_vGlowColor, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float)));

	_float4 vColor = { 1.f, 1.f, 1.f, 1.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSourColor", &vColor, sizeof(_float4)));
	_vector vAlphaZeroDiffuse = XMVectorSet(1.f, 0.85f, 0.2f, 0.2f);
	_vector vAlphaZeroGlow = XMVectorSet(1.f, 0.24f, 0.f, 0.4f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));

	return S_OK;
}

HRESULT CMobility_MotorCycle::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	m_pState_Handler->Set_Owner(this);

	m_pState_Handler->Set_Animation(STATE_MOTORCYCLE_IDLE, 5);
	m_pState_Handler->Set_Animation(STATE_MOTORCYCLE_JUMP_START, 4);
	m_pState_Handler->Set_Animation(STATE_MOTORCYCLE_JUMP_LOOP, 2);
	m_pState_Handler->Set_Animation(STATE_MOTORCYCLE_JUMP_RUN, 3);
	m_pState_Handler->Set_Animation(STATE_MOTORCYCLE_JUMP_END, 1);
	m_pState_Handler->Set_Animation(STATE_MOTORCYCLE_RUN, 5);

	m_pState_Handler->SetUp_StartState(STATE_MOTORCYCLE_RUN);

	return S_OK;
}

CMobility_MotorCycle* CMobility_MotorCycle::Create()
{
	CMobility_MotorCycle* pInstance = new CMobility_MotorCycle();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMobility_MotorCycle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMobility_MotorCycle::Clone(void* pArg)
{
	CMobility_MotorCycle* pInstance = new CMobility_MotorCycle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMobility_MotorCycle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMobility_MotorCycle::Free()
{
	__super::Free();

}
