#include "stdafx.h"
#include "..\Public\Water_Base.h"

#include "GameInstance.h"
#include "Imgui_Manager.h"

CWater_Base::CWater_Base()
	: CGameObject()
{
}

CWater_Base::CWater_Base(const CWater_Base & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWater_Base::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWater_Base::Initialize(void * pArg)
{
	USEGAMEINSTANCE;
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(CGameObject::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());

	FAILEDCHECK(Setup_TransformState());

	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);

	return S_OK;
}

void CWater_Base::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWater_Base::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
}

HRESULT CWater_Base::Render()
{
	NULLCHECK(m_pShaderCom);
	NULLCHECK(m_pVIBufferCom);

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();


	return S_OK;
}


HRESULT CWater_Base::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXWATER), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CWater_Base::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);

	USEGAMEINSTANCE;
	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float)));


	return S_OK;
}

HRESULT CWater_Base::Setup_TransformState()
{
	m_pTransformCom->Set_Scaled(_float3(10000.f, 10000.f, 10000.f));


	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

	return S_OK;
}



CWater_Base * CWater_Base::Create()
{
	CWater_Base*		pInstance = new CWater_Base();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWater_Base");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CWater_Base::Clone(void * pArg)
{
	CWater_Base*		pInstance = new CWater_Base(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWater_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWater_Base::Free()
{
	__super::Free();
}
