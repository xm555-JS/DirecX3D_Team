#include "stdafx.h"
#include "..\Public\Sky.h"

#include "GameInstance.h"

CSky::CSky()
	: CGameObject()
{
}

CSky::CSky(const CSky & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSky::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSky::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	if (FAILED(__super::Initialize(&TransformDesc)))
		__debugbreak();

	if (FAILED(SetUp_Components()))
		__debugbreak();

	USEGAMEINSTANCE;
	pGameInstance->Add_CallbackList(this, EVENT_CHANGESKYBOX);

	pGameInstance->Add_TickGroup(TG_DEFAULT, this);
	pGameInstance->Add_LateTickGroup(LTG_DEFAULT, this);

	return S_OK;
}

void CSky::Tick(_float fTimeDelta)
{
	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 0.05f);
}

void CSky::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&pGameInstance->Get_CamPosition()));

	//RELEASE_INSTANCE(CGameInstance);
	pGameInstance->Add_RenderGroup(RENDER_PRIORITY, this);
}

HRESULT CSky::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

	m_pShaderCom->Begin(0);
	 
	m_pVIBufferCom->Render();

	return S_OK;
}

void CSky::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_CHANGESKYBOX == tEvent.eEventType)
	{
		m_iTextureIdx = (_uint)tEvent.lParam;
	}
}


HRESULT CSky::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXCUBETEX),ID_UINT(SHADER), (CComponent**)&m_pShaderCom)))
		__debugbreak();

	///* For.Com_Renderer */
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, ID_UINT(RENDERER), ID_UINT(RENDERER), (CComponent**)&m_pRendererCom)))
	//	__debugbreak();

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iCurLevel, ID_UINT(TEXTURE, TEXTURE_SKY), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom)))
		__debugbreak();

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_CUBE), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom)))
		__debugbreak();

	return S_OK;
}

HRESULT CSky::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		__debugbreak();
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		__debugbreak();
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		__debugbreak();
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 6)))
		__debugbreak();
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CSky * CSky::Create()
{
	CSky*		pInstance = new CSky();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSky");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CSky::Clone(void * pArg)
{
	CSky*		pInstance = new CSky(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSky");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSky::Free()
{
	__super::Free();
}
