#include "stdafx.h"
#include "..\Public\UI_Minimap.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "Imgui_Manager.h"

CUI_Minimap::CUI_Minimap()
	: CUI()
{
}

CUI_Minimap::CUI_Minimap(const CUI_Minimap & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Minimap::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Minimap::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;

	if (0 == m_tUIDesc.iTextureIdx)
	{
		m_iPassIdx = 18;
	}
	
	USEGAMEINSTANCE;

	SetUp_UIpos();

	m_pPlayer = (CPlayer*)PLAYERPTR;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Minimap::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	if (0 == m_tUIDesc.iTextureIdx)
	{
		_float4 vPlayerPos;
		XMStoreFloat4(&vPlayerPos, m_pPlayer->Get_Pos());
		//_float	fTemp = DEBUGFLOAT;
		//m_vMapPos = { (vPlayerPos.x + 200.f * 0.5f) / (512.f + 200.f), 1.f - (vPlayerPos.z + 200.f * 0.5f) / (512.f + 200.f) };
		//m_vMapPos = { (vPlayerPos.x + (fTemp) * 0.5f) / (512.f + fTemp), 1.f - (vPlayerPos.z + (fTemp) * 0.5f) / (512.f + fTemp) };
		m_vMapPos = { (vPlayerPos.x + (193.1f) * 0.5f) / (512.f + 193.1f), 1.f - (vPlayerPos.z + (193.1f) * 0.5f) / (512.f + 193.1f) };
		//m_vMapPos = { (vPlayerPos.x * 0.5f) / 512.f , 1.f - (vPlayerPos.z  * 0.5f) / 512.f };
		//m_vMapPos.x *= DEBUGFLOAT4(0).x;
		//m_vMapPos.y *= DEBUGFLOAT4(0).y;
	}
}

void CUI_Minimap::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Minimap::Render()
{
	FAILEDCHECK(CUI::Render());
	
	return S_OK;
}

HRESULT CUI_Minimap::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_MINIMAP), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Minimap::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	
	if (0 == m_tUIDesc.iTextureIdx)
	{
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vMapPos", &m_vMapPos, sizeof(_float2)));
	}
	return S_OK;
}

CUI_Minimap * CUI_Minimap::Create()
{
	CUI_Minimap*		pInstance = new CUI_Minimap();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Minimap");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Minimap::Clone(void * pArg)
{
	CUI_Minimap*		pInstance = new CUI_Minimap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Minimap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minimap::Free()
{
	__super::Free();
}
