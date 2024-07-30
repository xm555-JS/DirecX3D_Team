#include "stdafx.h"
#include "..\Public\UI_Inventory.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Inventory::CUI_Inventory()
	: CUI()
{
}

CUI_Inventory::CUI_Inventory(const CUI_Inventory & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Inventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Inventory::Initialize(void * pArg)
{
	return S_OK;
}

void CUI_Inventory::Tick(_float fTimeDelta)
{	
}

void CUI_Inventory::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_Inventory::Render()
{
	return S_OK;
}

void CUI_Inventory::Create_Inven_Window(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;
	if (0.4f < m_fAccTime)
	{
		m_fAccTime = 0.4f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::QuarticIn(vStartPoint, vTargetPoint, m_fAccTime, 0.4f));
	m_fSizeRatioX = m_fAlphaRatio = vCurPoint.x;
}

HRESULT CUI_Inventory::SetUp_Components()
{
	return S_OK;
}

HRESULT CUI_Inventory::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

void CUI_Inventory::Free()
{
	__super::Free();
}
