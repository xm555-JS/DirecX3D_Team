#include "stdafx.h"
#include "..\Public\Inventory.h"

#include "GameInstance.h"


CInventory::CInventory()
	: CGameObject()
{
}

CInventory::CInventory(const CInventory & rhs)
	: CGameObject(rhs)
{
}

HRESULT CInventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInventory::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(__super::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tInvenDesc, pArg, sizeof(InvenDesc));
	}

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_INVEN, this);
	pGameInstance->Add_LateTickGroup(LTG_INVEN, this);
	return S_OK;
}

void CInventory::Tick(_float fTimeDelta)
{	

}

void CInventory::LateTick(_float fTimeDelta)
{
}

HRESULT CInventory::Render()
{

	return S_OK;
}

CInventory * CInventory::Create()
{
	CInventory*		pInstance = new CInventory();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInventory");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CInventory::Clone(void * pArg)
{
	CInventory*		pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory::Free()
{
	__super::Free();
}
