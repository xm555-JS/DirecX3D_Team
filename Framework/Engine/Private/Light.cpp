#include "..\Public\Light.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"
#include "RenderTarget.h"
#include "Light_Manager.h"
#include "GameObject.h"

CLight::CLight()
{
}

HRESULT CLight::Initialize(const LIGHTDESC & LightDesc)
{
	m_LightDesc = LightDesc;

	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;

	//m_iDepthTargetCX = 1280 * 3;
	//m_iDepthTargetCY = 720 * 3;

//#ifdef _DEBUG
	//m_iDepthTargetCX = 1280 * 12;
	//m_iDepthTargetCY = 720 * 12;
	m_iDepthTargetCX = m_LightDesc.iDepthCX;
	m_iDepthTargetCY = m_LightDesc.iDepthCY;
//#endif // _DEBUG



	m_pLightDepth = pGameInstance->Create_RenderTarget(m_iDepthTargetCX, m_iDepthTargetCY, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 1.f));

	m_pLightManager = CLight_Manager::Get_Instance();

	_vector		vLightEye = XMLoadFloat4(&m_LightDesc.vPosition);
	//0.0001f == Áü¹ú¶ô ¹æÁö
	_vector		vLightAt = vLightEye - XMVectorSet(0.f, 1.f, 0.01f, 0.f);
	_vector		vLightUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(65.0f), 1280.f / 720.f, 0.2f, 1000.f));
	m_fFar = 1000.f;

	Create_DepthStencil();
	Create_Viewport();

	//memcpy(&m_WorldMatrix.m[3], &m_LightDesc.vPosition, sizeof(_float4));
	//m_WorldMatrix._44 = 1.f;

	//_vector		vLook = XMVectorSet(0.f, -1.f, 0.f, 0.f);
	//_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	//_vector		vUp = XMVector3Cross(vLook, vRight);

	//_float4		vRightX, vUpY, vLookZ;
	//XMStoreFloat4(&vRightX, vRight);
	//XMStoreFloat4(&vUpY, vUp);
	//XMStoreFloat4(&vLookZ, vLook);

	//memcpy(&m_WorldMatrix.m[0], &vRightX, sizeof(_float4));
	//memcpy(&m_WorldMatrix.m[1], &vUpY, sizeof(_float4));
	//memcpy(&m_WorldMatrix.m[2], &vLookZ, sizeof(_float4));


#ifdef _DEBUG
	m_pLightDepth->Ready_Debug(1150.0f, 450.f, 100.f, 100.f);
	//m_pLightDepth->Ready_Debug(1280.f * 0.5f, 720.f * 0.5f, 1280.f, 720.f);
#endif


	if (0.f == m_LightDesc.fRange)
		m_LightDesc.fRange = 30.f;

	return S_OK;
}

void CLight::Tick(_float fTimeDelta)
{
	_vector vCamPos = XMLoadFloat4(&CPipeLine::Get_Instance()->Get_CamPosition());
	if (Is_RangeIn(vCamPos))
	{
		//Setup_ViewPort();
		if (false == m_bRangeIn)
		{
			if (false == m_bUseForShadow)
				m_pLightManager->Add_RenderLightGroup(this);

			m_pLightManager->Add_ShadowLightGroup(this);

			m_bRangeIn = true;
		}
	}
	else
	{
		if (true == m_bRangeIn)
		{
			if (false == m_bUseForShadow)
				m_pLightManager->Remove_RenderLightGroup(this);

			m_pLightManager->Remove_ShadowLightGroup(this);

			m_bRangeIn = false;
		}
	}

}

void CLight::LateTick(_float fTimeDelta)
{



}

