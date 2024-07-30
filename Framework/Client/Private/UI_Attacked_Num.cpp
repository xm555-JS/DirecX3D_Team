#include "stdafx.h"
#include "..\Public\UI_Attacked_Num.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"

CUI_Attacked_Num::CUI_Attacked_Num()
	: CUI()
{
}

CUI_Attacked_Num::CUI_Attacked_Num(const CUI_Attacked_Num & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Attacked_Num::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Attacked_Num::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 13;

	//m_tUIDesc.fInitSizeX = 40.f;
	//m_tUIDesc.fInitSizeY = 47.856f;


	m_tUIDesc.fInitSizeX = 22.f;
	m_tUIDesc.fInitSizeY = 25.f;

	m_tUIDesc.vDeltaPos.x += 100.f;
	m_tUIDesc.vDeltaPos.y -= 100.f;

	SetUp_UIpos();
	
	if (m_tUIDesc.pGameObject)
	{
		SetUp_ViewportPos(m_tUIDesc.pGameObject);
		m_tUIDesc.fInitX = m_fX;
		m_tUIDesc.fInitY = m_fY;
	}
	XMStoreFloat4(&m_vTargerPos, m_tUIDesc.pGameObject->Get_Pos());

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

HRESULT CUI_Attacked_Num::Initialize_Pooling(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	//FAILEDCHECK(CUI::Initialize(&TransformDesc));

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 13;

	m_tUIDesc.fInitSizeX = 22.f;
	m_tUIDesc.fInitSizeY = 25.f;

	m_tUIDesc.vDeltaPos.x += 100.f;
	m_tUIDesc.vDeltaPos.y -= 100.f;

	SetUp_UIpos();

	if (m_tUIDesc.pGameObject)
	{
		SetUp_ViewportPos(m_tUIDesc.pGameObject);
		m_tUIDesc.fInitX = m_fX;
		m_tUIDesc.fInitY = m_fY;
	}
	XMStoreFloat4(&m_vTargerPos, m_tUIDesc.pGameObject->Get_Pos());

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);

	m_fAlphaRatio = 1.f;
	m_fAccTime = 0.f;
	m_fAlphaTime = 0.f;
	m_fStartTime = 0.f;

	return S_OK;
}

void CUI_Attacked_Num::Tick(_float fTimeDelta)
{
	CUI::Tick(fTimeDelta);
	m_fStartTime += fTimeDelta;
	if (m_tUIDesc.pGameObject)
	{
		SetUp_ViewportPos(m_tUIDesc.pGameObject);
	}
	SetUp_Animation(fTimeDelta);
}

void CUI_Attacked_Num::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Attacked_Num::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Attacked_Num::SetUp_Animation(_float fTimeDelta)
{
	if (Is_Dead())
	{
		return;
	}
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitX, m_tUIDesc.fInitY, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitX + 3.f, m_tUIDesc.fInitY - 10.f, 1.f, 1.f);

	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;
	if (0.25f < m_fAccTime)
	{
		m_fAccTime = 0.25f;
		m_fAlphaTime += fTimeDelta;

		_vector vStartPos = XMVectorSet(m_tUIDesc.fInitX + 3.f, m_tUIDesc.fInitY - 10.f, 1.f, 1.f);
		_vector vTargetPos = XMVectorSet(m_tUIDesc.fInitX + 8.f, m_tUIDesc.fInitY + 40.f, 0.3f, 1.f);

		_float4 vCurPos;

		if (0.6f < m_fAlphaTime)
		{
			m_fAlphaTime = 0.6f;
			GAMEINSTANCE->Add_PoolingObject(this);
			return;
		}

		XMStoreFloat4(&vCurPos, CEasing_Utillity::QuadIn(vStartPos, vTargetPos, m_fAlphaTime, 0.6f));
		m_fX = vCurPos.x;
		m_fY = vCurPos.y;
		m_fAlphaRatio = vCurPos.z;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fAccTime, 0.25f));

	if (0.f >= m_fAlphaTime)
	{
		m_fX = vCurPoint.x;
		m_fY = vCurPoint.y;
	}
}

HRESULT CUI_Attacked_Num::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_DAMAGE_NUMBER), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Attacked_Num::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));
	_float3 vColor = { 1.f, 0.321f, 0.321f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vColor", &vColor, sizeof(_float3)));

	return S_OK;
}

CUI_Attacked_Num * CUI_Attacked_Num::Create()
{
	CUI_Attacked_Num*		pInstance = new CUI_Attacked_Num();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Attacked_Num");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Attacked_Num::Clone(void * pArg)
{
	CUI_Attacked_Num*		pInstance = new CUI_Attacked_Num(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Attacked_Num");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Attacked_Num::Free()
{
	__super::Free();
}
