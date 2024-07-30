#include "stdafx.h"
#include "..\Public\UI_Proceeding.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "State_Handler.h"

CUI_Proceeding::CUI_Proceeding()
	: CUI()
{
}

CUI_Proceeding::CUI_Proceeding(const CUI_Proceeding & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Proceeding::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Proceeding::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;

	if (1 == m_tUIDesc.iTextureIdx)
	{
		m_iPassIdx = 16;
	}

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Proceeding::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	if (2 == m_tUIDesc.iTextureIdx)
	{
		Resizing_EdgeRing(fTimeDelta);
	}
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	if (pPlayer)
	{
		CState_Handler* pPlayerHandler = (CState_Handler*)pPlayer->Get_Component(STATE_HANDLER);
		if (STATE_PLAYER_EAT != pPlayerHandler->Get_CurStateType())
		{
			GAMEINSTANCE->Add_DeadObject(this);
		}
	}

	if (1 == m_tUIDesc.iTextureIdx)
	{
		m_fProgressRatio += 0.357 * fTimeDelta;
	}
}

void CUI_Proceeding::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Proceeding::Render()
{
	FAILEDCHECK(CUI::Render());
	if (0 == m_tUIDesc.iTextureIdx)
	{
		GAMEINSTANCE->Render_Font(TEXT("Font_Hancomdodum_Bold"),TEXT("½Ä»ç Áß"), m_tUIDesc.vFontPos, XMLoadFloat4(&m_tUIDesc.vFontColor), m_tUIDesc.fFontSize);
	}
	return S_OK;
}

void CUI_Proceeding::Resizing_EdgeRing(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX - 15.f, m_tUIDesc.fInitSizeY - 15.f, 0.85f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 0.f, 1.f);

	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;
	if (0.65f < m_fAccTime)
	{
		m_fAccTime = 0.f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CircularIn(vStartPoint, vTargetPoint, m_fAccTime, 0.65f));

	m_fSizeX = vCurPoint.x;
	m_fSizeY = vCurPoint.y;
	m_fAlphaRatio = vCurPoint.z;
}

HRESULT CUI_Proceeding::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_PROCEEDING), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Proceeding::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fProgressRatio", &m_fProgressRatio, sizeof(_float)));
	return S_OK;
}

CUI_Proceeding * CUI_Proceeding::Create()
{
	CUI_Proceeding*		pInstance = new CUI_Proceeding();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Proceeding");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Proceeding::Clone(void * pArg)
{
	CUI_Proceeding*		pInstance = new CUI_Proceeding(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Proceeding");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Proceeding::Free()
{
	__super::Free();
}
