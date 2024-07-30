#include "stdafx.h"
#include "..\Public\UI_Gun_CrossHair.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "State_Handler.h"

CUI_Gun_CrossHair::CUI_Gun_CrossHair()
	: CUI()
{
}

CUI_Gun_CrossHair::CUI_Gun_CrossHair(const CUI_Gun_CrossHair & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Gun_CrossHair::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Gun_CrossHair::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	m_pPlayer = (CPlayer*)PLAYERPTR;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Gun_CrossHair::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	if (1 == m_tUIDesc.iTextureIdx)
	{
		Resizing_CrossHair(fTimeDelta);
	}
}

void CUI_Gun_CrossHair::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Gun_CrossHair::Render()
{
	if (m_pPlayer)
	{
		CState_Handler* pPlayerHandler = (CState_Handler*)m_pPlayer->Get_Component(STATE_HANDLER);

		if (STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_ZOOM == pPlayerHandler->Get_CurStateType())
		{
			FAILEDCHECK(CUI::Render());
		}
	}
	return S_OK;
}

HRESULT CUI_Gun_CrossHair::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_2GUN_CROSSHAIR), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Gun_CrossHair::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

void CUI_Gun_CrossHair::Resizing_CrossHair(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX + 13.f, m_tUIDesc.fInitSizeY + 13.f, 1.f, 1.f);

	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;
	if (0.13f < m_fAccTime)
	{
		//m_fAccTime = 0.15f;
		m_fAccTime = 0.f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAccTime, 0.13f));
	m_fSizeX = vCurPoint.x;
	m_fSizeY = vCurPoint.y;
}

CUI_Gun_CrossHair * CUI_Gun_CrossHair::Create()
{
	CUI_Gun_CrossHair*		pInstance = new CUI_Gun_CrossHair();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Gun_CrossHair");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Gun_CrossHair::Clone(void * pArg)
{
	CUI_Gun_CrossHair*		pInstance = new CUI_Gun_CrossHair(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Gun_CrossHair");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Gun_CrossHair::Free()
{
	__super::Free();
}
