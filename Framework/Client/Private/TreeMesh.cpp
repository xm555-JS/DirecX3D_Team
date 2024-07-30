#include "stdafx.h"
#include "..\Public\TreeMesh.h"
#include "PhysXCollider.h"
#include "Math_Utillity.h"
#include "Easing_Utillity.h"

#include "GameInstance.h"
#include "Cursor.h"
#include "Collider.h"
#include "Sound_Manager.h"

CTreeMesh::CTreeMesh()
	: CGameObject()
{
}

CTreeMesh::CTreeMesh(const CTreeMesh& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTreeMesh::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTreeMesh::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	if (FAILED(__super::Initialize(&TransformDesc)))
		__debugbreak();

	if (FAILED(SetUp_Components()))
		__debugbreak();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_DEFAULT, this);
	pGameInstance->Add_LateTickGroup(TG_DEFAULT, this);

	return S_OK;
}

void CTreeMesh::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;
}

void CTreeMesh::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
}

HRESULT CTreeMesh::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

	m_pShaderCom->Begin(0);

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS));

		m_pModelCom->Render(i, m_pShaderCom);
	}

	return S_OK;
}



//void CTreeMesh::On_EventMessege(EVENTDESC tEvent)
//{
//	if (LEVEL_CHANGE == tEvent.eEventType)
//	{
//		CLevel_Loading*	pLevel = (CLevel_Loading*)tEvent.wParam;
//		switch (pLevel->Get_NextLevel())
//		{
//		case Client::LEVEL_LOADING:
//			break;
//		case Client::LEVEL_MENU:
//			m_TextureIdx = 2;
//			break;
//		case Client::LEVEL_LOGO:
//			m_TextureIdx = 0;
//			break;
//		case Client::LEVEL_GAMEPLAY:
//			break;
//		case Client::LEVEL_EDIT:
//			break;
//		case Client::LEVEL_END:
//			break;
//		default:
//			break;
//		}
//	}
//}

HRESULT CTreeMesh::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Renderer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(RENDERER), ID_UINT(RENDERER), (CComponent**)&m_pRendererCom));

	/* For.Com_Texture */
	//FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(TEXTURE, TEXTURE_BACKGROUND), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	return S_OK;
}

HRESULT CTreeMesh::SetUp_ShaderResource()
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
void CTreeMesh::Input_Key(_float fTimeDelta)
{

}

HRESULT CTreeMesh::SetUp_Model(ID_UINT iPrototypeID)
{
	/* For.Com_Model */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, iPrototypeID, ID_UINT(MODEL), (CComponent**)&m_pModelCom));

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}

CTreeMesh* CTreeMesh::Create()
{
	CTreeMesh* pInstance = new CTreeMesh();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTreeMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTreeMesh::Clone(void* pArg)
{
	CTreeMesh* pInstance = new CTreeMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTreeMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTreeMesh::Free()
{
	__super::Free();
}
