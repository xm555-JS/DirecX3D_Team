#include "stdafx.h"
#include "..\Public\Dragon_Appear_Trigger.h"

#include "GameInstance.h"

#include "Imgui_Manager.h"
#include "Enemy_Boss_Dragon.h"
#include "Sound_Manager.h"

CDragon_Appear_Trigger::CDragon_Appear_Trigger()
	: CGameObject()
{
}

CDragon_Appear_Trigger::CDragon_Appear_Trigger(const CDragon_Appear_Trigger& rhs)
	: CGameObject(rhs)
{
}

HRESULT CDragon_Appear_Trigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDragon_Appear_Trigger::Initialize(void* pArg)
{
	m_fFrustumRange = 5.f;
	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));

	if (FAILED(SetUp_Components()))
		__debugbreak();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);

	//_float4(117.f, 102.f, 466.f, 1.f)
	//51.f, 96.f, 442.f
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(100.f, 153.f, 435.27, 1.f));

	return S_OK;
}

void CDragon_Appear_Trigger::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	if (m_pCol_Sphere->Get_EnterColliders()->size() != 0)
	{
		m_pOwnerObj = GAMEINSTANCE->Get_Object(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_FROSTDRAGON));
		m_pTarget = m_pCol_Sphere->Get_EnterColliders()->front()->Get_Owner();
		if (m_pOwnerObj)
		{
			((CEnemy_Boss*)m_pOwnerObj)->Set_Target(m_pTarget);
			GAMEINSTANCE->Add_DeadObject(this);
		}

		MGR(CSound_Manager)->ChangeBGM(TEXT("Dragon_Enter.wav"), 0.3f, 0.1f);
	}

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
	//trigger 밟으면 디졸브로 사라졌으면합니다.
}

void CDragon_Appear_Trigger::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (IsInFrustum())
	{
		pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}

	pGameInstance->Add_ColliderGroup(ID_UINT(COL_TRIGGER), m_pCol_Sphere);
}

HRESULT CDragon_Appear_Trigger::Render()
{
	//if (nullptr == m_pShaderCom ||
	//	nullptr == m_pModelCom)
	//	return E_FAIL;
	//
	//_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();
	//
	///* 셰이더 전역변수에 값을 던진다. */
	//FAILEDCHECK(SetUp_ShaderResource());
	//
	//m_pShaderCom->Begin(0);
	//
	//for (_uint i = 0; i < iNumMeshContainers; ++i)
	//{
	//	FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));
	//	FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS));
	//
	//	m_pModelCom->Render(i, m_pShaderCom);
	//}

#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CDragon_Appear_Trigger::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	//FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_TRIGGER, TRIGGER_DRAGON), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));
	//m_pModelCom->Set_Owner(this);

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(10.f, 10.f, 10.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CDragon_Appear_Trigger::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	return S_OK;
}

CDragon_Appear_Trigger* CDragon_Appear_Trigger::Create()
{
	CDragon_Appear_Trigger* pInstance = new CDragon_Appear_Trigger();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDragon_Appear_Trigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDragon_Appear_Trigger::Clone(void* pArg)
{
	CDragon_Appear_Trigger* pInstance = new CDragon_Appear_Trigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDragon_Appear_Trigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDragon_Appear_Trigger::Free()
{
	__super::Free();
}
