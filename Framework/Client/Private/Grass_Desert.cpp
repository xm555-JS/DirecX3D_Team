#include "stdafx.h"
//#include "PhysXCollider.h"
//#include "Math_Utillity.h"
//#include "Easing_Utillity.h"
//#include "Cursor.h"
//#include "Sound_Manager.h"
#include "Grass_Desert.h"
#include "GameInstance.h"
#include "Collider.h"

CGrass_Desert::CGrass_Desert()
	: CGameObject()
{
}

CGrass_Desert::CGrass_Desert(const CGrass_Desert& rhs)
	: CGameObject(rhs)
{
}

HRESULT CGrass_Desert::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGrass_Desert::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());

	// Arg에 따른 모델 변경

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
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_GRASS_DESERT, iID.tNums.Num2, iID.tNums.Num3), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(TG_MAP, this);

	return S_OK;
}

void CGrass_Desert::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;
}

void CGrass_Desert::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
}

HRESULT CGrass_Desert::Render()
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

HRESULT CGrass_Desert::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	return S_OK;
}

HRESULT CGrass_Desert::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float)));
	return S_OK;
}
void CGrass_Desert::Input_Key(_float fTimeDelta)
{
}

HRESULT CGrass_Desert::SetUp_Model(ID_UINT iPrototypeID)
{
	/* For.Com_Model */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, iPrototypeID, ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}

CGrass_Desert* CGrass_Desert::Create()
{
	CGrass_Desert* pInstance = new CGrass_Desert();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGrass_Desert");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGrass_Desert::Clone(void* pArg)
{
	CGrass_Desert* pInstance = new CGrass_Desert(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGrass_Desert");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrass_Desert::Free()
{
	__super::Free();
}