HRESULT CLight::Render(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	_uint			iPassIndex = 0;
	if (true == m_bUseForShadow)
		return S_OK;

	/*ºû Á¤º¸¸¦ ¼ÎÀÌ´õ¿¡ ´øÁø´Ù. */
	if (LIGHTDESC::TYPE_DIRECTIONAL == m_LightDesc.eType)
	{
		iPassIndex = 0;
		FAILEDCHECK(pShader->Set_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4)));
	}
	else if (LIGHTDESC::TYPE_POINT == m_LightDesc.eType)
	{
		iPassIndex = 1;
		FAILEDCHECK(pShader->Set_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4)));
		FAILEDCHECK(pShader->Set_RawValue("g_fRange", &m_LightDesc.fRange, sizeof(_float)));
	}
	else if (LIGHTDESC::TYPE_SPOT == m_LightDesc.eType)
	{
		iPassIndex = 2;
		FAILEDCHECK(pShader->Set_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4)));
		FAILEDCHECK(pShader->Set_RawValue("g_vLightLook", &m_LightDesc.vDirection, sizeof(_float4)));
		FAILEDCHECK(pShader->Set_RawValue("g_fInLightRange", &m_LightDesc.fRangeIn, sizeof(_float)));
		FAILEDCHECK(pShader->Set_RawValue("g_fOutLightRange", &m_LightDesc.fRangeOut, sizeof(_float)));
	}
	else
	{
		__debugbreak();
	}

	FAILEDCHECK(pShader->Set_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4)));
	FAILEDCHECK(pShader->Set_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4)));
	FAILEDCHECK(pShader->Set_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4)));


	pShader->Begin(iPassIndex);

	/* ¼ÎÀÌµå Å¸°Ù¿¡ ±×¸°´Ù.  */
	pVIBuffer->Render();

	return S_OK;
}


HRESULT CLight::Render_Shadows(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{

	// No Use TYPE_DIRECTIONAL
	//_vector vCamPos = XMLoadFloat4(&CPipeLine::Get_Instance()->Get_CamPosition());
	//_vector		vLightAt = vCamPos;

	_float			fPower = 3.f;
	FAILEDCHECK(pShader->Set_RawValue("g_fPower", &fPower, sizeof(_float)));
	FAILEDCHECK(pShader->Set_RawValue("g_fRange", &m_LightDesc.fRange, sizeof(_float)));
	FAILEDCHECK(pShader->Set_RawValue("g_fFar", &m_fFar, sizeof(_float)));

	_float4x4	ViewMatrix, ProjMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_ViewMatrix)));
	XMStoreFloat4x4(&ProjMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));

	FAILEDCHECK(pShader->Set_RawValue("g_LightViewMatrix", &ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(pShader->Set_RawValue("g_LightProjMatrix", &ProjMatrix, sizeof(_float4x4)));
	FAILEDCHECK(pShader->Set_ShaderResourceView("g_LightDepthTexture", m_pLightDepth->Get_SRV()));

	pShader->Begin(0);
	pVIBuffer->Render();

	//m_pLightDepth->Render_Debug(pShader, pVIBuffer);

	return S_OK;
}

HRESULT CLight::Render_Depth(CShader* pShader, _uint iPass)
{
	USEGAMEINSTANCE;

	Setup_ViewPort();
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	_uint				iNumViewport = 1;
	m_pContext->RSGetViewports(&iNumViewport, m_pOldViewport);
	m_pContext->RSSetViewports(1, &m_ViewPortDesc);

	list<CRenderTarget*> pMRTList;
	pMRTList.push_back(m_pLightDepth);
	FAILEDCHECK(pGameInstance->Begin_MRT(&pMRTList, m_pDepthStencilView));


	RENDERDESC	Desc;
	Desc.eType = RENDERTYPE_DEPTH;
	Desc.lParam = this;

	for (auto& pObject : m_RenderObject)
	{
		//FAILEDCHECK(pShader->Set_RawValue("g_WorldMatrix", pObject->Get_WorldMatrix(), sizeof(_float4x4)));

		//pShader->Begin(iPass);
		pObject->Render(&Desc);
	}

	FAILEDCHECK(pGameInstance->End_MRT());

	m_pContext->RSSetViewports(1, m_pOldViewport);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CLight::Render_Debug(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	m_pLightDepth->Render_Debug(pShader, pVIBuffer);

	return S_OK;
}
#endif // _DEBUG

_bool CLight::Is_RangeIn(_vector vPosition)
{
	_vector vLightPos = XMLoadFloat4(&m_LightDesc.vPosition);
	_float	fLength = XMVectorGetX(XMVector3Length(vPosition - vLightPos));

	if (fLength > m_LightDesc.fRange + 20.f)
	{
		return false;
	}
	return true;
}

HRESULT CLight::SetUp_DepthComponents(CShader* pShader)
{
	//_float		fFar = MGR(CPipeLine)->Get_Viewport().MaxZ;
	FAILEDCHECK(pShader->Set_RawValue("g_fFar", &m_fFar, sizeof(_float)));


	_float4x4	ViewMatrix, ProjMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_ViewMatrix)));
	XMStoreFloat4x4(&ProjMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));
	FAILEDCHECK(pShader->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(pShader->Set_RawValue("g_ProjMatrix", &ProjMatrix, sizeof(_float4x4)));

	return S_OK;
}

