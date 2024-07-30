#include "..\Public\Render_Manager.h"


#include "Target_Manager.h"
#include "Light_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Camera.h"

#include "RenderTarget.h"
#include "Light.h"


IMPLEMENT_SINGLETON(CRender_Manager);

CRender_Manager::CRender_Manager()
{
	m_pTarget_Manager = CTarget_Manager::Get_Instance();
	m_pLight_Manager = CLight_Manager::Get_Instance();
}

_vector CRender_Manager::Get_SunDepth(_uint iX, _uint iY)
{
	Color MyColor(0, 0, 0);
	m_pSunDepth->GetPixel(iX, iY, &MyColor);
	BYTE TempR = MyColor.GetR();
	BYTE TempG = MyColor.GetG();
	BYTE TempB = MyColor.GetB();
	BYTE TempA = MyColor.GetA();

	_vector vColor = { (_float)TempR , (_float)TempG , (_float)TempB , (_float)TempA};

	return vColor;
}

_bool CRender_Manager::isUpperThanHight(_vector vPos, _float fOffset)
{
	_float fHight = 0.f;

	_vector	vTargetPos = vPos;

	_float2		vTargetUV;
	vTargetPos = XMVector3TransformCoord(vTargetPos, XMLoadFloat4x4(&m_HightViewMat));

	_float fViewZ = XMVectorGetZ(vTargetPos);
	vTargetPos = XMVector3TransformCoord(vTargetPos, XMLoadFloat4x4(&m_HightProjMat));

	_float		fTargetDepth = XMVectorGetZ(vTargetPos);
	vTargetUV.x = (XMVectorGetX(vTargetPos) + 1.f) * 0.5f;
	vTargetUV.y = (XMVectorGetY(vTargetPos) - 1.f) * -0.5f;

	//_float		fFar = MGR(CPipeLine)->Get_Viewport().MaxZ;
	//fTargetDepth -= fBias / g_fFar;


	float		fBias = 0.003f;
	if (1.f - fBias < vTargetUV.x ||
		1.f - fBias < vTargetUV.y ||
		0.f + fBias > vTargetUV.x ||
		0.f + fBias > vTargetUV.y)
		fTargetDepth = 0.f;

	_uint	iX = _uint(vTargetUV.x * 1280);
	_uint	iY = _uint(vTargetUV.y * 720);

	Color MyColor(0, 0, 0);
	m_pHight->GetPixel(iX, iY, &MyColor);
	_float TempR = (_float)MyColor.GetR() / 255.f;
	_float TempG = (_float)MyColor.GetG() / 255.f;
	_float TempB = (_float)MyColor.GetB() / 255.f;
	_float TempA = (_float)MyColor.GetA() / 255.f;
	
	_vector vColor = { (_float)TempR , (_float)TempG , (_float)TempB , (_float)TempA };

	//(x - 0.9f) * 10.f
	vColor = XMVectorSetX(vColor, (XMVectorGetX(vColor) * 0.1f) + 0.9f);
	//vColor = XMVectorSetY(vColor, (XMVectorGetY(vColor) * 0.1f) + 0.9f);
	//_float fViewDepthZ = XMVectorGetY(vColor) * 1000.f;

	_float fViewDepthZ = ((XMVectorGetY(vColor) * 0.1f) + 0.9f) * 1000.f;

	//_float	fFar = MGR(CPipeLine)->Get_Viewport().MaxZ;
	//_float	fViewZ = XMVectorGetY(vColor) * fFar;

	//if (1.f == XMVectorGetY(vColor) || fTargetDepth > XMVectorGetX(vColor))
	_float fTestDepthZ = ((1000.f - fViewDepthZ) * 2.75f) - 174.f;
	_float fTestViewZ = 1000.f - fViewZ;

	//if (1.f == XMVectorGetY(vColor) || fViewZ * fOffset > fViewDepthZ)
	if (1.f == XMVectorGetY(vColor) || fTestViewZ * fOffset < fTestDepthZ)
		return false;
	else
		return true;
}

void CRender_Manager::Set_CurRenderer(CRenderer* pRenderer)
{
	m_pCurRenderer = pRenderer;
	m_RenderObjects = pRenderer->Get_RenderObjects();

	assert(m_RenderObjects);

}

HRESULT CRender_Manager::Add_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject)
{
	if (eGroup >= RENDER_END ||
		nullptr == pGameObject)
		__debugbreak();

	if (m_pCurRenderer)
		m_pCurRenderer->Add_RenderGroup(eGroup, pGameObject);

	return S_OK;
}

