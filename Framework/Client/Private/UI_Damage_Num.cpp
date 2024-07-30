#include "stdafx.h"
#include "..\Public\UI_Damage_Num.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"

CUI_Damage_Num::CUI_Damage_Num()
	: CUI()
{
}

CUI_Damage_Num::CUI_Damage_Num(const CUI_Damage_Num & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Damage_Num::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Damage_Num::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 13;

	/*m_tUIDesc.fInitSizeX = 140.f;
	m_tUIDesc.fInitSizeY = 167.5f;*/
	m_tUIDesc.fInitSizeX = 160.f;
	m_tUIDesc.fInitSizeY = 191.5f;

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

HRESULT CUI_Damage_Num::Initialize_Pooling(void* pArg)
{
	m_fAlphaRatio = 1.f;
	m_fAccTime = 0.f;
	m_fAlphaTime = 0.f;
	m_fStartTime = 0.f;
	m_bCritical = false;

	CTransform::TRANSFORMDESC		TransformDesc;

	//FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 13;

	/*m_tUIDesc.fInitSizeX = 140.f;
	m_tUIDesc.fInitSizeY = 167.5f;*/
	m_tUIDesc.fInitSizeX = 160.f;
	m_tUIDesc.fInitSizeY = 191.5f;

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

void CUI_Damage_Num::Tick(_float fTimeDelta)
{
	CUI::Tick(fTimeDelta);
	SetUp_Color();

	if (m_bCritical)
	{
		SetUp_Critical_Animation(fTimeDelta);
	}
	else
	{
		m_fStartTime += fTimeDelta;
		if (m_tUIDesc.pGameObject && m_tUIDesc.iIdx * 0.15f < m_fStartTime)
		{
			m_fAlphaRatio = 1.f;
			SetUp_ViewportPos(m_vTargerPos);
		}
		SetUp_Animation(fTimeDelta);
	}
}

void CUI_Damage_Num::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Damage_Num::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Damage_Num::SetUp_Color()
{
	switch (m_eElement)
	{
	case ELEMENTAL_LIGHTNING:
		m_vColor = { 0.85f, 0.68f, 0.984f }; // ∫∏∂Û
		break;
	
	case ELEMENTAL_FIRE:
		m_vColor = { 0.988f, 0.698f, 0.411f }; // ¡÷»≤
		break;
	
	case ELEMENTAL_ICE:
		m_vColor = { 0.45f, 0.917f, 0.984f }; // «œ¥√
		break;

	case ELEMENTAL_MELEE:
		m_vColor = { 0.98f, 0.98f, 0.98f }; //»Úªˆ
		break;
	}
}

void CUI_Damage_Num::SetUp_Animation(_float fTimeDelta)
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

void CUI_Damage_Num::SetUp_Critical_Animation(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(70.f, 84.f, 1.f, 1.f);

	if (11 == m_tUIDesc.iTextureIdx)
	{
		vTargetPoint = XMVectorSet(55.f, 65.f, 1.f, 1.f);
	}



	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;
	if (0.15f < m_fAccTime)
	{
		m_fAccTime = 0.25f;
		m_fAlphaTime += fTimeDelta;

		_vector vStartPos = XMVectorSet(1.f, m_tUIDesc.fInitY, 1.f, 1.f);
		_vector vTargetPos = XMVectorSet(0.f, m_tUIDesc.fInitY - 30.f, 1.f, 1.f);

		_float4 vCurPos;

		if (0.7f < m_fAlphaTime)
		{
			m_fAlphaTime = 0.7f;
			GAMEINSTANCE->Add_PoolingObject(this);
			return;
		}

		XMStoreFloat4(&vCurPos, CEasing_Utillity::ExpoIn(vStartPos, vTargetPos, m_fAlphaTime, 0.7f));
		m_fAlphaRatio = vCurPos.x;
		//m_fY = vCurPos.y;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fAccTime, 0.15f));

	m_fSizeX = vCurPoint.x;
	m_fSizeY = vCurPoint.y;
}

HRESULT CUI_Damage_Num::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_DAMAGE_NUMBER), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Damage_Num::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vColor", &m_vColor, sizeof(_float3)));


	return S_OK;
}

CUI_Damage_Num * CUI_Damage_Num::Create()
{
	CUI_Damage_Num*		pInstance = new CUI_Damage_Num();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Damage_Num");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Damage_Num::Clone(void * pArg)
{
	CUI_Damage_Num*		pInstance = new CUI_Damage_Num(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Damage_Num");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Damage_Num::Free()
{
	__super::Free();
}
