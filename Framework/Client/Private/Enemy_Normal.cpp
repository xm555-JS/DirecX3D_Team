#include "stdafx.h"
#include "..\Public\Enemy_Normal.h"
#include "State_Handler.h"

#include "GameInstance.h"
#include "UI_Solid.h"

CEnemy_Normal::CEnemy_Normal()
	: CEnemy()
{
}

CEnemy_Normal::CEnemy_Normal(const CEnemy_Normal & rhs)
	: CEnemy(rhs)
{
}

HRESULT CEnemy_Normal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Normal::Initialize(void * pArg)
{

	FAILEDCHECK(__super::Initialize(pArg));


	return S_OK;
}

void CEnemy_Normal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

HRESULT CEnemy_Normal::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	_float4 vLook;
	_matrix ViewMatrixInv;
	ViewMatrixInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW)));
	XMStoreFloat4(&vLook, XMVector3Normalize(ViewMatrixInv.r[2]));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bRimLight", &m_bUseRimLight, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vLook", &vLook, sizeof(_float4)));

	m_vRimLightColor = { 1.f, 1.f, 1.f, 1.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_RimLightColor", &m_vRimLightColor, sizeof(_float4)));

	_float fInterval = 1.f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fRimLightInterval", &fInterval, sizeof(_float)));

	__super::SetUp_ShaderResource();

	return S_OK;
}

HRESULT CEnemy_Normal::Create_UI_Status(_uint iNameTexIdx, _float fDeltaY, _bool bNamed)
{
#if defined(_UI)
	USEGAMEINSTANCE;
	CGameObject* pGameObject = nullptr;
	NULLCHECK(pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR), ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR), 
		&CUI_Solid::UISolidDesc(this, 0, { 0.f, fDeltaY })));
	NULLCHECK(pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR), ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR), 
		&CUI_Solid::UISolidDesc(this, 2, { 0.f, fDeltaY })));
	NULLCHECK(pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR), ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR), 
		&CUI_Solid::UISolidDesc(this, 1, { 0.f, fDeltaY })));
	NULLCHECK(pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UISOLID, UISOLID_ENEMY_NAME), ID_UINT(UISOLID, UISOLID_ENEMY_NAME), 
		&CUI_Solid::UISolidDesc(pGameObject, iNameTexIdx)));
	NULLCHECK(pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UISOLID, UISOLID_ENEMY_ATTACK), ID_UINT(UISOLID, UISOLID_ENEMY_ATTACK),
		&CUI_Solid::UISolidDesc(pGameObject, 0)));

	if (bNamed)
	{
		NULLCHECK(pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UISOLID, UISOLID_ENEMY_RANK), ID_UINT(UISOLID, UISOLID_ENEMY_RANK), &CUI_Solid::UISolidDesc(pGameObject, 1)));
	}
	else
	{
		NULLCHECK(pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UISOLID, UISOLID_ENEMY_RANK), ID_UINT(UISOLID, UISOLID_ENEMY_RANK), &CUI_Solid::UISolidDesc(pGameObject, 0)));
	}
#endif // _UI
	return S_OK;

}


void CEnemy_Normal::Free()
{
	__super::Free();
}