HRESULT CRender_Manager::Initialize()
{
	if (nullptr == m_pTarget_Manager)
		__debugbreak();

	// Setup Device
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;

	// Viewport
	ZeroMemory(&m_ViewPortDesc, sizeof(D3D11_VIEWPORT));
	_uint		iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &m_ViewPortDesc);


	// Setup Matrix
	_matrix			WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(m_ViewPortDesc.Width, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, m_ViewPortDesc.Height, 0.f, 0.f);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(WorldMatrix));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(m_ViewPortDesc.Width, m_ViewPortDesc.Height, 0.f, 1.f)));

	// Create Shader
	NULLCHECK(m_pShader_Deferred = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	NULLCHECK(m_pShader_Blur = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_Blur.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	NULLCHECK(m_pShader_Shadow = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_Shadow.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	NULLCHECK(m_pShader_SSAO = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_SSAO.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	NULLCHECK(m_pShader_SSR = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_SSR.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	NULLCHECK(m_pShader_Light = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_Light.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	NULLCHECK(m_pShader_Outline = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_Outline.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	NULLCHECK(m_pShader_Bloom = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_Bloom.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	NULLCHECK(m_pShader_Copy = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_Copy.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	NULLCHECK(m_pShader_Glow = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_Glow.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	NULLCHECK(m_pShader_Water = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_Water.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	NULLCHECK(m_pShader_DOF = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_DOF.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	NULLCHECK(m_pShader_DownSampling = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_DownSampling.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));

	NULLCHECK(m_pVIBuffer = CVIBuffer_Rect::Create());


	//(x - 0.8f) * 5.f
	m_pSunDepth = Bitmap::FromFile(TEXT("../Bin/Resources/Textures/RenderTarget/SunDepth.bmp"));
	// View matrix
	// {-0.221814990, -0.680338919, -0.698524892, 0.00000000}
	// {0.00000000, 0.716370523, -0.697719991, 0.00000000}
	// {0.975088716, -0.154764757, -0.158901736, 0.00000000}
	// {-168.585876, 237.843552, 1070.74402, 1.00000000}
	//
	// Proj Matrix
	// {2.74747753, 0.00000000, 0.00000000, 0.00000000}
	// {0.00000000, 2.74747753, 0.00000000, 0.00000000}
	// {0.00000000, 0.00000000, 1.00013328, 1.00000000}
	// {0.00000000, 0.00000000, -0.200026661, 0.00000000}


	//(x - 0.9f) * 10.f
	m_pHight = Bitmap::FromFile(TEXT("../Bin/Resources/Textures/RenderTarget/Hight.bmp"));
	m_HightViewMat = _float4x4(
		-1.00000000, 0.00000000, 0.00000000, 0.00000000,
		0.00000000, 1.00708007e-06, -1.00000000, 0.00000000,
		-0.00000000, -1.00000000, -1.00708007e-06, 0.00000000,
		256.000000, 256.000000, 1000.00024, 1.00000000);
	m_HightProjMat = _float4x4(
		2.74747753, 0.00000000, 0.00000000, 0.00000000,
		0.00000000, 2.74747753, 0.00000000, 0.00000000,
		0.00000000, 0.00000000, 1.00013328, 1.00000000,
		0.00000000, 0.00000000, -0.200026661, 0.00000000);
	// View matrix
	// {-1.00000000, 0.00000000, 0.00000000, 0.00000000}
	// {0.00000000, 1.00708007e-06, -1.00000000, 0.00000000}
	// {-0.00000000, -1.00000000, -1.00708007e-06, 0.00000000}
	// {256.000000, 256.000000, 1000.00024, 1.00000000}
	//
	// Proj Matrix
	// {2.74747753, 0.00000000, 0.00000000, 0.00000000}
	// {0.00000000, 2.74747753, 0.00000000, 0.00000000}
	// {0.00000000, 0.00000000, 1.00013328, 1.00000000}
	// {0.00000000, 0.00000000, -0.200026661, 0.00000000}



#define	NOISEPATH "../Bin/Resources/Textures/Effect/Noise/"
	m_pNoiseTex = CTexture::Create(string(NOISEPATH));

	return S_OK;
}

HRESULT CRender_Manager::Draw_RenderGroup()
{
	if (!m_pCurRenderer ||
		!m_RenderObjects)
		return S_OK;

	if (m_pCurRenderer->Get_Owner() &&
		m_pCurRenderer->Get_Owner()->Is_Dead())
	{
		m_pCurRenderer = nullptr;
		m_RenderObjects = nullptr;
		return S_OK;
	}

	USEGAMEINSTANCE;
	FAILEDCHECK(Clear_RenderTargets());

	//FAILEDCHECK(Render_Priority());
	FAILEDCHECK(Render_NonAlphaBlend());
	FAILEDCHECK(Render_Terrain());
	FAILEDCHECK(Render_Screen_Outline_Unit());
	FAILEDCHECK(Render_Cloud_Sky());
	FAILEDCHECK(Render_Cloud());
	FAILEDCHECK(Render_AlphaBlend());
	FAILEDCHECK(Render_Water());
	FAILEDCHECK(Render_Water_Sueface());
	FAILEDCHECK(Render_NonLight());
	FAILEDCHECK(Render_UI());
	FAILEDCHECK(Render_Effect());

	FAILEDCHECK(Render_Lights());


	FAILEDCHECK(Render_Glow());
	FAILEDCHECK(Render_SSR());

	FAILEDCHECK(Render_Screen_Shadow());


	FAILEDCHECK(Render_Screen_Blend());
	//FAILEDCHECK(Render_FinalTexture());


#ifdef _DEBUG

	if (KEY_TOGGLE(VK_F9))
		FAILEDCHECK(Render_Debug()); 
#endif // _DEBUG

	return S_OK;
}

void CRender_Manager::Creat_DDSFromRT(const _tchar* pMRTTag, const _tchar* pPath)
{
	m_pCurRenderer->Creat_DDSFromRT(pMRTTag, pPath);
}

//HRESULT CRender_Manager::Render_Priority()
//{
//	//FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Priority"))));
//
//	for (auto& pGameObject : m_RenderObjects[RENDER_PRIORITY])
//	{
//		if (nullptr != pGameObject)
//			pGameObject->Render();
//
//		//Safe_Release(pGameObject);
//	}
//	//m_RenderObjects[RENDER_PRIORITY].clear();
//
//	//FAILEDCHECK(m_pTarget_Manager->End_MRT());
//
//
//	return S_OK;
//}

HRESULT CRender_Manager::Clear_RenderTargets()
{
	m_pCurRenderer->Find_RenderTarget(TEXT("Target_Glow"))->Clear();
	m_pCurRenderer->Find_RenderTarget(TEXT("Target_RimLight"))->Clear();
	m_pCurRenderer->Find_RenderTarget(TEXT("Target_Effect"))->Clear();
	m_pCurRenderer->Find_RenderTarget(TEXT("Target_Distortion"))->Clear();
	m_pCurRenderer->Find_RenderTarget(TEXT("Target_UI"))->Clear();
	m_pCurRenderer->Find_RenderTarget(TEXT("Target_Normal"))->Clear();
	m_pCurRenderer->Find_RenderTarget(TEXT("Target_SSR_Flag"))->Clear();
	m_pCurRenderer->Find_RenderTarget(TEXT("Target_TerrainFlag"))->Clear();

	m_pContext->ClearDepthStencilView(m_pCurRenderer->Find_DepthStencilView(TEXT("Target_UI")), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	return S_OK;
}

HRESULT CRender_Manager::Render_Terrain()
{
	//CRenderTarget* pTerrainFlag = m_pCurRenderer->Find_RenderTarget(TEXT("Target_Terrain"));
	//pTerrainFlag->Clear();
	FAILEDCHECK(m_pTarget_Manager->BeginOverwrite_MRT(m_pCurRenderer->Find_MRT(TEXT("MRT_Deferred_Terrain"))));

	for (auto& pGameObject : m_RenderObjects[RENDER_TERRAIN])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_TERRAIN].clear();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

	//Render_Blur(TEXT("Target_Normal"), TEXT("Target_Normal"), 2);

	return S_OK;
}

HRESULT CRender_Manager::Render_NonAlphaBlend()
{
	/* 렌더타겟을 장치에 Diffuse + Normal 바인딩한ㄷ앋. */
	//FAILEDCHECK(m_pTarget_Manager->Begin_MRT(TEXT("MRT_Deferred")));
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("MRT_Deferred"))));

	for (auto& pGameObject : m_RenderObjects[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_PRIORITY].clear();


	for (auto& pGameObject : m_RenderObjects[RENDER_NONALPHABLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_NONALPHABLEND].clear();




	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	return S_OK;
}

HRESULT CRender_Manager::Render_Decal()
{
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Decal"))));
	for (auto& pGameObject : m_RenderObjects[RENDER_DECAL])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render(m_pShader_Deferred);
			m_pVIBuffer->Render();
		}

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_DECAL].clear();
	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	//FAILEDCHECK(m_pTarget_Manager->Begin_MRT(TEXT("Target_Blur_Decal")));
	//_float	fPower = 0.5f;

	//// For VIBuffer
	//m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_fPower", &fPower, sizeof(_float));

	//FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Decal"))));

	//m_pShader_Blur->Begin(0);
	//m_pVIBuffer->Render();

	//FAILEDCHECK(m_pTarget_Manager->End_MRT());




	//FAILEDCHECK(m_pTarget_Manager->Begin_MRT(TEXT("Target_Decal")));

	//m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_fPower", &fPower, sizeof(_float));

	//FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Blur_Decal"))));
	//m_pShader_Blur->Begin(1);
	//m_pVIBuffer->Render();

	//FAILEDCHECK(m_pTarget_Manager->End_MRT());


	return S_OK;
}

HRESULT CRender_Manager::Render_Water()
{
#if defined(_WATER)

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Water"))));

	FAILEDCHECK(m_pShader_Water->Set_ShaderResourceView("g_DepthTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Depth"))));

	FAILEDCHECK(m_pShader_Water->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Water->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Water->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));


	_float		fDiffusePower = 0.1f;
	_float4		vMaxPower = { 0.f, 0.3f, 0.42f, 1.f };
	_float		fAlphaPower = 0.15f;
	_float		fYDepth = 10.5f;
	_float		fFar = MGR(CPipeLine)->Get_Viewport().MaxZ;
	FAILEDCHECK(m_pShader_Water->Set_RawValue("g_fDiffusePower", &fDiffusePower, sizeof(_float)));
	FAILEDCHECK(m_pShader_Water->Set_RawValue("g_vMaxPower", &vMaxPower, sizeof(_float4)));
	FAILEDCHECK(m_pShader_Water->Set_RawValue("g_fAlphaPower", &fAlphaPower, sizeof(_float)));
	FAILEDCHECK(m_pShader_Water->Set_RawValue("g_fYDepth", &fYDepth, sizeof(_float)));
	FAILEDCHECK(m_pShader_Water->Set_RawValue("g_fFar", &fFar, sizeof(_float)));

	_float4x4		CamViewMatrixInv, CamProjMatrixInv;
	XMStoreFloat4x4(&CamViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&CamProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ))));

	FAILEDCHECK(m_pShader_Water->Set_RawValue("g_CamViewMatrixInv", &CamViewMatrixInv, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Water->Set_RawValue("g_CamProjMatrixInv", &CamProjMatrixInv, sizeof(_float4x4)));

	m_pShader_Water->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

#endif
	return S_OK;
}

HRESULT CRender_Manager::Render_Cloud()
{
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Cloud"))));

	m_RenderObjects[RENDER_CLOUD].sort([](CGameObject* pSour, CGameObject* pDest)
		{
			return pSour->Get_CamDistance() > pDest->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[RENDER_CLOUD])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_CLOUD].clear();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

	return S_OK;
}

HRESULT CRender_Manager::Render_SSR()
{
#if defined(_SSR)

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_SSR"))));

	FAILEDCHECK(m_pShader_SSR->Set_ShaderResourceView("g_DepthTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Depth"))));
	FAILEDCHECK(m_pShader_SSR->Set_ShaderResourceView("g_NormalTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Normal"))));
	FAILEDCHECK(m_pShader_SSR->Set_ShaderResourceView("g_SSRflagTexture", m_pCurRenderer->Get_SRV(TEXT("Target_SSR_Flag"))));
	FAILEDCHECK(m_pShader_SSR->Set_ShaderResourceView("g_DiffuseTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Diffuse"))));

	FAILEDCHECK(m_pShader_SSR->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_SSR->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_SSR->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));


	_float		fFar = MGR(CPipeLine)->Get_Viewport().MaxZ;
	_float4		vBaseColor = { 0.f, 0.2f, 0.3f, 0.2f };
	FAILEDCHECK(m_pShader_SSR->Set_RawValue("g_fFar", &fFar, sizeof(_float)));
	FAILEDCHECK(m_pShader_SSR->Set_RawValue("g_vBaseColor", &vBaseColor, sizeof(_float4)));


	_float4x4		CamViewMatrixInv, CamProjMatrixInv;
	_float4x4		CamViewMatrix, CamProjMatrix;
	XMStoreFloat4x4(&CamViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&CamProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ))));
	XMStoreFloat4x4(&CamViewMatrix, XMMatrixTranspose(pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW)));
	XMStoreFloat4x4(&CamProjMatrix, XMMatrixTranspose(pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ)));

	FAILEDCHECK(m_pShader_SSR->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4)));
	FAILEDCHECK(m_pShader_SSR->Set_RawValue("g_CamViewMatrixInv", &CamViewMatrixInv, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_SSR->Set_RawValue("g_CamProjMatrixInv", &CamProjMatrixInv, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_SSR->Set_RawValue("g_CamViewMatrix", &CamViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_SSR->Set_RawValue("g_CamProjMatrix", &CamProjMatrix, sizeof(_float4x4)));

	m_pShader_SSR->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

#endif
	return S_OK;
}

HRESULT CRender_Manager::Render_Lights()
{
	/* 셰이드 타겟을 장치에 바인드한다. */
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("MRT_LightAcc"))));


	/* 모든 빛은 이 노멀텍스쳐(타겟)과 연산이 이뤄지면 되기때문에.
	모든 빛마다 각각 던질피룡가 없다. */
	FAILEDCHECK(m_pShader_Light->Set_ShaderResourceView("g_NormalTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Normal"))));
	FAILEDCHECK(m_pShader_Light->Set_ShaderResourceView("g_DepthTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Depth"))));


	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	FAILEDCHECK(m_pShader_Light->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Light->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Light->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));




	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	_float		fFar = pPipeLine->Get_Viewport().MaxZ;
	FAILEDCHECK(m_pShader_Light->Set_RawValue("g_fFar", &fFar, sizeof(_float)));

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ))));

	FAILEDCHECK(m_pShader_Light->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Light->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4)));

	FAILEDCHECK(m_pShader_Light->Set_RawValue("g_vCamPosition", &pPipeLine->Get_CamPosition(), sizeof(_float4)));

	RELEASE_INSTANCE(CPipeLine);



	m_pLight_Manager->Render_Lights(m_pShader_Light, m_pVIBuffer);

	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	return S_OK;
}

