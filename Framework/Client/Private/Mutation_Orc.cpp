#include "stdafx.h"
#include "..\Public\Mutation_Orc.h"
#include "State_Handler.h"
#include "Navigation.h"


#include "GameInstance.h"

/*돌연변이 오크*/
CMutation_Orc::CMutation_Orc()
	: CEnemy_Normal()
{
}

CMutation_Orc::CMutation_Orc(const CMutation_Orc& rhs)
	: CEnemy_Normal(rhs)
{
}

HRESULT CMutation_Orc::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMutation_Orc::Initialize(void* pArg)
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
	FAILEDCHECK(Create_UI_Status(12, 1.4f));

	return S_OK;
}

void CMutation_Orc::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;


	m_pState_Handler->Tick(fTimeDelta);

	SetUp_NonAttacked(fTimeDelta);
}

void CMutation_Orc::LateTick(_float fTimeDelta)
{
#ifndef _MAPTOOL
	if (!m_pTransformCom->Is_InAir())
		m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);
#endif
	if (IsInFrustum())
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);

}

HRESULT CMutation_Orc::Render()
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


HRESULT CMutation_Orc::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MUTATION_ORC), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
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

HRESULT CMutation_Orc::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	m_pState_Handler->Set_Animation(STATE_ENEMY_MUTATION_ORC_IDLE_NORMAL, 9);//두리번거리는 상태 



	m_pState_Handler->SetUp_StartState(STATE_ENEMY_MUTATION_ORC_IDLE_NORMAL);
	return S_OK;
}

CMutation_Orc* CMutation_Orc::Create()
{
	CMutation_Orc* pInstance = new CMutation_Orc();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMutation_Orc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMutation_Orc::Clone(void* pArg)
{
	CMutation_Orc* pInstance = new CMutation_Orc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMutation_Orc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMutation_Orc::Free()
{
	__super::Free();
}