void CLight::Bind_PipeLine()
{
	CPipeLine*			pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix		ViewMatrix = XMLoadFloat4x4(&m_ViewMatrix);
	pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, ViewMatrix);

	_matrix		ProjMatrix = XMLoadFloat4x4(&m_ProjMatrix);
	pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, ProjMatrix);

	RELEASE_INSTANCE(CPipeLine);
}

void CLight::Create_Viewport()
{
	//D3D11_VIEWPORT		m_pOldViewport[1];
	//_uint				iNumViewport = 1;
	//m_pContext->RSGetViewports(&iNumViewport, m_pOldViewport);
	//D3D11_VIEWPORT			ViewPortDesc;

	ZeroMemory(&m_ViewPortDesc, sizeof(D3D11_VIEWPORT));
	m_ViewPortDesc.TopLeftX = 0;
	m_ViewPortDesc.TopLeftY = 0;
	m_ViewPortDesc.Width = m_iDepthTargetCX;
	m_ViewPortDesc.Height = m_iDepthTargetCY;
	m_ViewPortDesc.MinDepth = 0.f;
	m_ViewPortDesc.MaxDepth = 1.f;


	//m_pContext->RSSetViewports(1, &ViewPortDesc);
}

void CLight::Create_DepthStencil()
{
	ID3D11Texture2D* pDepthStencilTexture = nullptr;
	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = m_iDepthTargetCX;
	TextureDesc.Height = m_iDepthTargetCY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	FAILEDCHECK(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture));
	FAILEDCHECK(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDepthStencilView));

	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	Safe_Release(pDepthStencilTexture);
}

void CLight::Setup_ViewPort()
{
	_vector		vLightEye = XMLoadFloat4(&m_LightDesc.vPosition);
	//0.0001f == Áü¹ú¶ô ¹æÁö

	_vector		vLightAt;

	// ¿©±â ¸¶À½¿¡ ¾Èµë ³ªÁß¿¡ ¿©À¯ µÇ¸é ¼öÁ¤ÇÏÀÚ
	if (m_pLookAtObject)
	{
		if (true == m_pLookAtObject->Is_Dead())
		{
			m_pLookAtObject = nullptr;
			vLightAt = XMVectorSet(256.f, 0.f, 256.f, 1.f);
		}
		else
		{
			vLightAt = m_pLookAtObject->Get_Pos();
			vLightEye = XMLoadFloat4(&m_LightDesc.vPosition);
		}
	}
	else
		vLightAt = XMVectorSet(256.f, 0.f, 256.f, 1.f);


	_vector		vLightUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp));
}

CLight * CLight::Create(const LIGHTDESC & LightDesc)
{
	CLight*		pInstance = new CLight();

	if (FAILED(pInstance->Initialize(LightDesc)))
	{
		MSG_BOX("Failed to Created : CLight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pLightDepth);
	Safe_Release(m_pDepthStencilView);
	__super::Free();
}