HRESULT CRender_Manager::Render_Screen_Blend()
{
	USEGAMEINSTANCE;

	_float		fFar = MGR(CPipeLine)->Get_Viewport().MaxZ;
	//_float fGammaPower = m_Debugfloat[1];
	_float fGammaPower = 1.521f;
	//_float fRevGammaPower = m_Debugfloat[0];
	_float fRevGammaPower = 1.3f;
	// Render Environment
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Environment"))));

	CLight* pSun = m_pLight_Manager->Get_Sun();
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_DiffuseTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Diffuse"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_ShadeTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Shade"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_DepthTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Depth"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_ToonFlagTexture", m_pCurRenderer->Get_SRV(TEXT("Target_ToonFlag"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_EffectTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Effect"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_AlphaBlendTexture", m_pCurRenderer->Get_SRV(TEXT("Target_AlphaBlend"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_NonLightTexture", m_pCurRenderer->Get_SRV(TEXT("Target_NonLight"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_ShadowTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Shadow"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_WaterTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Water"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_DistortionTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Distortion"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_SSRTexture", m_pCurRenderer->Get_SRV(TEXT("Target_SSR"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_OutlineTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Outline"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_NormalTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Normal"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_TerrainFlagTexture", m_pCurRenderer->Get_SRV(TEXT("Target_TerrainFlag"))));
	//FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_ShadeFlagTexture", m_pCurRenderer->Get_SRV(TEXT("Target_ShadeFlag"))));
	

	//_float		fPowerSSAO = m_Debugfloat[0];
	//_float		fPowerSSAO2 = m_Debugfloat[1];
	//_float		fMaxDistance = m_Debugfloat[2];
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fFogRate", &m_fFogRate, sizeof(_float)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_vFogColor", &m_vFogColor, sizeof(_float4)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fGammaPower", &fGammaPower, sizeof(_float)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fFar", &fFar, sizeof(_float)));
	//FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fPowerSSAO", &fPowerSSAO, sizeof(_float)));
	//FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fPowerSSAO2", &fPowerSSAO2, sizeof(_float)));
	//FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fMaxDistance", &fMaxDistance, sizeof(_float)));


	m_pShader_Deferred->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader_Deferred->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader_Deferred->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	_float4x4		CamViewMatrixInv, CamProjMatrixInv;
	XMStoreFloat4x4(&CamViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&CamProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ))));

	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_CamViewMatrixInv", &CamViewMatrixInv, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_CamProjMatrixInv", &CamProjMatrixInv, sizeof(_float4x4)));


	m_pShader_Deferred->Begin(2);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());



	// Render HDR
	Render_Screen_HDR();




	// Render ScreenEffect
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_ScreenEffect"))));

	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_EnvironmentTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Environment"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_DistortionTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Distortion"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_DiffuseTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Diffuse"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_DepthTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Depth"))));
	//FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_EffectTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Effect"))));
	//FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_UITexture", m_pCurRenderer->Get_SRV(TEXT("Target_UI"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_GlowTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Glow"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_GlowBluredTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Glow_Blured"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_BlurFlagTexture", m_pCurRenderer->Get_SRV(TEXT("Target_BlurFlag"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_RimLightTexture", m_pCurRenderer->Get_SRV(TEXT("Target_RimLight"))));

	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_CloudTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Cloud"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_CloudSkyTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Cloud_Sky"))));

	
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_LumiNewTexture", m_pCurRenderer->Get_SRV(TEXT("Target_DownSampling_1"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_LumiOldTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Luminance_Old"))));
	FAILEDCHECK(m_pNoiseTex->Set_ShaderResourceView(m_pShader_Deferred, "g_NoiseTexture", 675));
	
	_float2		vSunPos = { 0.f, 0.f };
	_bool		bIsSun = false;
	//CLight* pSun = m_pLight_Manager->Get_Sun();
	if (pSun)
	{
		bIsSun = true;
		vSunPos = SetUp_ViewportPos(XMLoadFloat4(&pSun->Get_LightDesc()->vPosition));

		_vector vCampos = XMLoadFloat4(&pGameInstance->Get_CamPosition());

		_float2		vLightUV;
		vCampos = XMVector3TransformCoord(vCampos, pSun->Get_ViewMatrix());
		vCampos = XMVector3TransformCoord(vCampos, pSun->Get_ProjMatrix());

		_float		fCamDepth = XMVectorGetZ(vCampos);

		vLightUV.x = (XMVectorGetX(vCampos) + 1.f) * 0.5f;
		vLightUV.y = (XMVectorGetY(vCampos) - 1.f) * -0.5f;

		float		fBias = 0.003f;
		if (1.f - fBias < vLightUV.x ||
			1.f - fBias < vLightUV.y ||
			0.f + fBias > vLightUV.x ||
			0.f + fBias > vLightUV.y)
			fCamDepth = 0.f;

		FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_LightDepthTexture", pSun->Get_RendetTarget()->Get_SRV()));
		FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_vLightUV", &vLightUV, sizeof(_float2)));
		FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fCamDepth", &fCamDepth, sizeof(_float)));
	}

	_float		fSunPower = 1.f;

	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fDOFRate", &m_fDOFRate, sizeof(_float)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fFar", &fFar, sizeof(_float)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fGammaPower", &fGammaPower, sizeof(_float)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fRevGammaPower", &fRevGammaPower, sizeof(_float)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_vSunPos", &vSunPos, sizeof(_float2)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_bIsSun", &bIsSun, sizeof(_bool)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fSunPower", &fSunPower, sizeof(_float)));
	

	//_float fMiddleGrey = m_Debugfloat[2];
	//_float fLumWhiteSqr = m_Debugfloat[3];
	_float fMiddleGrey = 2.5f;
	_float fLumWhiteSqr = 3.5f;
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fMiddleGrey", &fMiddleGrey, sizeof(_float)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fLumWhiteSqr", &fLumWhiteSqr, sizeof(_float)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fSaturationRate", &m_fSaturationRate, sizeof(_float)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fBrightness", &m_fBrightness, sizeof(_float)));

	_uint		iBlurPower = 0;// m_Debugint[0];
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_iBlurPower", &iBlurPower, sizeof(_uint)));

	m_pShader_Deferred->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader_Deferred->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader_Deferred->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));



	m_pShader_Deferred->Begin(3);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	D3D11_VIEWPORT				OldViewPortDesc;
	ZeroMemory(&OldViewPortDesc, sizeof(D3D11_VIEWPORT));
	_uint		iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &OldViewPortDesc);
	m_pContext->RSSetViewports(1, &m_pCurRenderer->Find_Viewport(TEXT("Target_DownSampling_1")));
	Render_Screen_Copy(TEXT("Target_Luminance_Old"), TEXT("Target_Luminance_Old_Copy"), m_pCurRenderer->Find_DepthStencilView(TEXT("Target_DownSampling_1")));
	Render_Screen_CopyBlend(TEXT("Target_Luminance_Old_Copy"), TEXT("Target_DownSampling_1"), TEXT("Target_Luminance_Old"), 0.1f, m_pCurRenderer->Find_DepthStencilView(TEXT("Target_DownSampling_1")));
	m_pContext->RSSetViewports(1, &OldViewPortDesc);


	//_float2	vPointBlurPos = { m_Debugfloat[0] / m_ViewPortDesc.Width, m_Debugfloat[1] / m_ViewPortDesc.Height };
	//_float	fPointBlurPower = m_Debugfloat[2];
	//_float	fBlurStart = m_Debugfloat[3];
	//_float	fBlurWidth = m_Debugfloat[4];

	Render_RadialBlur(TEXT("Target_ScreenEffect"), TEXT("Target_ScreenEffect"), m_vRadialBlurPos, m_fRadialBlurPower, m_fRadialBlurStart, m_fRadialBlurWidth);


	// Render AfterEffect
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));

	//_float fBright = m_Debugfloat[0];
	_float fBright = 1.f;
	//_float fGammaPower = 1.f;

	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fBright", &fBright, sizeof(_float)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_fGammaPower", &fGammaPower, sizeof(_float)));
	
	//_float2 vRCADir = { m_Debugfloat[0] / m_ViewPortDesc.Width, m_Debugfloat[1] / m_ViewPortDesc.Height };
	//_float2 vGCADir = { m_Debugfloat[2] / m_ViewPortDesc.Width, m_Debugfloat[3] / m_ViewPortDesc.Height };
	//_float2 vBCADir = { m_Debugfloat[4] / m_ViewPortDesc.Width, m_Debugfloat[5] / m_ViewPortDesc.Height };
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_vRCADir", &m_vRCADir, sizeof(_float2)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_vGCADir", &m_vGCADir, sizeof(_float2)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_vBCADir", &m_vBCADir, sizeof(_float2)));

	//_float2	vPointCAPos = { m_Debugfloat[0] / m_ViewPortDesc.Width, m_Debugfloat[1] / m_ViewPortDesc.Height };
	//_float4	vPointCAPower = { m_Debugfloat[2] , m_Debugfloat[3], m_Debugfloat[4], m_Debugfloat[5] };
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_vRadialCAPos", &m_vRadialCAPos, sizeof(_float2)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_vRadialCAPower", &m_vRadialCAPower, sizeof(_float4)));

	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_UITexture", m_pCurRenderer->Get_SRV(TEXT("Target_UI"))));
	FAILEDCHECK(m_pShader_Deferred->Set_ShaderResourceView("g_ScreenEffectTexture", m_pCurRenderer->Get_SRV(TEXT("Target_ScreenEffect"))));

	m_pShader_Deferred->Begin(4);
	m_pVIBuffer->Render();





	return S_OK;
}

