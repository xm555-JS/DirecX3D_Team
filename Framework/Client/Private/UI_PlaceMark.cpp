#include "stdafx.h"
#include "..\Public\UI_PlaceMark.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "UI_Dist_Num.h"
#include "Player.h"

CUI_PlaceMark::CUI_PlaceMark()
	: CUI()
{
}

CUI_PlaceMark::CUI_PlaceMark(const CUI_PlaceMark & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PlaceMark::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_PlaceMark::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;

	if (2 == m_tUIDesc.iTextureIdx)
	{
		m_fAccTime = 1.f;
	}

	SetUp_UIpos();

	if (0 == m_tUIDesc.iTextureIdx)
	{
		Create_Dist_Num();
	}

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

HRESULT CUI_PlaceMark::Initialize_Pooling(void* pArg)
{
	m_bExpansion = false;
	m_fAccTime = 0.f;
	m_fWaitTime = 0.f;
	m_bAble = false;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;

	if (2 == m_tUIDesc.iTextureIdx)
	{
		m_fAccTime = 1.f;
	}

	SetUp_UIpos();

	if (0 == m_tUIDesc.iTextureIdx)
	{
		Create_Dist_Num();
	}

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_PlaceMark::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	


	if (3 > m_tUIDesc.iTextureIdx)
	{
		SetUp_Pos();
	}
	else if (3 == m_tUIDesc.iTextureIdx)
	{
		SetUp_MarkArrow();
	}	
}

void CUI_PlaceMark::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
	if (1 == m_tUIDesc.iTextureIdx)
	{
		SetUp_Mark_Effect(fTimeDelta);
	}
	else if (2 == m_tUIDesc.iTextureIdx)
	{
		Emphasis_Mark(fTimeDelta);
	}
	else if (0 == m_tUIDesc.iTextureIdx)
	{
		SetUp_DistNum_Pos();
	}
}

HRESULT CUI_PlaceMark::Render()
{
	if (m_bRender && m_bAble)
	{
		FAILEDCHECK(CUI::Render());

		_bool bGlow = false;
		_float fGlowPower = 0.f;
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));
	}

	return S_OK;
}

void CUI_PlaceMark::On_Dead()
{
	USEGAMEINSTANCE;
	for (_uint i = 0; i < DISTNUM_END; ++i)
	{
		if (m_pDistNum[i])
		{
			pGameInstance->Add_PoolingObject(m_pDistNum[i]);
		}
	}
}

