#include "stdafx.h"
#include "..\Public\UI.h"

#include "GameInstance.h"

CUI::CUI()
	: CGameObject()
{
}

CUI::CUI(const CUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	if (FAILED(__super::Initialize(&TransformDesc)))
		__debugbreak();

	//memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));

	m_tUIDesc.fInitX = g_iWinCX >> 1;
	m_tUIDesc.fInitY = g_iWinCY >> 1;
	m_tUIDesc.fInitSizeX = 200.f;
	m_tUIDesc.fInitSizeY = 200.f;
	m_tUIDesc.fAngle = 0.f;
	m_tUIDesc.iTextureIdx = 0.f;
	m_strFontText = "";
	m_tUIDesc.iRenderLayer = 1000.f;
	m_tUIDesc.vFontPos = { 0.f,0.f };
	m_tUIDesc.vFontColor = { 1.f, 1.f, 1.f, 1.f };
	m_tUIDesc.fFontSize = 1.f;
	
	m_iMaxTextureIdx = m_pTextureCom->Get_NumTexture() - 1;

	return S_OK;
}

HRESULT CUI::Initialize_Pooling(void* pArg)
{
	RESET_ONCE(m_bSound0);
	RESET_ONCE(m_bSound1);
	RESET_ONCE(m_bSound2);
	RESET_ONCE(m_bSound3);
	RESET_ONCE(m_bSound4);
	RESET_ONCE(m_bSound5);
	return S_OK;
}

void CUI::Tick(_float fTimeDelta)
{
	MoveWithFont();

	if (0.01f > m_fSizeX)
	{
		m_fSizeX = 0.01f;
	}
	if (0.01f > m_fSizeY)
	{
		m_fSizeY = 0.01f;
	}

	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(m_tUIDesc.fAngle));
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));
}

HRESULT CUI::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	m_pShaderCom->Begin(m_iPassIdx);

	m_pVIBufferCom->Render();

	/* Font Render*/
	USEGAMEINSTANCE;
	
	if (m_tUIDesc.bRenderFontText)
	{
		wstring strTemp = towstring(m_strFontText);
		pGameInstance->Render_Font(TEXT("Font_Hancomdodum_Bold"), strTemp.c_str(), m_tUIDesc.vFontPos, XMLoadFloat4(&m_tUIDesc.vFontColor), m_tUIDesc.fFontSize);
	}
	else if (m_tUIDesc.bRenderFontNum)
	{
		_tchar strNum[MAX_PATH];
		wsprintf(strNum, TEXT("%d"), m_iFontNum);
		//string iNum = to_string(m_iFontNum);
		//swprintf_s(szTemp, TEXT("%02d:%02d"), iMin, iSec);
		pGameInstance->Render_Font(TEXT("Font_Hancomdodum_Bold"), strNum, m_tUIDesc.vFontPos, XMLoadFloat4(&m_tUIDesc.vFontColor), m_tUIDesc.fFontSize);
	}
	return S_OK;
}

void CUI::SetUp_UIpos()
{
	m_iRenderLayer = m_tUIDesc.iRenderLayer;
	m_vFontDelta.x = m_tUIDesc.vFontPos.x - m_tUIDesc.fInitX;
	m_vFontDelta.y = m_tUIDesc.vFontPos.y - m_tUIDesc.fInitY;

	m_fSizeX = m_tUIDesc.fInitSizeX;
	m_fSizeY = m_tUIDesc.fInitSizeY;
	m_fX = m_tUIDesc.fInitX;
	m_fY = m_tUIDesc.fInitY;

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

}

void CUI::MoveWithFont()
{
	if (m_bMoveWithFont)
	{
		m_tUIDesc.vFontPos.x = m_fX + m_vFontDelta.x;
		m_tUIDesc.vFontPos.y = m_fY + m_vFontDelta.y;
	}
	else
	{
		m_vFontDelta.x = m_tUIDesc.vFontPos.x - m_fX;
		m_vFontDelta.y = m_tUIDesc.vFontPos.y - m_fY;
	}
}

void CUI::Move_Frame(_float fFrameTime, _float fTimeDelta)
{
	m_fFrameTime += fTimeDelta;
	if (m_fFrameTime >= fFrameTime)
	{
		m_tUIDesc.iTextureIdx++;
		m_fFrameTime = 0.f;

		if (m_tUIDesc.iTextureIdx > m_iMaxTextureIdx)
		{
			m_tUIDesc.iTextureIdx = 0;
		}
	}
}