HRESULT CRender_Manager::Render_Screen_HDR()
{
#if defined(_HDR)
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Luminance"))));


	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));

	FAILEDCHECK(m_pShader_DownSampling->Set_ShaderResourceView("g_TargetTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Environment"))));

	m_pShader_DownSampling->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());




	D3D11_VIEWPORT				OldViewPortDesc;
	ZeroMemory(&OldViewPortDesc, sizeof(D3D11_VIEWPORT));
	_uint		iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &OldViewPortDesc);


	//128
	m_pContext->RSSetViewports(1, &m_pCurRenderer->Find_Viewport(TEXT("Target_DownSampling_128")));
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_DownSampling_128")), m_pCurRenderer->Find_DepthStencilView(TEXT("Target_DownSampling_128"))));

	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));

	_float	fWinCX = 128.f;
	_float	fWinCY = 128.f;
	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_fWinCX", &fWinCX, sizeof(_float)));
	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_fWinCY", &fWinCY, sizeof(_float)));


	FAILEDCHECK(m_pShader_DownSampling->Set_ShaderResourceView("g_LumninanceTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Luminance"))));

	m_pShader_DownSampling->Begin(1);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	// 64 ~ 2
	for (size_t i = 0; i < 6; ++i)
	{
		_uint iScale = pow(2, 6 - i);
		_uint iSRVScale = pow(2, 7 - i);
		
		wstring	strScale = to_wstring(iScale);
		wstring	strSRVScale = to_wstring(iSRVScale);
		wstring	strTargetName = TEXT("Target_DownSampling_");
		wstring	strMRTName = strTargetName + strScale;
		wstring	strSRVName = strTargetName + strSRVScale;

		m_pContext->RSSetViewports(1, &m_pCurRenderer->Find_Viewport(strMRTName.c_str()));
		FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(strMRTName.c_str()), m_pCurRenderer->Find_DepthStencilView(strMRTName.c_str())));

		FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
		FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
		FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));

		fWinCX = (_float)iScale;
		fWinCY = (_float)iScale;
		FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_fWinCX", &fWinCX, sizeof(_float)));
		FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_fWinCY", &fWinCY, sizeof(_float)));

		FAILEDCHECK(m_pShader_DownSampling->Set_ShaderResourceView("g_LumninanceTexture", m_pCurRenderer->Get_SRV(strSRVName.c_str())));

		m_pShader_DownSampling->Begin(1);
		m_pVIBuffer->Render();

		FAILEDCHECK(m_pTarget_Manager->End_MRT());
	}


	//1
	m_pContext->RSSetViewports(1, &m_pCurRenderer->Find_Viewport(TEXT("Target_DownSampling_1")));
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_DownSampling_1")), m_pCurRenderer->Find_DepthStencilView(TEXT("Target_DownSampling_1"))));

	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));

	fWinCX = 1.f;
	fWinCY = 1.f;
	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_fWinCX", &fWinCX, sizeof(_float)));
	FAILEDCHECK(m_pShader_DownSampling->Set_RawValue("g_fWinCY", &fWinCY, sizeof(_float)));

	FAILEDCHECK(m_pShader_DownSampling->Set_ShaderResourceView("g_LumninanceTexture", m_pCurRenderer->Get_SRV(TEXT("Target_DownSampling_2"))));

	m_pShader_DownSampling->Begin(1);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

	m_pContext->RSSetViewports(1, &OldViewPortDesc);


