#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Transform.h"


CGameObject::CGameObject()
	: m_pTransformCom(CTransform::Create())
{
	 
}

CGameObject::CGameObject(const CGameObject & Prototype)
	: m_pTransformCom((CTransform*)Prototype.m_pTransformCom->Clone())
	, m_iCurLevel(Prototype.m_iCurLevel)
{
}

CComponent * CGameObject::Get_Component(ID_UINT iComponentID)
{
	return Find_Components(iComponentID);
}

void CGameObject::Setup_ComponentDead()
{
	for (auto& Pair : m_Components)
	{
		Pair.second->Set_Disable();
		Pair.second->Set_Dead();
	}
}

void CGameObject::Setup_ComponentDisable()
{
	for (auto& Pair : m_Components)
		Pair.second->Set_Disable();
}

void CGameObject::Setup_ComponentAble()
{
	for (auto& Pair : m_Components)
		Pair.second->Set_Able();
}

HRESULT CGameObject::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		/*  멤버변수에 트랜스폼에 대한 셋팅은 끝났다. */
		m_pTransformCom->Set_TransformDesc((CTransform::TRANSFORMDESC*)pArg);
	}

	USEGAMEINSTANCE;

	m_Components.emplace(pGameInstance->Get_TransformID().iID, m_pTransformCom);

	m_pWorldMatrix = m_pTransformCom->Get_WorldMatrix_ptr();
	m_pCombinedTransformationMatrix = m_pTransformCom->Get_CombinedTransformationMatrix_ptr();

	Safe_AddRef(m_pTransformCom);

	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, ID_UINT iPrototypeID, ID_UINT iComponentID, CComponent** ppOut, void * pArg)
{
	if (nullptr != Find_Components(iComponentID))
	{
		return S_OK;
		//__debugbreak();
	}

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

 	CComponent*		pComponent = pGameInstance->Clone_Component(iLevelIndex, iPrototypeID, pArg);
	if (nullptr == pComponent)
	{
		Safe_Release(pGameInstance);
		__debugbreak();
	}

	m_Components.emplace(iComponentID.iID, pComponent);
	*ppOut = pComponent;
	//Safe_AddRef(pComponent);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CGameObject::Remove_Component(ID_UINT iComponentID)
{
	CComponent* pComponent = Find_Components(iComponentID);

	if(nullptr == pComponent)
		__debugbreak();

	USEGAMEINSTANCE;
	//pGameInstance->Add_DeadBase(pComponent);
	Safe_Release(pComponent);

	m_Components.erase(iComponentID.iID);

	return S_OK;
}

HRESULT CGameObject::Change_Component(_uint iLevelIndex, ID_UINT iPrototypeID, ID_UINT iComponentID, CComponent ** ppOut, void * pArg)
{
	Remove_Component(iComponentID);
	Add_Component(iLevelIndex, iPrototypeID, iComponentID, ppOut, pArg);
	return S_OK;
}

_bool CGameObject::IsInFrustum(void)
{
	_bool bInFrustum = GAMEINSTANCE->isIn_Frustum_InWorldSpace(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), m_fFrustumRange);

	if (false == bInFrustum)
		m_bIsRender = false;

	return bInFrustum;
}

CComponent * CGameObject::Find_Components(ID_UINT iComponentID)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CID_Finder(iComponentID.iID));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Compute_CamDistance()
{
	USEGAMEINSTANCE;

	_vector vCamDir;
	_float4 vCamPos = pGameInstance->Get_CamPosition();

	vCamDir = XMLoadFloat4(&vCamPos) - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	m_fCamDistance = XMVectorGetX(XMVector3Length(vCamDir));

	return S_OK;
}

HRESULT CGameObject::SetUp_PoolingState(_uint iLevelIndex, _uint tLayerID, _uint tPrototypeID)
{
	m_iCurLevel = iLevelIndex;
	m_iLayerID.iID = tLayerID;
	m_iPrototypeID.iID = tPrototypeID;

	m_bClone = true;
	m_bDisable = false;
	m_bDead = false;

	return S_OK;
}

void CGameObject::Free()
{
	if (Is_Clone())
	{
		for (auto& Pair : m_Components)
			Safe_Release(Pair.second);

		m_Components.clear();
	}

	Safe_Release(m_pTransformCom);
}