_float2 CUI::SetUp_ViewportPos(CGameObject* pGameObject)
{
	if (!pGameObject)
	{
		return { 0.f,0.f };
	}
	_vector vObjectPos = pGameObject->Get_Pos();

	_matrix	ViewMatrix = XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW));
	vObjectPos = XMVector3TransformCoord(vObjectPos, ViewMatrix);

	_matrix ProjMatrix = XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ));
	vObjectPos = XMVector3TransformCoord(vObjectPos, ProjMatrix);

	_float4 vProjPos;
	XMStoreFloat4(&vProjPos, vObjectPos);

	_float4 vViewportPos;
	{vViewportPos.x = vProjPos.x * (g_iWinCX * 0.5f);
	vViewportPos.y = vProjPos.y * (g_iWinCY * 0.5f);
	vViewportPos.x += g_iWinCX * 0.5f;
	vViewportPos.y = g_iWinCY * 0.5f - vViewportPos.y; }

	m_fX = vViewportPos.x + m_tUIDesc.vDeltaPos.x;
	m_fY = vViewportPos.y + m_tUIDesc.vDeltaPos.y;
	
	return { m_fX, m_fY };
}

_float2 CUI::SetUp_ViewportPos(_float4 vPos)
{
	_vector vObjectPos = XMLoadFloat4(&vPos);

	_matrix	ViewMatrix = XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW));
	vObjectPos = XMVector3TransformCoord(vObjectPos, ViewMatrix);

	_matrix ProjMatrix = XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ));
	vObjectPos = XMVector3TransformCoord(vObjectPos, ProjMatrix);

	_float4 vProjPos;
	XMStoreFloat4(&vProjPos, vObjectPos);

	_float4 vViewportPos;
	{vViewportPos.x = vProjPos.x * (g_iWinCX * 0.5f);
	vViewportPos.y = vProjPos.y * (g_iWinCY * 0.5f);
	vViewportPos.x += g_iWinCX * 0.5f;
	vViewportPos.y = g_iWinCY * 0.5f - vViewportPos.y; }

	m_fX = vViewportPos.x + m_tUIDesc.vDeltaPos.x;
	m_fY = vViewportPos.y + m_tUIDesc.vDeltaPos.y;

	return { m_fX, m_fY };
}

void CUI::Load_UIData(string strFileName)
{
	USEGAMEINSTANCE;
	json Data;

	wstring strFolder = TEXT("UI/");
	wstring strTemp = towstring(strFileName);
	wstring strFullName = strFolder + strTemp;

	const wchar_t* szName = strFullName.c_str();
	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(szName).c_str(), &Data);

	_uint iPrototypeID;

	for (auto& elem : Data)
	{
		CUI::UIDesc UIDesc;
		string strTemp = "";
		iPrototypeID = elem["iPrototypeID"];
		UIDesc.fInitX = elem["fInitX"];
		UIDesc.fInitY = elem["fInitY"];
		UIDesc.fInitSizeX = elem["fInitSizeX"];
		UIDesc.fInitSizeY = elem["fInitSizeY"];
		UIDesc.fAngle = elem["fAngle"];
		UIDesc.iTextureIdx = elem["iTextureIdx"];
		UIDesc.iRenderLayer = elem["iRenderLayer"];
		strTemp = elem["strText"];
		UIDesc.vFontPos.x = elem["vFontPosx"];
		UIDesc.vFontPos.y = elem["vFontPosy"];
		UIDesc.vFontColor.x = elem["vFontColorx"];
		UIDesc.vFontColor.y = elem["vFontColory"];
		UIDesc.vFontColor.z = elem["vFontColorz"];
		UIDesc.vFontColor.w = elem["vFontColorw"];
		UIDesc.fFontSize = elem["fFontSize"];
		UIDesc.bRenderFontText = elem["bRenderFontText"];
		UIDesc.bRenderFontNum = elem["bRenderFontNum"];

		ID_UINT IDPrototype;
		IDPrototype.iID = iPrototypeID;

		CGameObject* pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, IDPrototype, IDPrototype, &UIDesc);
		CUI* pUI = (CUI*)pGameObject;
		pUI->Set_FontText(strTemp);
	}
}

HRESULT CUI::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		__debugbreak();*/

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_tUIDesc.iTextureIdx));
	return S_OK;
}

void CUI::Free()
{
	__super::Free();
}