#endif

	return S_OK;
}

HRESULT CRender_Manager::Render_NonLight()
{
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_NonLight"))));

	for (auto& pGameObject : m_RenderObjects[RENDER_NONLIGHT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_NONLIGHT].clear();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

	return S_OK;
}

HRESULT CRender_Manager::Render_AlphaBlend()
{
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_AlphaBlend"))));

	m_RenderObjects[RENDER_ALPHABLEND].sort([](CGameObject* pSour, CGameObject* pDest)
		{
			return pSour->Get_CamDistance() > pDest->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[RENDER_ALPHABLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_ALPHABLEND].clear();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	return S_OK;
}

HRESULT CRender_Manager::Render_Blur(wstring strInMRTTag, wstring strOutMRTTag, _uint iPower)
{
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Blur")), m_pCurRenderer->Find_DepthStencilView(TEXT("Target_Blur"))));

	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));


	_float fSigma = ((_float)iPower) * 0.25f;
	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_iPower", &iPower, sizeof(_int)));
	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_fSigma", &fSigma, sizeof(_float)));

	FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pCurRenderer->Get_SRV(strInMRTTag.c_str())));

	m_pShader_Blur->Begin(2);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());



	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(strOutMRTTag.c_str())));

	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_ShaderResourceView("g_CopyTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Blur"))));

	m_pShader_Copy->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	return S_OK;
}

HRESULT CRender_Manager::Render_RadialBlur(wstring strInMRTTag, wstring strOutMRTTag, _float2 vPoint, _uint iPower, _float fBlurStart, _float fBlurWidth)
{
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Blur")), m_pCurRenderer->Find_DepthStencilView(TEXT("Target_Blur"))));

	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));


	_float	fSigma = ((_float)iPower) * 0.25f;
	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_iPower", &iPower, sizeof(_int)));
	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_fSigma", &fSigma, sizeof(_float)));
	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_vPoint", &vPoint, sizeof(_float2)));
	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_fBlurStart", &fBlurStart, sizeof(_float)));
	FAILEDCHECK(m_pShader_Blur->Set_RawValue("g_fBlurWidth", &fBlurWidth, sizeof(_float)));
	

	FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pCurRenderer->Get_SRV(strInMRTTag.c_str())));

	m_pShader_Blur->Begin(3);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());



	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(strOutMRTTag.c_str())));

	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_ShaderResourceView("g_CopyTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Blur"))));

	m_pShader_Copy->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

	return S_OK;
}

HRESULT CRender_Manager::Render_Glow()
{
	Render_Blur_2Pass(TEXT("Target_Glow"), TEXT("Target_Glow_Blured"), 1.05f, 2.f, 4);
	//Render_Blur(TEXT("Target_Glow"), TEXT("Target_Glow_Blured"), 10);

	return S_OK;
}

HRESULT CRender_Manager::Render_Screen_SSAO()
{
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_SSAO"))));


	m_pShader_SSAO->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader_SSAO->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader_SSAO->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	FAILEDCHECK(m_pShader_SSAO->Set_ShaderResourceView("g_DepthTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Depth"))));
	FAILEDCHECK(m_pShader_SSAO->Set_ShaderResourceView("g_NormalTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Normal"))));


	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);
	_float		fFar = pPipeLine->Get_Viewport().MaxZ;
	_float	fPower = 1.f;
	m_pShader_SSAO->Set_RawValue("g_fFar", &fFar, sizeof(_float));
	m_pShader_SSAO->Set_RawValue("g_fPower", &fPower, sizeof(_float));
	RELEASE_INSTANCE(CPipeLine);

	m_pShader_SSAO->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());



	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Blur_SSAO"))));


	//_float4x4	ProjMatrix;
	//XMStoreFloat4x4(&ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(m_ViewPortDesc.Width, m_ViewPortDesc.Height, 0.f, 1.f)));

	// For VIBuffer
	fPower = 1.f;
	m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	m_pShader_Blur->Set_RawValue("g_fPower", &fPower, sizeof(_float));

	FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pCurRenderer->Get_SRV(TEXT("Target_SSAO"))));


	m_pShader_Blur->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_SSAO"))));


	//_float4x4	ProjMatrix;
	//XMStoreFloat4x4(&ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(m_ViewPortDesc.Width, m_ViewPortDesc.Height, 0.f, 1.f)));

	m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	m_pShader_Blur->Set_RawValue("g_fPower", &fPower, sizeof(_float));

	FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Blur_SSAO"))));


	m_pShader_Blur->Begin(1);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	return S_OK;
}

