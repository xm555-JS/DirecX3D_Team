#include "stdafx.h"
#include "..\Public\Mutation_HunterDog.h"
#include "State_Handler.h"
#include "Navigation.h"


#include "GameInstance.h"

/*돌연변이 사냥개*/
CMutation_HunterDog::CMutation_HunterDog()
	: CEnemy_Normal()
{
}

CMutation_HunterDog::CMutation_HunterDog(const CMutation_HunterDog& rhs)
	: CEnemy_Normal(rhs)
{
}

HRESULT CMutation_HunterDog::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMutation_HunterDog::Initialize(void* pArg)
{
	m_fFrustumRange = 2.5f;


	FAILEDCHECK(__super::Initialize(pArg));

	if (FAILED(SetUp_Components()))
		__debugbreak();

	//if(pArg)
	//	memcpy(&m_pInfo, pArg, sizeof(ENEMYDESC));

	USEGAMEINSTANCE;


	//m_pTransformCom->Set_Scaled(_float3(m_pInfo.vScale.x, m_pInfo.vScale.y, m_pInfo.vScale.z));
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_pInfo.vPosition));
	//m_pModelCom->Set_CurrentAnimation(m_iAnimNum);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vStartPos));

	FAILEDCHECK(SetUp_State_Handler());
	FAILEDCHECK(Create_UI_Status(10));

	return S_OK;
}

void CMutation_HunterDog::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;


	m_pState_Handler->Tick(fTimeDelta);

	SetUp_NonAttacked(fTimeDelta);
}

void CMutation_HunterDog::LateTick(_float fTimeDelta)
{
#ifndef _MAPTOOL
	if (!m_pTransformCom->Is_InAir())
		m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);
#endif

	if (IsInFrustum())
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);

}

HRESULT CMutation_HunterDog::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

		m_pModelCom->Render(i, m_pShaderCom, 0, "g_Bones");
	}

	return S_OK;
}


HRESULT CMutation_HunterDog::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MUTATION_HUNTERDOG), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.m_iCurrentIndex = 0;
	NaviDesc.pOwner = this;
	if (FAILED(__super::Add_Component(m_iCurLevel, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc)))
		return E_FAIL;
	m_pNaviCom->Find_CurrentIndex(m_pTransformCom);

	return S_OK;
}

HRESULT CMutation_HunterDog::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	return S_OK;
}
HRESULT CMutation_HunterDog::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	m_pState_Handler->Set_Animation(STATE_ENEMY_MUTATION_HUNTERDOG_IDLE_NORMAL, 9);//두리번거리는 상태 



	m_pState_Handler->SetUp_StartState(STATE_ENEMY_MUTATION_HUNTERDOG_IDLE_NORMAL);
	return S_OK;
}

CMutation_HunterDog* CMutation_HunterDog::Create()
{
	CMutation_HunterDog* pInstance = new CMutation_HunterDog();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMutation_HunterDog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMutation_HunterDog::Clone(void* pArg)
{
	CMutation_HunterDog* pInstance = new CMutation_HunterDog(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMutation_HunterDog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMutation_HunterDog::Free()
{
	__super::Free();
}
