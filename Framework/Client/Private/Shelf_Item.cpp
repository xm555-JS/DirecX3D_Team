#include "stdafx.h"
//#include "PhysXCollider.h"
//#include "Math_Utillity.h"
//#include "Easing_Utillity.h"
//#include "Sound_Manager.h"
#include "Shelf_Item.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Item_Manager.h"


CShelf_Item::CShelf_Item()
	: CGameObject()
{
}

CShelf_Item::CShelf_Item(const CShelf_Item& rhs)
	: CGameObject(rhs)
{
}

HRESULT CShelf_Item::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CShelf_Item::Initialize(void* pArg)
{
	m_fFrustumRange = 1.f;
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

	m_eItem_Type = (ITEM_TYPE)(iID.tNums.Num2 + 2);

	// 모델 번호
	// Num2 = 1 버섯
	// Num2 = 2 선인장
	// Num2 = 3 비트
	// Num2 = 4 고사리
	// Num2 = 5 벼
	// Num2 = 6 상추
	// Num2 = 7 조개
	// Num2 = 8 소라

	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_SHELF_ITEM, iID.tNums.Num2, iID.tNums.Num3), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);

	return S_OK;
}

void CShelf_Item::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	if (0 < CItem_Manager::Get_Instance()->Find_ItemNum(m_eItem_Type))
	{
		m_bRender = true;
	}
	else
	{
		m_bRender = false;
	}

}

void CShelf_Item::LateTick(_float fTimeDelta)
{
	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}
}

HRESULT CShelf_Item::Render()
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

		if (m_bRender)
		{
			m_pModelCom->Render(i, m_pShaderCom);
		}
	}

	//#ifdef _DEBUG
	//	m_pCol_Sphere->Render();
	//#endif // _DEBUG

	return S_OK;
}

HRESULT CShelf_Item::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	return S_OK;
}

HRESULT CShelf_Item::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float)));


	return S_OK;
}
void CShelf_Item::Input_Key(_float fTimeDelta)
{
}

CShelf_Item* CShelf_Item::Create()
{
	CShelf_Item* pInstance = new CShelf_Item();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CShelf_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShelf_Item::Clone(void* pArg)
{
	CShelf_Item* pInstance = new CShelf_Item(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShelf_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShelf_Item::Free()
{
	__super::Free();
}