HRESULT CRender_Manager::Render_UI()
{
	FAILEDCHECK(m_pTarget_Manager->BeginOverwrite_MRT(m_pCurRenderer->Find_MRT(TEXT("MRT_Deferred_UI")), m_pCurRenderer->Find_DepthStencilView(TEXT("Target_UI"))));

	m_RenderObjects[RENDER_UISOLID].sort([](CGameObject* pSour, CGameObject* pDest)
		{
			return pSour->Get_CamDistance() > pDest->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[RENDER_UISOLID])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_UISOLID].clear();

	m_RenderObjects[RENDER_UI].sort([](CGameObject* pSour, CGameObject* pDest)
		{
			return pSour->Get_RenderLayer() > pDest->Get_RenderLayer();
		});

	for (auto& pGameObject : m_RenderObjects[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_UI].clear();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	return S_OK;
}

HRESULT CRender_Manager::Render_Effect()
{
	//FAILEDCHECK(m_pTarget_Manager->BeginOverwrite_MRT(m_pCurRenderer->Find_MRT(TEXT("MRT_Effect"))));
	FAILEDCHECK(m_pTarget_Manager->BeginOverwrite_MRT(m_pCurRenderer->Find_MRT(TEXT("MRT_Effect"))));

	m_RenderObjects[RENDER_EFFECT].sort([](CGameObject* pSour, CGameObject* pDest)
		{
			return pSour->Get_CamDistance() > pDest->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[RENDER_EFFECT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_EFFECT].clear();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());



	FAILEDCHECK(m_pTarget_Manager->BeginOverwrite_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Distortion"))));

	m_RenderObjects[RENDER_EFFECT_DISTORTION].sort([](CGameObject* pSour, CGameObject* pDest)
		{
			return pSour->Get_CamDistance() > pDest->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[RENDER_EFFECT_DISTORTION])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_EFFECT_DISTORTION].clear();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

	return S_OK;
}

HRESULT CRender_Manager::Render_Water_Sueface()
{
	FAILEDCHECK(m_pTarget_Manager->BeginOverwrite_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Water_Surface"))));

	for (auto& pGameObject : m_RenderObjects[RENDER_WATER_SURFACE])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_WATER_SURFACE].clear();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

	return S_OK;
}

HRESULT CRender_Manager::Render_Cloud_Sky()
{
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Cloud_Sky"))));

	for (auto& pGameObject : m_RenderObjects[RENDER_CLOUD_SKY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_CLOUD_SKY].clear();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

	return S_OK;
}

HRESULT CRender_Manager::Render_Screen_Shadow()
{
#if defined(_SHADOW)

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Shadow"))));

	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));


	_float		fPower = 2.5f;
	_float		fFar = MGR(CPipeLine)->Get_Viewport().MaxZ;
	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_fPower", &fPower, sizeof(_float)));
	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_fFar", &fFar, sizeof(_float)));

	_float4x4		CamViewMatrixInv, CamProjMatrixInv;
	XMStoreFloat4x4(&CamViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&CamProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ))));

	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_CamViewMatrixInv", &CamViewMatrixInv, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_CamProjMatrixInv", &CamProjMatrixInv, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Shadow->Set_ShaderResourceView("g_DepthTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Depth"))));

	_int iNumLight = (_int)m_pLight_Manager->Get_RenderLights()->size();
	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_iNumLights", &iNumLight, sizeof(_int)));

	m_pLight_Manager->Render_Shadows(m_pShader_Shadow, m_pVIBuffer);

	//m_pShader_Shadow->Begin(0);
	//m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());



	//Render_Blur(TEXT("Target_Shadow"), TEXT("Target_Shadow"), 1.f, 1.f, 1);
	Render_Blur(TEXT("Target_Shadow"), TEXT("Target_Shadow"), 10.f);
	//Render_Blur(TEXT("Target_Shadow"), TEXT("Target_Shadow"), 1.1f, 2.f, 2);


#endif

	return S_OK;
}

HRESULT CRender_Manager::Render_Screen_Shadow_Dir()
{
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Shadow"))));

	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));


	_float		fPower = 2.5f;
	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_fPower", &fPower, sizeof(_float)));

	_float		fFar = MGR(CPipeLine)->Get_Viewport().MaxZ;
	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_fFar", &fFar, sizeof(_float)));



	USEGAMEINSTANCE;
	_float4x4		CamViewMatrixInv, CamProjMatrixInv;
	XMStoreFloat4x4(&CamViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&CamProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ))));

	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_CamViewMatrixInv", &CamViewMatrixInv, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_CamProjMatrixInv", &CamProjMatrixInv, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Shadow->Set_ShaderResourceView("g_DepthTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Depth"))));

	_int iNumLight = (_int)m_pLight_Manager->Get_RenderLights()->size();
	FAILEDCHECK(m_pShader_Shadow->Set_RawValue("g_iNumLights", &iNumLight, sizeof(_int)));

	m_pLight_Manager->Render_Shadows(m_pShader_Shadow, m_pVIBuffer);

	m_pShader_Shadow->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	//// XY 블러로 뭉개는 부분
	//FAILEDCHECK(m_pTarget_Manager->Begin_MRT(TEXT("Target_Blur_Shadow")));

	////_float4x4	ProjMatrix;
	////XMStoreFloat4x4(&ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(m_ViewPortDesc.Width, m_ViewPortDesc.Height, 0.f, 1.f)));
	//fPower = 2.f;

	//// For VIBuffer
	//m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_fPower", &fPower, sizeof(_float));

	//FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Shadow"))));

	//m_pShader_Blur->Begin(0);
	//m_pVIBuffer->Render();

	//FAILEDCHECK(m_pTarget_Manager->End_MRT());



	//FAILEDCHECK(m_pTarget_Manager->Begin_MRT(TEXT("Target_Shadow")));
	////_float4x4	ProjMatrix;
	////XMStoreFloat4x4(&ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(m_ViewPortDesc.Width, m_ViewPortDesc.Height, 0.f, 1.f)));

	//m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_fPower", &fPower, sizeof(_float));

	//FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Blur_Shadow"))));


	//m_pShader_Blur->Begin(1);
	//m_pVIBuffer->Render();

	//FAILEDCHECK(m_pTarget_Manager->End_MRT());


	return S_OK;
}

HRESULT CRender_Manager::Render_Screen_Outline()
{
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Outline"))));

	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));


	_float		fPower = 2.5f;
	_float		fFar = MGR(CPipeLine)->Get_Viewport().MaxZ;
	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_fPower", &fPower, sizeof(_float)));
	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_fFar", &fFar, sizeof(_float)));

	FAILEDCHECK(m_pShader_Outline->Set_ShaderResourceView("g_DepthTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Depth"))));

	m_pShader_Outline->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	//// XY 블러로 뭉개는 부분
	//FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Blur_Outline"))));
	//fPower = 2.f;
	//// For VIBuffer
	//m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_fPower", &fPower, sizeof(_float));

	//FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Shadow"))));

	//m_pShader_Blur->Begin(0);
	//m_pVIBuffer->Render();

	//FAILEDCHECK(m_pTarget_Manager->End_MRT());



	//FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Shadow"))));
	//m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	//m_pShader_Blur->Set_RawValue("g_fPower", &fPower, sizeof(_float));

	//FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Blur_Outline"))));


	//m_pShader_Blur->Begin(1);
	//m_pVIBuffer->Render();

	//FAILEDCHECK(m_pTarget_Manager->End_MRT());

	return S_OK;
}