void CUI_PlaceMark::SetUp_Pos()
{
	if (!m_tUIDesc.pGameObject)
		return;


	_float4 vTempTargetPos;
	_vector vTargetPos = m_tUIDesc.pGameObject->Get_Pos();

	//_float4 vTemp;
	//memcpy(&vTemp, m_tUIDesc.pGameObject->Get_WorldMatrix()->m[1], sizeof(_float4));
	_vector vTargetUp = {0.f,1.f,0.f,0.f};//XMLoadFloat4(&vTemp);

	vTargetUp = XMVector3Normalize(vTargetUp);
  
	vTargetPos += vTargetUp * m_fOffset_TargetPosY;
	XMStoreFloat4(&vTempTargetPos, vTargetPos);
	_float2 vViewPos = SetUp_ViewportPos(vTempTargetPos);



	_matrix ViewMatrixInv;
	ViewMatrixInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW)));

	_vector vCamLook = ViewMatrixInv.r[2];
	_vector vCamPos = ViewMatrixInv.r[3];

	vCamLook = XMVector3Normalize(vCamLook);

	_vector vTargetDir = vTargetPos - vCamPos;

	// 내 앞에 있으면 return
	if (0.f < vViewPos.x && g_iWinCX > vViewPos.x && 0.f < vViewPos.y && g_iWinCY > vViewPos.y
		&& 0.f < XMVectorGetX(XMVector3Dot(vTargetDir, vCamLook)))
	{
		m_fSizeX = m_tUIDesc.fInitSizeX;
		m_fSizeY = m_tUIDesc.fInitSizeY;
		m_bExpansion = false;
		return;
	}

	_vector vRayPos = vCamLook * XMVector3Dot(vTargetDir, vCamLook) + vCamPos;
	_vector vDir = vTargetPos - vRayPos;

	vDir = XMVector3Normalize(vDir);
	vDir *= 2.f;

	_float4 vPos;
	XMStoreFloat4(&vPos, vCamPos + vCamLook * 5.f + vDir);

	_vector vObjectPos = XMLoadFloat4(&vPos);

	_matrix	ViewMatrix = XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW));
	vObjectPos = XMVector3TransformCoord(vObjectPos, ViewMatrix);

	_matrix ProjMatrix = XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ));
	vObjectPos = XMVector3TransformCoord(vObjectPos, ProjMatrix);

	_float4 vProjPos;
	XMStoreFloat4(&vProjPos, vObjectPos);

	_float4 vViewportPos;
	{vViewportPos.x = vProjPos.x * 1.9f * (g_iWinCX * 0.5f);
	vViewportPos.y = vProjPos.y * (g_iWinCY * 0.5f);
	vViewportPos.x += g_iWinCX * 0.5f;
	vViewportPos.y = g_iWinCY * 0.5f - vViewportPos.y; }

	m_fX = vViewportPos.x + m_tUIDesc.vDeltaPos.x;
	m_fY = vViewportPos.y + m_tUIDesc.vDeltaPos.y;

	m_fSizeX = m_tUIDesc.fInitSizeX + 7.f;
	m_fSizeY = m_tUIDesc.fInitSizeY + 7.f;
	m_bExpansion = true; 
}

void CUI_PlaceMark::Emphasis_Mark(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	_vector vStartPoint;
	_vector vTargetPoint;
	if (m_bExpansion)
	{
		vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX + 220.f, 1.f, 1.f, 1.f);
		vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX, 1.f, 1.f, 1.f);
	}
	else
	{
		vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX + 213.f, 1.f, 1.f, 1.f);
		vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX - 7.f, 1.f, 1.f, 1.f);
	}

	if (0.45f < m_fAccTime)
	{
		m_fAccTime = 0.45f;
		m_bAble = false;
	}

	_float4 vCurPoint;

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicIn(vStartPoint, vTargetPoint, m_fAccTime, 0.45f));
	m_fSizeX = m_fSizeY = vCurPoint.x;

	USEGAMEINSTANCE;
	if (KEY_DOWN('T'))
	{
		m_fAccTime = 0.f;
		m_bAble = true;
	}
}

void CUI_PlaceMark::SetUp_Mark_Effect(_float fTimeDelta)
{
	m_fWaitTime -= fTimeDelta;
	m_bRender = false;
	if (0.f > m_fWaitTime)
	{
		m_bRender = true;
		m_fAccTime += fTimeDelta;
	}

	_vector vStartPoint;
	_vector vTargetPoint;

	if (m_bExpansion)
	{
		vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX - 25.f, 1.f, 1.f, 1.f);
		vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX, 1.f, 1.f, 1.f);
	}
	else
	{
		vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX - 35.f, 1.f, 1.f, 1.f);
		vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX - 10.f, 1.f, 1.f, 1.f);
	}

	if (0.65f < m_fAccTime)
	{
		m_fAccTime = 0.f;
		m_fWaitTime = 0.55f;
	}

	_float4 vCurPoint;

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CircularOut(vStartPoint, vTargetPoint, m_fAccTime, 0.65f));
	m_fSizeX = m_fSizeY = vCurPoint.x;
}

