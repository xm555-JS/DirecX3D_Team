#include "stdafx.h"
//#include "PhysXCollider.h"
//#include "Math_Utillity.h"
//#include "Easing_Utillity.h"
//#include "Sound_Manager.h"
#include "Gray_Rock.h"
#include "GameInstance.h"
#include "Collider.h"


CGray_Rock::CGray_Rock()
	: CGameObject()
{
}

CGray_Rock::CGray_Rock(const CGray_Rock& rhs)
	: CGameObject(rhs)
{
}

HRESULT CGray_Rock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGray_Rock::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());

	/* For.Com_Model */
	ID_UINT iID;
	_uint iModelNum = *(_uint*)pArg;
	if (iModelNum > 255)
	{
		iID.tNums.Num2 = iModelNum % 255;
		iID.tNums.Num3 = iModelNum / 255;
	}
	else
	{
		iID.tNums.Num2 = iModelNum;
		iID.tNums.Num3 = NULL;
	}
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MDOEL_GRAY_ROCK, iID.tNums.Num2, iID.tNums.Num3), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);

	return S_OK;
}

void CGray_Rock::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;
}

void CGray_Rock::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
}

HRESULT CGray_Rock::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	m_pShaderCom->Begin(0);

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS));

		m_pModelCom->Render(i, m_pShaderCom);
	}

	return S_OK;
}

HRESULT CGray_Rock::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	return S_OK;
}

HRESULT CGray_Rock::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float)));
	return S_OK;
}

void CGray_Rock::Input_Key(_float fTimeDelta)
{
}

CGray_Rock* CGray_Rock::Create()
{
	CGray_Rock* pInstance = new CGray_Rock();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGray_Rock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGray_Rock::Clone(void* pArg)
{
	CGray_Rock* pInstance = new CGray_Rock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGray_Rock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGray_Rock::Free()
{
	__super::Free();
}