HRESULT CRender_Manager::Render_Screen_Outline_Unit()
{
#if defined(_OUTLINE_UNIT)
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Outline"))));

	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));


	//_float		fDepthDiff = m_Debugfloat[0];
	//_float		fMaxDepthDiff = m_Debugfloat[1];
	//_float		fLineBrightness = m_Debugfloat[2];
	//_float		fThickness = m_Debugfloat[3];
	_float		fDepthDiff = 0.016f;
	_float		fMaxDepthDiff = 10.f;
	//_float		fLineBrightness = 0.33f;
	//_float		fLineBrightness = 0.583f;
	_float		fThickness = 1.f;
	_float		fFar = MGR(CPipeLine)->Get_Viewport().MaxZ;
	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_fDepthDiff", &fDepthDiff, sizeof(_float)));
	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_fMaxDepthDiff", &fMaxDepthDiff, sizeof(_float)));
	//FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_fLineBrightness", &fLineBrightness, sizeof(_float)));
	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_fThickness", &fThickness, sizeof(_float)));
	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_fFar", &fFar, sizeof(_float)));

	

	FAILEDCHECK(m_pShader_Outline->Set_ShaderResourceView("g_DepthTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Depth"))));
	FAILEDCHECK(m_pShader_Outline->Set_ShaderResourceView("g_FlagTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Outline_Flag"))));

	m_pShader_Outline->Begin(2);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

#endif
	return S_OK;
}

HRESULT CRender_Manager::Render_Bloom()
{

	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Final"))));


	//_float4x4	ProjMatrix;
	//XMStoreFloat4x4(&ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(m_ViewPortDesc.Width, m_ViewPortDesc.Height, 0.f, 1.f)));
	_float	fPower = 1.f;

	m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	m_pShader_Blur->Set_RawValue("g_fPower", &fPower, sizeof(_float));

	FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Bloom"))));


	m_pShader_Blur->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());




	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Bloom"))));


	//_float4x4	ProjMatrix;
	//XMStoreFloat4x4(&ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(m_ViewPortDesc.Width, m_ViewPortDesc.Height, 0.f, 1.f)));

	m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	m_pShader_Blur->Set_RawValue("g_fPower", &fPower, sizeof(_float));

	FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Blur_Bloom"))));


	m_pShader_Blur->Begin(1);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

	return S_OK;
}

HRESULT CRender_Manager::Render_Blur_2Pass(wstring strInMRTTag, wstring strOutMRTTag, _float fPower, _float fRange, _uint iNumRepeat)
{
	D3D11_VIEWPORT				ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	_uint		iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);
	for (size_t i = 0; i < iNumRepeat; ++i)
	{

		FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_BlurX")), m_pCurRenderer->Find_DepthStencilView(TEXT("Target_Blur"))));

		//_float	fPower = 1.f;

		m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
		m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
		m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
		m_pShader_Blur->Set_RawValue("g_fPower", &fPower, sizeof(_float));
		m_pShader_Blur->Set_RawValue("g_fRange", &fRange, sizeof(_float));
		 
		//FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Flag"))));
		if (0 == i)
		{
			FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pCurRenderer->Get_SRV(strInMRTTag.c_str())));
		}
		else
		{
			FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pCurRenderer->Get_SRV(TEXT("Target_BlurY"))));
		}


		m_pShader_Blur->Begin(0);
		m_pVIBuffer->Render();

		FAILEDCHECK(m_pTarget_Manager->End_MRT());




		FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_BlurY")), m_pCurRenderer->Find_DepthStencilView(TEXT("Target_Blur"))));

		m_pShader_Blur->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
		m_pShader_Blur->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
		m_pShader_Blur->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
		m_pShader_Blur->Set_RawValue("g_fPower", &fPower, sizeof(_float));
		m_pShader_Blur->Set_RawValue("g_fRange", &fRange, sizeof(_float));

		FAILEDCHECK(m_pShader_Blur->Set_ShaderResourceView("g_BlurTexture", m_pCurRenderer->Get_SRV(TEXT("Target_BlurX"))));


		m_pShader_Blur->Begin(1);
		m_pVIBuffer->Render();

		FAILEDCHECK(m_pTarget_Manager->End_MRT());
	}



	//FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Final"))));
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(strOutMRTTag.c_str())));

	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_ShaderResourceView("g_CopyTexture", m_pCurRenderer->Get_SRV(TEXT("Target_BlurY"))));

	m_pShader_Copy->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

	m_pContext->RSSetViewports(iNumViewports, &ViewPortDesc);

	return S_OK;
}

HRESULT CRender_Manager::Render_Screen_Bloom()
{
	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Screen_Bloom_Ori"))));

	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_ShaderResourceView("g_CopyTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Final"))));
	//FAILEDCHECK(m_pShader_Copy->Set_ShaderResourceView("g_CopyTexture", m_pCurRenderer->Find_MRT(TEXT("Target_Final"))));

	m_pShader_Copy->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());



	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Screen_Bloom"))));

	FAILEDCHECK(m_pShader_Bloom->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Bloom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Bloom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));

	FAILEDCHECK(m_pShader_Bloom->Set_ShaderResourceView("g_FinalTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Final"))));

	m_pShader_Bloom->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	
	Render_Blur(TEXT("Target_Screen_Bloom"), TEXT("Target_Screen_Bloom_Blur"), 10.f);



	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Final"))));

	_float fPower = 1.3f;
	FAILEDCHECK(m_pShader_Bloom->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Bloom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Bloom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));
	m_pShader_Bloom->Set_RawValue("g_fPower", &fPower, sizeof(_float));

	FAILEDCHECK(m_pShader_Bloom->Set_ShaderResourceView("g_FinalTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Screen_Bloom_Ori"))));
	FAILEDCHECK(m_pShader_Bloom->Set_ShaderResourceView("g_BloomBlurTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Screen_Bloom_Blur"))));
	FAILEDCHECK(m_pShader_Bloom->Set_ShaderResourceView("g_BloomTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Screen_Bloom"))));

	m_pShader_Bloom->Begin(1);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	return S_OK;
}

HRESULT CRender_Manager::Render_Screen_Blur()
{
	return S_OK;
}

HRESULT CRender_Manager::Render_Screen_Copy(wstring strInMRTTag, wstring strOutMRTTag, ID3D11DepthStencilView* pDepthStencilView)
{
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(strOutMRTTag.c_str()), pDepthStencilView));

	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_ShaderResourceView("g_CopyTexture", m_pCurRenderer->Get_SRV(strInMRTTag.c_str())));

	m_pShader_Copy->Begin(0);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

	return S_OK;
}

HRESULT CRender_Manager::Render_Screen_CopyBlend(wstring strSourMRTTag, wstring strDestMRTTag, wstring strOutMRTTag, _float fBlendRate, ID3D11DepthStencilView* pDepthStencilView)
{
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(strOutMRTTag.c_str()), pDepthStencilView));

	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_ShaderResourceView("g_SourTexture", m_pCurRenderer->Get_SRV(strSourMRTTag.c_str())));
	FAILEDCHECK(m_pShader_Copy->Set_ShaderResourceView("g_DestTexture", m_pCurRenderer->Get_SRV(strDestMRTTag.c_str())));

	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_fBlendRate", &fBlendRate, sizeof(_float)));

	m_pShader_Copy->Begin(1);
	m_pVIBuffer->Render();

	FAILEDCHECK(m_pTarget_Manager->End_MRT());

	return S_OK;
}

