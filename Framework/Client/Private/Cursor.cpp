#include "stdafx.h"
#include "..\Public\Cursor.h"

#include "GameInstance.h"
#include "Level_Loading.h"

CCursor::CCursor()
	: CUI()
{
}

CCursor::CCursor(const CCursor & rhs)
	: CUI(rhs)
{
}

HRESULT CCursor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCursor::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	if (FAILED(CGameObject::Initialize(&TransformDesc)))
		__debugbreak();

	if (FAILED(SetUp_Components()))
		__debugbreak();

	memcpy(&m_iTextureIdx, pArg, sizeof(_uint));

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_vCursorSize[MODE_DEFAULT] = { 14.f, 18.f };
	m_vCursorSize[MODE_GAMEPLAY] = { 16.f, 16.f };
	m_vCursorSize[MODE_NONGRAB] = { 50.f, 50.f };
	m_vCursorSize[MODE_GRAB] = { 50.f, 50.f };
	//m_vCursorSize[MODE_END] = { 1.f, 1.f };


	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	USEGAMEINSTANCE;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));


	return S_OK;
}

void CCursor::Tick(_float fTimeDelta)
{
	if (MODE_END == m_eCurCursorMode)
	{
		m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 0.f));
	}
	else
	{
		m_pTransformCom->Set_Scaled(_float3(m_vCursorSize[m_eCurCursorMode].x, m_vCursorSize[m_eCurCursorMode].y, 0.f));
	}
	 
	if (MODE_DEFAULT == m_eCurCursorMode)
	{
		_float3	vPos = Get_MousePos(g_hWnd);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(vPos.x - (g_iWinCX * 0.5f), -vPos.y + (g_iWinCY * 0.5f), 0.f, 1.f));
	}
	else
	{
		//m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));
	}
}

void CCursor::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CCursor::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}


void CCursor::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_PICKING == tEvent.eEventType)
	{

	}

}

HRESULT CCursor::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEX), ID_UINT(SHADER), (CComponent**)&m_pShaderCom)))
		__debugbreak();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, ID_UINT(RENDERER), ID_UINT(RENDERER), (CComponent**)&m_pRendererCom)))
		__debugbreak();

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iCurLevel, ID_UINT(TEXTURE, TEXTURE_CURSOR), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom)))
		__debugbreak();

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom)))
		__debugbreak();

	return S_OK;
}

HRESULT CCursor::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		__debugbreak();*/
	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		__debugbreak();
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		__debugbreak();
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		__debugbreak();

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_eCurCursorMode)))
		__debugbreak();


	return S_OK;
}

CCursor * CCursor::Create()
{
	CCursor*		pInstance = new CCursor();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCursor");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CCursor::Clone(void * pArg)
{
	CCursor*		pInstance = new CCursor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCursor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCursor::Free()
{
	__super::Free();




}