void CUI_PlaceMark::SetUp_MarkArrow()
{
	if (!m_tUIDesc.pGameObject)
		return;

	m_tUIDesc.vDeltaPos.y = 25.f;
	m_tUIDesc.fAngle = 180.f;

	_float4 vTempTargetPos;
	_vector vTargetPos = m_tUIDesc.pGameObject->Get_Pos();

	_float4 vTemp;
	memcpy(&vTemp, m_tUIDesc.pGameObject->Get_WorldMatrix()->m[1], sizeof(_float4));
	_vector vTargetUp = XMLoadFloat4(&vTemp);

	vTargetUp = XMVector3Normalize(vTargetUp);

	vTargetPos += vTargetUp * m_fOffset_TargetPosY;
	XMStoreFloat4(&vTempTargetPos, vTargetPos);
	_float2 vViewPos = SetUp_ViewportPos(vTempTargetPos);

	_matrix ViewMatrixInv;
	ViewMatrixInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW)));

	_vector vCamUp = ViewMatrixInv.r[1];
	_vector vCamLook = ViewMatrixInv.r[2];
	_vector vCamPos = ViewMatrixInv.r[3];

	vCamLook = XMVector3Normalize(vCamLook);

	_vector vTargetDir = vTargetPos - vCamPos;

	// 내 앞에 있으면 return
	if (0.f < vViewPos.x && g_iWinCX > vViewPos.x && 0.f < vViewPos.y && g_iWinCY > vViewPos.y
		&& 0.f < XMVectorGetX(XMVector3Dot(vTargetDir, vCamLook)))
	{
		m_fSizeX = m_tUIDesc.fInitSizeX;
		m_fSizeY = m_tUIDesc.fInitSizeY;
		return;
	}
	m_tUIDesc.vDeltaPos.y = 0.f;

	_vector vRayPos = vCamLook * XMVector3Dot(vTargetDir, vCamLook) + vCamPos;
	_vector vDir = vTargetPos - vRayPos;


	vDir = XMVector3Normalize(vDir);
	vDir *= 2.25f;

	_float4 vPos;
	XMStoreFloat4(&vPos, vCamPos + vCamLook * 5.f + vDir);

	_vector vObjectPos = XMLoadFloat4(&vPos);


	_matrix	ViewMatrix = XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW));
	vObjectPos = XMVector3TransformCoord(vObjectPos, ViewMatrix);

	_matrix ProjMatrix = XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ));
	vObjectPos = XMVector3TransformCoord(vObjectPos, ProjMatrix);

	_float4 vProjPos;
	XMStoreFloat4(&vProjPos, vObjectPos);

	_float4 vViewportPos;
	{vViewportPos.x = vProjPos.x * 1.8f * (g_iWinCX * 0.5f);
	vViewportPos.y = vProjPos.y * (g_iWinCY * 0.5f);
	vViewportPos.x += g_iWinCX * 0.5f;
	vViewportPos.y = g_iWinCY * 0.5f - vViewportPos.y; }

	m_fX = vViewportPos.x;
	m_fY = vViewportPos.y;

	m_tUIDesc.fAngle = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(vCamUp, vDir)));
	if (640.f < m_fX)
	{
		m_tUIDesc.fAngle *= -1.f;
	}

	m_fSizeX = m_tUIDesc.fInitSizeX + 7.f;
	m_fSizeY = m_tUIDesc.fInitSizeY + 7.f;
}

void CUI_PlaceMark::Create_Dist_Num()
{
	USEGAMEINSTANCE;
	for (_uint i = 0; i < DISTNUM_END; ++i)
	{
		m_pDistNum[i] = (CUI_Dist_Num*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_DIST_NUM), ID_UINT(UI, UI_DIST_NUM));
	}
	m_pDistNum[METERS]->Set_iTextureIdx(10);
}