HRESULT CRender_Manager::Render_FinalTexture()
{
	//FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Final"))));

	//Render_Blur(TEXT("Target_Final"), TEXT("Target_Final"), 30);

	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Copy->Set_ShaderResourceView("g_CopyTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Final"))));
	//FAILEDCHECK(m_pShader_Copy->Set_ShaderResourceView("g_CopyTexture", m_pCurRenderer->Find_MRT(TEXT("Target_Final"))));

	m_pShader_Copy->Begin(0);
	m_pVIBuffer->Render();

	//FAILEDCHECK(m_pTarget_Manager->End_MRT());

	return S_OK;
}

HRESULT CRender_Manager::Render_Outline()
{
	/* 렌더타겟을 장치에 Diffuse + Normal 바인딩한ㄷ앋. */
	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(m_pCurRenderer->Find_MRT(TEXT("Target_Final"))));

	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Outline->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Outline->Set_ShaderResourceView("g_FlagTexture", m_pCurRenderer->Get_SRV(TEXT("Target_Flag"))));


	m_pShader_Outline->Begin(1);
	m_pVIBuffer->Render();


	FAILEDCHECK(m_pTarget_Manager->End_MRT());


	return S_OK;
}

#ifdef _DEBUG
HRESULT CRender_Manager::Render_Debug()
{
	for (auto& pComponent : m_DebugComponents)
	{
		if (nullptr != pComponent)
			pComponent->Render();

		Safe_Release(pComponent);
	}

	m_DebugComponents.clear();

	if (nullptr == m_pTarget_Manager)
		__debugbreak();

	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(m_pShader_Deferred->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));


	m_pCurRenderer->Render_Debug(TEXT("MRT_Deferred"), m_pShader_Deferred, m_pVIBuffer);
	m_pCurRenderer->Render_Debug(TEXT("MRT_LightAcc"), m_pShader_Deferred, m_pVIBuffer);

	m_pCurRenderer->Render_Debug(TEXT("Target_BlurX"), m_pShader_Deferred, m_pVIBuffer);
	m_pCurRenderer->Render_Debug(TEXT("Target_BlurY"), m_pShader_Deferred, m_pVIBuffer);

	m_pCurRenderer->Render_Debug(TEXT("Target_Shadow"), m_pShader_Deferred, m_pVIBuffer);
	m_pCurRenderer->Render_Debug(TEXT("MRT_Deferred_Terrain"), m_pShader_Deferred, m_pVIBuffer);

	m_pCurRenderer->Render_Debug(TEXT("Target_Outline"), m_pShader_Deferred, m_pVIBuffer);

	m_pCurRenderer->Render_Debug(TEXT("MRT_Effect"), m_pShader_Deferred, m_pVIBuffer);
	m_pCurRenderer->Render_Debug(TEXT("Target_Glow_Blured"), m_pShader_Deferred, m_pVIBuffer);

	m_pCurRenderer->Render_Debug(TEXT("Target_Cloud_Sky"), m_pShader_Deferred, m_pVIBuffer);
	m_pCurRenderer->Render_Debug(TEXT("Target_Cloud"), m_pShader_Deferred, m_pVIBuffer);
	
	CLight* pSun = m_pLight_Manager->Get_Sun();
	pSun->Get_RendetTarget()->Ready_Debug(1050.0f, 150.f, 100.f, 100.f);
	pSun->Get_RendetTarget()->Render_Debug(m_pShader_Deferred, m_pVIBuffer);


	MGR(CLight_Manager)->Get_ShadowLights()->back()->Get_RendetTarget()->Render_Debug(m_pShader_Deferred, m_pVIBuffer);


	return S_OK;
}

//HRESULT CRender_Manager::Render_SampleObejct()
//{
//	USEGAMEINSTANCE;
//	CCamera* pCurCamera = pGameInstance->Get_CurrentCamera();
//
//
//	CPipeLine* pPipeLine = CPipeLine::Get_Instance();
//	_matrix		ViewMatrix = XMMatrixInverse(nullptr, XMMatrixIdentity());
//	pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, ViewMatrix);
//	pPipeLine->Tick();
//
//
//	if (nullptr == m_pSampleObject)
//		return S_OK;
//
//	list<CRenderTarget*> MRTList;
//	MRTList.push_back(m_pSampleTarget);
//	FAILEDCHECK(m_pTarget_Manager->Begin_MRT(&MRTList));
//
//	m_pSampleObject->Render();
//
//	FAILEDCHECK(m_pTarget_Manager->End_MRT());
//
//	pGameInstance->Set_CurrentCamera(pCurCamera);
//
//
//	// For SampleTarget
//	m_pSampleTarget->Render_Debug(m_pShader_Deferred, m_pVIBuffer);
//	return S_OK;
//}

HRESULT CRender_Manager::Add_DebugRenderGroup(CComponent* pComponent)
{
	m_DebugComponents.push_back(pComponent);

	Safe_AddRef(pComponent);

	return S_OK;
}
#endif // _DEBUG


_float2 CRender_Manager::SetUp_ViewportPos(_vector vWorldPos)
{
	_vector vObjectPos = vWorldPos;

	_matrix	ViewMatrix = XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW));
	vObjectPos = XMVector3TransformCoord(vObjectPos, ViewMatrix);

	_matrix ProjMatrix = XMLoadFloat4x4(GAMEINSTANCE->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ));
	vObjectPos = XMVector3TransformCoord(vObjectPos, ProjMatrix);

	_float4 vProjPos;
	XMStoreFloat4(&vProjPos, vObjectPos);

	// 0,0 to 0.5,0.5
	vProjPos.x = (vProjPos.x + 1.f) * 0.5f;
	vProjPos.y = (vProjPos.y - 1.f) * -0.5f;

	//VIEWPORTDESC Desc = MGR(CPipeLine)->Get_Viewport();

	//_float4 vViewportPos;
	//vViewportPos.x = vProjPos.x * (Desc.Width * 0.5f);
	//vViewportPos.y = vProjPos.y * (Desc.Height * 0.5f);
	//vViewportPos.x += Desc.Width * 0.5f;
	//vViewportPos.y = Desc.Height * 0.5f - vViewportPos.y;

	return { vProjPos.x, vProjPos.y };
}

void CRender_Manager::Free()
{
	__super::Free();


#ifdef _DEBUG
	for (auto& pComponent : m_DebugComponents)
		Safe_Release(pComponent);

	//Safe_Release(m_pSampleTarget);

	m_DebugComponents.clear();
#endif // _DEBUG

	Safe_Release(m_pShader_Deferred);
	Safe_Release(m_pShader_Blur);
	Safe_Release(m_pShader_Shadow);
	Safe_Release(m_pShader_SSAO);
	Safe_Release(m_pShader_Light);
	Safe_Release(m_pShader_Outline);
	Safe_Release(m_pShader_Bloom);
	Safe_Release(m_pShader_Copy);
	Safe_Release(m_pShader_Glow);
	Safe_Release(m_pShader_Water);
	Safe_Release(m_pShader_SSR);
	Safe_Release(m_pShader_DOF);
	Safe_Release(m_pShader_DownSampling);
	
	Safe_Release(m_pNoiseTex);

	Safe_Release(m_pVIBuffer);
}