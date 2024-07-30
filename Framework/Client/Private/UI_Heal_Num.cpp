#include "stdafx.h"
#include "..\Public\UI_Heal_Num.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"


CUI_Heal_Num::CUI_Heal_Num()
	: CUI()
{
}

CUI_Heal_Num::CUI_Heal_Num(const CUI_Heal_Num & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Heal_Num::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Heal_Num::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 13;

	m_tUIDesc.fInitSizeX = 140.f;
	m_tUIDesc.fInitSizeY = 167.5f;

	m_tUIDesc.vDeltaPos.x += 100.f;
	m_tUIDesc.vDeltaPos.y -= 200.f;
	SetUp_UIpos();
	
	if (m_tUIDesc.pGameObject)
	{
		SetUp_ViewportPos(m_tUIDesc.pGameObject);
		m_tUIDesc.fInitX = m_fX;
		m_tUIDesc.fInitY = m_fY;
	}

	m_fAlphaRatio = 0.f;
	XMStoreFloat4(&m_vTargerPos, m_tUIDesc.pGameObject->Get_Pos());

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

HRESULT CUI_Heal_Num::Initialize_Pooling(void* pArg)
{
	m_fAlphaRatio = 1.f;
	m_fAccTime = 0.f;
	m_fAlphaTime = 0.f;
	m_fStartTime = 0.f;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 13;

	m_tUIDesc.fInitSizeX = 140.f;
	m_tUIDesc.fInitSizeY = 167.5f;

	m_tUIDesc.vDeltaPos.x += 100.f;
	m_tUIDesc.vDeltaPos.y -= 200.f;
	SetUp_UIpos();

	if (m_tUIDesc.pGameObject)
	{
		SetUp_ViewportPos(m_tUIDesc.pGameObject);
		m_tUIDesc.fInitX = m_fX;
		m_tUIDesc.fInitY = m_fY;
	}

	m_fAlphaRatio = 0.f;
	XMStoreFloat4(&m_vTargerPos, m_tUIDesc.pGameObject->Get_Pos());

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Heal_Num::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	m_fStartTime += fTimeDelta;
	if (m_tUIDesc.pGameObject && m_tUIDesc.iIdx * 0.1f < m_fStartTime)
	{
		m_fAlphaRatio = 1.f;
		SetUp_ViewportPos(m_tUIDesc.pGameObject);
	}
	SetUp_Animation(fTimeDelta);
}

void CUI_Heal_Num::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Heal_Num::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Heal_Num::SetUp_Animation(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 1.f, 1.f);
	//_vector vTargetPoint = XMVectorSet(55.f, 65.803f, 1.f, 1.f);
	//_vector vTargetPoint = XMVectorSet(50.f, 59.821f, 1.f, 1.f);
	//_vector vTargetPoint = XMVectorSet(45.f, 53.838f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(30.f, 36.f, 1.f, 1.f);

	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;
	if (0.25f < m_fAccTime)
	{
		m_fAccTime = 0.25f;
		m_fAlphaTime += fTimeDelta;

		_vector vStartPos = XMVectorSet(1.f, m_tUIDesc.fInitY, 1.f, 1.f);
		_vector vTargetPos = XMVectorSet(0.2f, m_tUIDesc.fInitY - 30.f, 1.f, 1.f);

		_float4 vCurPos;

		if (0.65f < m_fAlphaTime)
		{
			m_fAlphaTime = 0.65f;
			GAMEINSTANCE->Add_PoolingObject(this);
			return;
		}

		XMStoreFloat4(&vCurPos, CEasing_Utillity::ExpoIn(vStartPos, vTargetPos, m_fAlphaTime, 0.65f));
		m_fAlphaRatio = vCurPos.x;
		m_fY = vCurPos.y;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fAccTime, 0.25f));

	m_fSizeX = vCurPoint.x;
	m_fSizeY = vCurPoint.y;
}

HRESULT CUI_Heal_Num::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_DAMAGE_NUMBER), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Heal_Num::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));
	_float3 vColor = { 0.388f, 0.933f, 0.525f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vColor", &vColor, sizeof(_float3)));

	return S_OK;
}

CUI_Heal_Num * CUI_Heal_Num::Create()
{
	CUI_Heal_Num*		pInstance = new CUI_Heal_Num();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Heal_Num");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Heal_Num::Clone(void * pArg)
{
	CUI_Heal_Num*		pInstance = new CUI_Heal_Num(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Heal_Num");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Heal_Num::Free()
{
	__super::Free();
}