void CUI_PlaceMark::SetUp_DistNum_Pos()
{
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	if (!pPlayer || !m_tUIDesc.pGameObject)
	{
		m_pDistNum[THOUSANDS]->Set_Render(false);
		m_pDistNum[HUNDREDS]->Set_Render(false);
		m_pDistNum[TENS]->Set_Render(false);
		m_pDistNum[UNITS]->Set_Render(false);
		m_pDistNum[METERS]->Set_Render(false);
		return;
	}
	else
	{
		m_pDistNum[THOUSANDS]->Set_Render(true);
		m_pDistNum[HUNDREDS]->Set_Render(true);
		m_pDistNum[TENS]->Set_Render(true);
		m_pDistNum[UNITS]->Set_Render(true);
		m_pDistNum[METERS]->Set_Render(true);
	}

	_uint iInitDist;
	_uint iDist;
	iInitDist = iDist = XMVectorGetX(XMVector3Length(pPlayer->Get_Pos() - m_tUIDesc.pGameObject->Get_Pos()));

	//_uint iTemp;

	m_pDistNum[UNITS]->Set_iTextureIdx(iDist % 10);
	iDist /= 10;
	m_pDistNum[TENS]->Set_iTextureIdx(iDist % 10);
	iDist /= 10;
	m_pDistNum[HUNDREDS]->Set_iTextureIdx(iDist % 10);
	iDist /= 10;
	m_pDistNum[THOUSANDS]->Set_iTextureIdx(iDist % 10);

	if (0 == m_pDistNum[THOUSANDS]->Get_UIDesc().iTextureIdx)
	{
		m_pDistNum[THOUSANDS]->Set_Render(false);
		if (0 == m_pDistNum[HUNDREDS]->Get_UIDesc().iTextureIdx)
		{
			m_pDistNum[HUNDREDS]->Set_Render(false);
			if (0 == m_pDistNum[TENS]->Get_UIDesc().iTextureIdx)
			{
				m_pDistNum[TENS]->Set_Render(false);
			}
			else
			{
				m_pDistNum[TENS]->Set_Render(true);
			}

		}
		else
		{
			m_pDistNum[HUNDREDS]->Set_Render(true);
		}
	}
	else
	{
		m_pDistNum[THOUSANDS]->Set_Render(true);
	}

	// 절두체 밖
	if (m_bExpansion)
	{
		// Size
		for (_uint i = 0; i < DISTNUM_END; ++i)
		{
			m_pDistNum[i]->Set_vUISize({ 30.f,33.f });
		}

		//위 (숫자 아래)
		if (360.f > m_fY && 400.f < m_fX && 800.f > m_fX)
		{
			m_pDistNum[THOUSANDS]->Set_vUIPos({ m_fX - 42.f,m_fY + 35.f });
			m_pDistNum[HUNDREDS]->Set_vUIPos({ m_fX - 28.f,m_fY + 35.f });
			m_pDistNum[TENS]->Set_vUIPos({ m_fX - 14.f,m_fY + 35.f });
			m_pDistNum[UNITS]->Set_vUIPos({ m_fX,m_fY + 35.f });
			m_pDistNum[METERS]->Set_vUIPos({ m_fX + 18.f,m_fY + 35.f });
		}
		//아래 (숫자 위)
		else if (360.f < m_fY && 400.f < m_fX && 800.f > m_fX)
		{
			m_pDistNum[THOUSANDS]->Set_vUIPos({ m_fX - 42.f,m_fY - 35.f });
			m_pDistNum[HUNDREDS]->Set_vUIPos({ m_fX - 28.f,m_fY - 35.f });
			m_pDistNum[TENS]->Set_vUIPos({ m_fX - 14.f,m_fY - 35.f });
			m_pDistNum[UNITS]->Set_vUIPos({ m_fX,m_fY - 35.f });
			m_pDistNum[METERS]->Set_vUIPos({ m_fX + 18.f,m_fY - 35.f });
		}
		//오른쪽 (숫자 왼쪽)
		else if (800.f < m_fX)
		{
			m_pDistNum[THOUSANDS]->Set_vUIPos({ m_fX - 80.f,m_fY });
			m_pDistNum[HUNDREDS]->Set_vUIPos({ m_fX - 66.f,m_fY });
			m_pDistNum[TENS]->Set_vUIPos({ m_fX - 52.f,m_fY });
			m_pDistNum[UNITS]->Set_vUIPos({ m_fX - 38.f,m_fY });
			m_pDistNum[METERS]->Set_vUIPos({ m_fX - 20.f,m_fY });
		}
		//왼쪽 (숫자 오른쪽)
		else if (400.f > m_fX)
		{
			m_pDistNum[THOUSANDS]->Set_vUIPos({ m_fX + 14.f,m_fY });
			m_pDistNum[HUNDREDS]->Set_vUIPos({ m_fX + 28.f,m_fY });
			m_pDistNum[TENS]->Set_vUIPos({ m_fX + 42.f,m_fY });
			m_pDistNum[UNITS]->Set_vUIPos({ m_fX + 56.f,m_fY });
			m_pDistNum[METERS]->Set_vUIPos({ m_fX + 74.f,m_fY });
			if (0 == m_pDistNum[THOUSANDS]->Get_UIDesc().iTextureIdx)
			{
				m_pDistNum[HUNDREDS]->Set_vUIPos({ m_fX + 14.f,m_fY });
				m_pDistNum[TENS]->Set_vUIPos({ m_fX + 28.f,m_fY });
				m_pDistNum[UNITS]->Set_vUIPos({ m_fX + 42.f,m_fY });
				m_pDistNum[METERS]->Set_vUIPos({ m_fX + 60.f,m_fY });
				if (0 == m_pDistNum[HUNDREDS]->Get_UIDesc().iTextureIdx)
				{
					m_pDistNum[TENS]->Set_vUIPos({ m_fX + 14.f,m_fY });
					m_pDistNum[UNITS]->Set_vUIPos({ m_fX + 28.f,m_fY });
					m_pDistNum[METERS]->Set_vUIPos({ m_fX + 46.f,m_fY });
					if (0 == m_pDistNum[TENS]->Get_UIDesc().iTextureIdx)
					{
						m_pDistNum[UNITS]->Set_vUIPos({ m_fX + 14.f,m_fY });
						m_pDistNum[METERS]->Set_vUIPos({ m_fX + 32.f,m_fY });
					}
				}
			}
		}
	}
	else // 절두체 안
	{
		// Size
		for (_uint i = 0; i < DISTNUM_END; ++i)
		{
			m_pDistNum[i]->Set_vUISize({ 25.f,28.f });
		}
		m_pDistNum[THOUSANDS]->Set_vUIPos({ m_fX - 36.f,m_fY - 30.f });
		m_pDistNum[HUNDREDS]->Set_vUIPos({ m_fX - 24.f ,m_fY - 30.f });
		m_pDistNum[TENS]->Set_vUIPos({ m_fX - 12.f,m_fY - 30.f });
		m_pDistNum[UNITS]->Set_vUIPos({ m_fX, m_fY - 30.f });
		m_pDistNum[METERS]->Set_vUIPos({ m_fX + 15.f,m_fY - 30.f });

		if (10 > iInitDist)
		{
			m_pDistNum[UNITS]->Set_vUIPos({ m_fX - 8.f,m_fY - 30.f });
			m_pDistNum[METERS]->Set_vUIPos({ m_fX + 4.f, m_fY - 30.f });
		}
	}

	if (!m_bAble)
	{
		m_pDistNum[THOUSANDS]->Set_Render(false);
		m_pDistNum[HUNDREDS]->Set_Render(false);
		m_pDistNum[TENS]->Set_Render(false);
		m_pDistNum[UNITS]->Set_Render(false);
		m_pDistNum[METERS]->Set_Render(false);
	}
}

HRESULT CUI_PlaceMark::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_PLACEMARK), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_PlaceMark::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	_bool	bGlow = false;
	_float	fGlowPower = 0.f;
	if (1 == m_tUIDesc.iTextureIdx ||
		2 == m_tUIDesc.iTextureIdx)
	{
		bGlow = true;

		if (2 == m_tUIDesc.iTextureIdx)
		{
			fGlowPower = 0.7f;
		}
	}
	
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));

	return S_OK;
}

CUI_PlaceMark * CUI_PlaceMark::Create()
{
	CUI_PlaceMark*		pInstance = new CUI_PlaceMark();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_PlaceMark");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_PlaceMark::Clone(void * pArg)
{
	CUI_PlaceMark*		pInstance = new CUI_PlaceMark(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_PlaceMark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlaceMark::Free()
{
	__super::Free();
}
