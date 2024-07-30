#include "stdafx.h"
#include "..\Public\UI_Order.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Order::CUI_Order()
	: CUI()
{
}

CUI_Order::CUI_Order(const CUI_Order & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Order::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Order::Initialize(void * pArg)
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

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);

	return S_OK;
}

void CUI_Order::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
}

void CUI_Order::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Order::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Order::Create_Change_Alpha(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(0.f, 0.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 0.35f, 1.f, 1.f);
	_float4 vCurPoint;

	m_fAlphaTime += fTimeDelta;

	if (0.6f < m_fAlphaTime)
	{
		m_fAlphaTime = 0.6f;
		if (1 == m_tUIDesc.iTextureIdx)
		{
			m_tUIDesc.bRenderFontNum = true;
		}
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAlphaTime, 0.6f));
	m_fAlphaRatio = vCurPoint.x;
}

void CUI_Order::Create_Change_Size(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX * 1.2f, m_tUIDesc.fInitSizeY * 1.2f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 1.f, 1.f);
	_float4 vCurPoint;

	m_fSizeTime += fTimeDelta;

	if (0.6f < m_fSizeTime)
	{
		m_fSizeTime = 0.6f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fSizeTime, 0.6f));
	m_fSizeX = vCurPoint.x;
	m_fSizeY = vCurPoint.y;
}

void CUI_Order::Create_Change_Pos(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitY + 200.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitY, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	m_fMoveTime += fTimeDelta;

	if (0.6f < m_fMoveTime)
	{
		m_fMoveTime = 0.6f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fMoveTime, 0.6f));
	m_fY = vCurPoint.x;
}

HRESULT CUI_Order::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_LOGO), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Order::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

void CUI_Order::Free()
{
	__super::Free();
}
