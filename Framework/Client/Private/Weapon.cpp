#include "stdafx.h"
#include "..\Public\Weapon.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"

#include "Imgui_Manager.h"

#include "Light.h"

CWeapon::CWeapon()
	: CGameObject()
{

}

CWeapon::CWeapon(const CWeapon & rhs)
	: CGameObject(rhs)
{

}

void CWeapon::Setup_WeaponState(CGameObject* pTargetObject, string strTargetBoneName)
{
	m_pTarget = pTargetObject;
	m_strTargetBoneName = strTargetBoneName;

	CModel* pTargetModel = nullptr;
	NULLCHECK(pTargetModel = (CModel*)m_pTarget->Get_Component(MODEL_DYNAMIC));
	NULLCHECK(m_pBonePtr = pTargetModel->Find_HierarcyNode(m_strTargetBoneName.c_str()));
	NULLCHECK(m_pParentTransformCom = (CTransform*)m_pTarget->Get_Component(TRANSFORM));

	//USEGAMEINSTANCE;
	//pGameInstance->Add_TickGroup(TG_WEAPON, this);
	//pGameInstance->Add_LateTickGroup(LTG_WEAPON, this);
}

HRESULT CWeapon::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWeapon::Initialize(void * pArg)
{
	USEGAMEINSTANCE;

	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));

	m_fTimeDurationDissolve = 0.5f;
	m_fTimeAccDissolve = m_fTimeDurationDissolve;
	m_fDissolveStrength = 0.1f;
	m_iDissolveTextureNoise = 28;
	m_iDissolveTextureColor = 1;

	return S_OK;
}

void CWeapon::Tick(_float fTimeDelta)
{
	if (nullptr == m_pBonePtr)
		return;

	if (m_bDissolve && !m_bUsing)
	{
		if (m_fTimeAccDissolve < m_fTimeDurationDissolve)
			m_fTimeAccDissolve += fTimeDelta * 2.f;
		else
			m_fTimeAccDissolve = m_fTimeDurationDissolve;
	}
	else if (m_bDissolve && m_bUsing)
	{
		if (0 < m_fTimeAccDissolve)
			m_fTimeAccDissolve -= fTimeDelta * 2.f;
		else
			m_fTimeAccDissolve = 0.f;
	}
	else
	{
		m_fTimeAccDissolve = 0.f;
	}

	_matrix		ParentMatrix = m_pBonePtr->Get_OffsetMatrix() * m_pBonePtr->Get_CombinedMatrix() * m_pBonePtr->Get_TransformMatrix();
	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * ParentMatrix * m_pParentTransformCom->Get_WorldMatrix());
}

void CWeapon::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
}

HRESULT CWeapon::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());
	

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

		m_pModelCom->Render(i, m_pShaderCom);
	}

	_bool bDissolve = false;
	_float fResetDissove = 0.f;
	_float4 vResetDissove = _float4(0.f, 0.f, 0.f, 0.f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsDissolve", &bDissolve, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vDissolveColor", &vResetDissove, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fDissolveStrength", &fResetDissove, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fTimeAccDissolve", &fResetDissove, sizeof(_float)));

	return S_OK;
}

HRESULT CWeapon::Render(void* pArg)
{
	if(m_bUsing)
	{
		RENDERDESC	Desc;
		memcpy(&Desc, pArg, sizeof(RENDERDESC));

		if (RENDERTYPE_DEPTH == Desc.eType)
		{
			_float4x4		WorldMatrixTranspose;

			XMStoreFloat4x4(&WorldMatrixTranspose, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

			FAILEDCHECK(m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrixTranspose, sizeof(_float4x4)));

			CLight* pTargetLight = (CLight*)Desc.lParam;
			pTargetLight->SetUp_DepthComponents(m_pShaderCom);

			//m_pShaderCom->Begin(3);

			for (_uint i = 0; i < m_iNumMeshContainers; ++i)
			{
				FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

				m_pModelCom->Render(i, m_pShaderCom, 2, "g_Bones");
			}
		}
	}

	return S_OK;
}

HRESULT CWeapon::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

#if defined(_NOISE_TEX)
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE), (CComponent**)&m_pDissolveTextureCom));
#endif
	return S_OK;
}

HRESULT CWeapon::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float4x4		WorldMatrixTranspose;

	XMStoreFloat4x4(&WorldMatrixTranspose, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrixTranspose, sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsDissolve", &m_bDissolve, sizeof(_bool)));
#if defined(_NOISE_TEX)
	FAILEDCHECK(m_pDissolveTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DissolveNoiseTexture", m_iDissolveTextureNoise));
	FAILEDCHECK(m_pDissolveTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DissolveColorTexture", m_iDissolveTextureColor));
#endif
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fDissolveStrength", &m_fDissolveStrength, sizeof(_float)));
	_float fTimeAccDissove = 0.f;
	fTimeAccDissove = (m_fTimeAccDissolve / m_fTimeDurationDissolve) * (1.f + m_fDissolveStrength); // 디졸드 지속하고 싶은 시간 만큼 나눠줌.
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fTimeAccDissolve", &fTimeAccDissove, sizeof(_float)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &m_bGlow, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vGlowColor", &m_vGlowColor, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float)));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CWeapon::Free()
{
	__super::Free();
}
