#include "stdafx.h"
#include "..\Public\AnimObject.h"
#include "PhysXCollider.h"
#include "Math_Utillity.h"
#include "Easing_Utillity.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"

#include "Imgui_Manager.h"

CAnimObject::CAnimObject()
	: CGameObject()
{
}

CAnimObject::CAnimObject(const CAnimObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CAnimObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimObject::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	if (FAILED(__super::Initialize(&TransformDesc)))
		__debugbreak();

	if (nullptr != pArg)
	{
		memcpy(&m_iAnimModelID, pArg, sizeof(_uint));
	}

	if (FAILED(SetUp_Components()))
		__debugbreak();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_PLAYER, this);
	pGameInstance->Add_LateTickGroup(LTG_PLAYER, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	m_pModelCom->Set_CurrentAnimation(m_iAnimNum);

	XMStoreFloat4x4(&m_BoneMatrix, m_pTransformCom->Get_WorldMatrix());

	return S_OK;
}

void CAnimObject::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	m_pModelCom->Set_CurrentAnimation(m_iAnimNum);

	USEGAMEINSTANCE;

	if (KEY_PRESSING(VK_LEFT))
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta, -2.f);

	if (KEY_PRESSING(VK_RIGHT))
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta, 2.f);

	if (KEY_PRESSING(VK_UP))
		m_pTransformCom->Go_Up(fTimeDelta, 2.f);

	if (KEY_PRESSING(VK_DOWN))
		m_pTransformCom->Go_Down(fTimeDelta, 2.f);

	if (m_bAnimPlay)
		m_pModelCom->Play_Animation(fTimeDelta * m_fAnimAcc);

	//m_pModelCom->Set_AnimationGroup(ANIM_PLAYER_FEMALE, WEAPON_GUN);
	m_pColCom->Update(XMLoadFloat4x4(&m_BoneMatrix)/* * XMLoadFloat4x4(&m_pModelCom->Get_TransformMatrix())*/);

}

void CAnimObject::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);

#ifdef _DEBUG
	GAMEINSTANCE->Add_DebugRenderGroup(m_pColCom);
#endif
}

HRESULT CAnimObject::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		//if (i == 0 || i == 1 || i == 6)
		//	continue;

		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

		m_pModelCom->Render(i, m_pShaderCom, m_iPassIndex, "g_Bones");
	}

	m_pColCom->Render();

	return S_OK;
}

HRESULT CAnimObject::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	ID_UINT ModelID;

	ModelID = m_iAnimModelID;

	/* For.Com_Model */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ModelID, ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));

	if (TYPE_ANIM_GROUP == m_pModelCom->Get_ModelType())
	{
		m_pModelCom->Set_AnimationGroup(ANIM_PLAYER_FEMALE);

		GAMEINSTANCE->SetUp_HierarchyNodePtr(ANIM_PLAYER_FEMALE, m_pModelCom);
	}

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(10.f, 10.f, 10.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);

	/* For.Com_Collider */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER), (CComponent**)&m_pColCom, &ColliderDesc));
	m_pColCom->Set_Owner(this);

	return S_OK;
}

HRESULT CAnimObject::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	//if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
	//	__debugbreak();


	return S_OK;
}

void CAnimObject::Input_Key(_float fTimeDelta)
{
}

CAnimObject* CAnimObject::Create()
{
	CAnimObject* pInstance = new CAnimObject();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAnimObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAnimObject::Clone(void* pArg)
{
	CAnimObject* pInstance = new CAnimObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAnimObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimObject::Free()
{
	__super::Free();
}
