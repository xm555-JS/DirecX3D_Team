#include "stdafx.h"
#include "..\Public\Enemy_Boss.h"
#include "PhysXCollider.h"
#include "Math_Utillity.h"
#include "Easing_Utillity.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "UI.h"

#include "Navigation.h"

#include "Imgui_Manager.h"

CEnemy_Boss::CEnemy_Boss()
	: CEnemy()
{
}

CEnemy_Boss::CEnemy_Boss(const CEnemy_Boss& rhs)
	: CEnemy(rhs)
{
}

HRESULT CEnemy_Boss::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Boss::Initialize(void * pArg)
{
	FAILEDCHECK(__super::Initialize(pArg));

	return S_OK;
}

void CEnemy_Boss::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	__super::Tick(fTimeDelta);
}

void CEnemy_Boss::LateTick(_float fTimeDelta)
{

}

HRESULT CEnemy_Boss::Render()
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
		//FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS));

		m_pModelCom->Render(i, m_pShaderCom, 0, "g_Bones");
	}

	_bool bUseRimLight = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bRimLight", &bUseRimLight, sizeof(_bool)));

	return S_OK;
}

HRESULT CEnemy_Boss::Create_UI_Status(string strFileName)
{
#if defined(_UI)
	USEGAMEINSTANCE;
	json Data;

	wstring strFolder = TEXT("UI/");
	wstring strTemp = towstring(strFileName);
	wstring strFullName = strFolder + strTemp;

	const wchar_t* szName = strFullName.c_str();
	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(szName).c_str(), &Data);

	_uint iPrototypeID;

	for (auto& elem : Data)
	{
		CUI::UIDesc UIDesc;
		string strTemp = "";
		iPrototypeID = elem["iPrototypeID"];
		UIDesc.fInitX = elem["fInitX"];
		UIDesc.fInitY = elem["fInitY"];
		UIDesc.fInitSizeX = elem["fInitSizeX"];
		UIDesc.fInitSizeY = elem["fInitSizeY"];
		UIDesc.fAngle = elem["fAngle"];
		UIDesc.iTextureIdx = elem["iTextureIdx"];
		UIDesc.iRenderLayer = elem["iRenderLayer"];
		strTemp = elem["strText"];
		UIDesc.vFontPos.x = elem["vFontPosx"];
		UIDesc.vFontPos.y = elem["vFontPosy"];
		UIDesc.vFontColor.x = elem["vFontColorx"];
		UIDesc.vFontColor.y = elem["vFontColory"];
		UIDesc.vFontColor.z = elem["vFontColorz"];
		UIDesc.vFontColor.w = elem["vFontColorw"];
		UIDesc.fFontSize = elem["fFontSize"];
		UIDesc.bRenderFontText = elem["bRenderFontText"];
		UIDesc.bRenderFontNum = elem["bRenderFontNum"];
		UIDesc.pGameObject = this;

		ID_UINT IDPrototype;
		IDPrototype.iID = iPrototypeID;

		CGameObject* pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, IDPrototype, IDPrototype, &UIDesc);
		CUI* pUI = (CUI*)pGameObject;
		pUI->Set_FontText(strTemp);
	}
#endif
	return S_OK;
}


HRESULT CEnemy_Boss::SetUp_Components()
{
	return S_OK;
}

HRESULT CEnemy_Boss::SetUp_ShaderResource()
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

	m_vRimLightColor = { 0.5f, 0.5f, 0.5f, 1.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_RimLightColor", &m_vRimLightColor, sizeof(_float4)));

	_float fInterval = 1.f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fRimLightInterval", &fInterval, sizeof(_float)));

	__super::SetUp_ShaderResource();

	return S_OK;
}

void CEnemy_Boss::Push_EachOther(CCollider* pTargetCol, CCollider* pVolumeCol, CTransform* pTargetTransform)
{
	_float fTargetColRadius = pTargetCol->Get_ColliderDesc()->vScale.x * 0.5f;
	_float fColRadius = pVolumeCol->Get_ColliderDesc()->vScale.x * 0.5f;

	CGameObject* pTarget = pTargetCol->Get_Owner();

	_vector vTargetPos = pTargetCol->Get_Owner()->Get_Pos();
	_vector vPos = pVolumeCol->Get_Owner()->Get_Pos();
	_vector vDir = vPos - vTargetPos;
	_vector vTargetDir = vTargetPos - vPos;

	_float fLength = XMVectorGetX(XMVector3Length(vDir));
	_float fPushLength = (fColRadius + fTargetColRadius - fLength);

	vDir = XMVector3Normalize(vDir) * fPushLength;
	vTargetDir = XMVector3Normalize(vTargetDir) * fPushLength;

	vTargetPos += vTargetDir;

	//((CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM)))->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
	CNavigation* pTargetNaviCom = dynamic_cast<CNavigation*>(pTarget->Get_Component(ID_UINT(NAVIGATION)));
	((CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM)))->MoveTo(vTargetPos, pTargetNaviCom);
}


void CEnemy_Boss::Free()
{
	__super::Free();
}
