#include "stdafx.h"
#include "AnimGround.h"

#include "GameInstance.h"

CAnimGround::CAnimGround()
	: CGameObject()
{
}

CAnimGround::CAnimGround(const CAnimGround & rhs)
	: CGameObject(rhs)
{
}

HRESULT CAnimGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimGround::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(__super::Initialize(&TransformDesc));

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_DEFAULT, this);
	pGameInstance->Add_LateTickGroup(LTG_DEFAULT, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
	m_pTransformCom->Set_Scaled(_float3(16.f, 16.f, 16.f));


	return S_OK;
}

void CAnimGround::Tick(_float fTimeDelta)
{	
}

void CAnimGround::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (!KEY_PRESSING(VK_SHIFT))
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}
}

HRESULT CAnimGround::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CAnimGround::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEX), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(TEXTURE, TEXTURE_BACKGROUND), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CAnimGround::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);
	USEGAMEINSTANCE;
		
	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));
	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0));

	return S_OK;
}

CAnimGround * CAnimGround::Create()
{
	CAnimGround*		pInstance = new CAnimGround();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAnimGround");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CAnimGround::Clone(void * pArg)
{
	CAnimGround*		pInstance = new CAnimGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAnimGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimGround::Free()
{
	__super::Free();
}
