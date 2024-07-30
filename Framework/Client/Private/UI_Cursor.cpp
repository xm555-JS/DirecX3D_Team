#include "stdafx.h"
#include "..\Public\UI_Cursor.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Manager.h"

CUI_Cursor::CUI_Cursor()
	: CUI()
{
}

CUI_Cursor::CUI_Cursor(const CUI_Cursor & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Cursor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Cursor::Initialize(void * pArg)
{
	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&pArg));

	m_iPassIdx = 0;
	m_tUIDesc.iRenderLayer = 0.f;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	//m_fSizeX = g_iWinCX;
	//m_fSizeY = g_iWinCY;
	//m_vCursorSize[MODE_DEFAULT] = { 14.f, 18.f };
	//m_vCursorSize[MODE_GAMEPLAY] = { 16.f, 16.f };
	//m_vCursorSize[MODE_NONGRAB] = { 50.f, 50.f };
	//m_vCursorSize[MODE_GRAB] = { 50.f, 50.f };
	////m_vCursorSize[MODE_END] = { 1.f, 1.f };


	//m_fX = g_iWinCX >> 1;
	//m_fY = g_iWinCY >> 1;

	//USEGAMEINSTANCE;

	//XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

	//m_tUIDesc.fInitX = g_iWinCX >> 1;
	//m_tUIDesc.fInitY = g_iWinCY >> 1;
	m_tUIDesc.fInitSizeX = 35.f;
	m_tUIDesc.fInitSizeY = 35.f;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_ALERT, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ALERT, this);

	return S_OK;
}

void CUI_Cursor::Tick(_float fTimeDelta)
{
	//if (MODE_END == m_eCurCursorMode)
	//{
	//	m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 0.f));
	//}
	//else
	//{
	//	m_pTransformCom->Set_Scaled(_float3(m_vCursorSize[m_eCurCursorMode].x, m_vCursorSize[m_eCurCursorMode].y, 0.f));
	//}

	//if (MODE_DEFAULT == m_eCurCursorMode)
	//{
	//	_float3	vPos = Get_MousePos(g_hWnd);
	//	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(vPos.x - (g_iWinCX * 0.5f), -vPos.y + (g_iWinCY * 0.5f), 0.f, 1.f));
	//}
	//else
	//{
	//	//m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	//	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));
	//}
#ifdef _DEBUG
	ShowCursor(true);
#else
	ShowCursor(false);
#endif // _DEBUG

	
	_float3 vMousePos = Get_MousePos(g_hWnd);

	m_fX = vMousePos.x + m_fSizeX * 0.5f - 4.f;
	m_fY = vMousePos.y + m_fSizeY * 0.5f - 2.f;

	USEGAMEINSTANCE;
	m_bRender = pGameInstance->Get_CursorFix();

	CUI::Tick(fTimeDelta);
}

void CUI_Cursor::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Cursor::Render()
{
	if (!m_bRender)
	{
		FAILEDCHECK(CUI::Render());
	}

	return S_OK;
}


void CUI_Cursor::On_EventMessege(EVENTDESC tEvent)
{
	/*if (EVENT_PICKING == tEvent.eEventType)
	{

	}*/
} 

HRESULT CUI_Cursor::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_CURSOR), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Cursor::SetUp_ShaderResource()
{
	//NULLCHECK(m_pShaderCom);

	///*if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
	//	__debugbreak();*/

	//FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));
	//FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iTextureIdx));
	
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Cursor * CUI_Cursor::Create()
{
	CUI_Cursor*		pInstance = new CUI_Cursor();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Cursor");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Cursor::Clone(void * pArg)
{
	CUI_Cursor*		pInstance = new CUI_Cursor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Cursor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Cursor::Free()
{
	__super::Free();
}
