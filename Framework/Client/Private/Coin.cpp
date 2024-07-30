#include "stdafx.h"
//#include "PhysXCollider.h"
//#include "Math_Utillity.h"
//#include "Easing_Utillity.h"
#include "Sound_Manager.h"
#include "Coin.h"
#include "GameInstance.h"
#include "Collider.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"
#include "Minigame_Manager.h"

CCoin::CCoin()
	: CGameObject()
{
}

CCoin::CCoin(const CCoin& rhs)
	: CGameObject(rhs)
{
}

HRESULT CCoin::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCoin::Initialize(void* pArg)
{
	m_fFrustumRange = 1.f;
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(__super::Initialize(&TransformDesc));

	///* For.Com_Model */
	//ID_UINT iID;
	//_uint iModelNum = *(_uint*)pArg;
	//if (iModelNum > 255)
	//{
	//	iID.tNums.Num2 = iModelNum % 255;
	//	iID.tNums.Num3 = iModelNum / 255;
	//}
	//else
	//{
	//	iID.tNums.Num2 = iModelNum;
	//	iID.tNums.Num3 = NULL;
	//}

	FAILEDCHECK(SetUp_Components());

	//FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_COIN, iID.tNums.Num2, iID.tNums.Num3), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));
	//m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);

	//m_fEffect_StartTime = fRandom(0.f, 3.f);

	return S_OK;
}

void CCoin::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	USEGAMEINSTANCE;

	IMPL_ONCE(m_bEffect0, EFFECT_EXCUTE("MiniGame_Effect_Coin", XMMatrixScaling(0.5f, 0.5f, 0.5f), 1.f, this, CEffect_Manager::FOLLOW_POS));

	Collider_ToPlayer();

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());

	if (MGR(CMinigame_Manager)->Get_FinishedGame(MINIGAME_BIKE))
	{
		pGameInstance->Add_DeadObject(this);
	}
}

void CCoin::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_MINIGAME), m_pCol_Sphere);
	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}
}

HRESULT CCoin::Render()
{
	if (nullptr == m_pShaderCom /*||
		nullptr == m_pModelCom*/)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	m_pShaderCom->Begin(0);

	//for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	//{
	//	FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));
	//	FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS));

	//	m_pModelCom->Render(i, m_pShaderCom);
	//}

	//#ifdef _DEBUG
	m_pCol_Sphere->Render();
	//#endif // _DEBUG

	return S_OK;
}

HRESULT CCoin::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Collider */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(20.f, 20.f, 20.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 10.f, 0.f);

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_MINIGAME), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CCoin::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float)));


	return S_OK;
}
void CCoin::Input_Key(_float fTimeDelta)
{
}

void CCoin::Collider_ToPlayer()
{
	USEGAMEINSTANCE;

	for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
	{
		EFFECT_POS("MiniGame_Effect_Coin_Pass", m_pTransformCom->Get_WorldMatrix_ptr());
		pGameInstance->Add_PoolingObject(this);
		CMinigame_Manager::Get_Instance()->Plus_GainNum(1);
		MGR(CSound_Manager)->PlaySound(TEXT("Coin.wav"), CSound_Manager::CHANNEL_UI, 0.4f);
	}
}

CCoin* CCoin::Create()
{
	CCoin* pInstance = new CCoin();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCoin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCoin::Clone(void* pArg)
{
	CCoin* pInstance = new CCoin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCoin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCoin::Free()
{
	__super::Free();
}
