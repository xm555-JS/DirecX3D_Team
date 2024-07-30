#include "stdafx.h"
#include "..\Public\UI_Order_Result.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Order_Manager.h"

CUI_Order_Result::CUI_Order_Result()
	: CUI_Order()
{
}

CUI_Order_Result::CUI_Order_Result(const CUI_Order_Result & rhs)
	: CUI_Order(rhs)
{
}

HRESULT CUI_Order_Result::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Order_Result::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;


	SetUp_UIpos();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);
	return S_OK;
}

HRESULT CUI_Order_Result::Initialize_Pooling(void* pArg)
{
	m_fAlphaTime = 0.f;
	m_fSizeTime = 0.f;
	m_fAccTime = 0.f;
	m_fAlphaRatio = 1.f;
	m_fTimeDir = 1.f;

	RESET_ONCE(m_bCreate);

	CUI::Initialize_Pooling(pArg);

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;


	SetUp_UIpos();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);
	return S_OK;
}

void CUI_Order_Result::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	FadeOut_Black_Background(fTimeDelta);
	FadeInOut_Yellow_Light(fTimeDelta);
	Rotation_ItemBack_Circle(fTimeDelta);
	if (7 == m_tUIDesc.iTextureIdx)
	{
		USEGAMEINSTANCE;
		_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
		_float4 vCurPoint;

		m_fAccTime += fTimeDelta;

		if (1.f < m_fAccTime)
		{
			m_fAccTime = 1.f;
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAccTime, 1.f));
		m_fAlphaRatio = vCurPoint.x;
	}
}

void CUI_Order_Result::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Order_Result::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Order_Result::FadeOut_Black_Background(_float fTimeDelta)
{
	if (19 == m_tUIDesc.iTextureIdx)
	{
		USEGAMEINSTANCE;
		_vector vStartPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
		_float4 vCurPoint;

		m_fAccTime += fTimeDelta;

		if (0.6f < m_fAccTime)
		{
			IMPL_ONCE(m_bCreate,
				pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
					&CUI::UIDESC(640.f, 320.f, 500.f, 500.f, 0.f, 18, 455, { 0.f,0.f }, nullptr, m_tUIDesc.iIdx,m_tUIDesc.vFontPos));
				pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_ITEM), ID_UINT(UI, UI_ORDER_ITEM),
					&CUI::UIDESC(640.f, 320.f, 150.f, 150.f, 0.f, m_tUIDesc.iIdx, 450));
					);
		}
		if (0.8f < m_fAccTime)
		{
			pGameInstance->Add_PoolingObject(this);
			IMPL_ONCE(m_bSound0, MGR(CSound_Manager)->PlaySound(TEXT("UI_Order_Result.mp3"), CSound_Manager::CHANNEL_UI, 0.4f);
			);
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAccTime, 1.f));
		m_fAlphaRatio = vCurPoint.x;
	}
}

void CUI_Order_Result::FadeInOut_Yellow_Light(_float fTimeDelta)
{
	if (18 == m_tUIDesc.iTextureIdx)
	{
		USEGAMEINSTANCE;
		_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
		_float4 vCurPoint;

		m_fAccTime += fTimeDelta * m_fTimeDir;

		if (0.f > m_fAccTime)
		{
			pGameInstance->Add_PoolingObject(this);

			if (1 == m_tUIDesc.vFontPos.x)
			{
				Load_UIData("Order_ResultButton");
			}
		}
		if (1.f < m_fAccTime)
		{
			_uint iTextureIdx = 0;
			if (ORDER_HERMITCRAB < m_tUIDesc.iIdx)
			{
				iTextureIdx = 14;
			}
			else
			{
				iTextureIdx = 17;
			}


			IMPL_ONCE(m_bCreate,
				pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
					&CUI::UIDESC(640.f, 320.f, 400.f, 400.f, 0.f, iTextureIdx, 460));
				pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
					&CUI::UIDESC(356.f, 574.f, 541.f, 108.f, 0.f, 7, 460));
				pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_ITEMNAME), ID_UINT(UI, UI_ORDER_ITEMNAME),
					&CUI::UIDESC(365.f, 570.f, 557.f, 75.f, 0.f, m_tUIDesc.iIdx, 460)));

			m_fTimeDir *= -1.f;
			m_fAccTime = 1.f;
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAccTime, 1.f));
		m_fAlphaRatio = vCurPoint.x;
	}
}

void CUI_Order_Result::Rotation_ItemBack_Circle(_float fTimeDelta)
{
	if (14 == m_tUIDesc.iTextureIdx
		|| 17 == m_tUIDesc.iTextureIdx)
	{
		USEGAMEINSTANCE;
		_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
		_float4 vCurPoint;

		m_fAccTime += fTimeDelta;

		if (1.f < m_fAccTime)
		{
			m_fAccTime = 1.f;
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAccTime, 1.f));
		m_fAlphaRatio = vCurPoint.x;
		
		m_tUIDesc.fAngle += fTimeDelta * 5.f;
	}
}

HRESULT CUI_Order_Result::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ORDER_RESULT), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Order_Result::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));
	return S_OK;
}

CUI_Order_Result * CUI_Order_Result::Create()
{
	CUI_Order_Result*		pInstance = new CUI_Order_Result();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Order_Result");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Order_Result::Clone(void * pArg)
{
	CUI_Order_Result*		pInstance = new CUI_Order_Result(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Order_Result");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Order_Result::Free()
{
	__super::Free();
}
