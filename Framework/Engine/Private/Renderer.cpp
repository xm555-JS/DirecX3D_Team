#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"

#include "GameInstance.h"
#include "RenderTarget.h"

CRenderer::CRenderer()
	: CComponent()
	, m_pTarget_Manager(CTarget_Manager::Get_Instance())
{
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject)
{
	if (eGroup >= RENDER_END ||
		nullptr == pGameObject)
		__debugbreak();

	m_RenderObjects[eGroup].push_back(pGameObject);
	pGameObject->Set_RenderState(true);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CRenderer::Initialize(void* pArg)
{
	if (nullptr == m_pTarget_Manager)
		__debugbreak();

	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;

	/* 렌더타겟들을 추가한다. */

	D3D11_VIEWPORT			m_ViewPortDesc;
	ZeroMemory(&m_ViewPortDesc, sizeof(D3D11_VIEWPORT));
	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &m_ViewPortDesc);


	for (size_t i = 0; i < RENDER_END; ++i)
		m_RenderObjects[i].clear();

#pragma region Add_RenderTarget

	/* For. Base Target*/
	m_RenderTargets.emplace(TEXT("Target_AlphaBlend"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_NonLight"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_UI"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Effect"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	Create_DepthStencilView(TEXT("Target_UI"), 1.f);


	// NonAlphaBlend
	m_RenderTargets.emplace(TEXT("Target_Diffuse"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Normal"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 1.f, 0.f, 1.f)));
	m_RenderTargets.emplace(TEXT("Target_Depth"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 1.f)));
	m_RenderTargets.emplace(TEXT("Target_Shade"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f)));
	m_RenderTargets.emplace(TEXT("Target_Specular"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_ToonFlag"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_ShadeFlag"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f)));
	

	/* For Glow*/
	m_RenderTargets.emplace(TEXT("Target_Glow_Diffuse"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Glow"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Glow_Blured"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));

	/* For RimLight*/
	m_RenderTargets.emplace(TEXT("Target_RimLight"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));


	/* For Shadow*/
	m_RenderTargets.emplace(TEXT("Target_Shadow"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));


	/* For Bloom*/
	m_RenderTargets.emplace(TEXT("Target_Screen_Bloom"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Screen_Bloom_Ori"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Screen_Bloom_Blur"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));

	/* For Blur*/
	_float		fTargetScale = 1.f;
	m_RenderTargets.emplace(TEXT("Target_Blur"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width * fTargetScale, m_ViewPortDesc.Height * fTargetScale, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_BlurX"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width * fTargetScale, m_ViewPortDesc.Height * fTargetScale, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_BlurY"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width * fTargetScale, m_ViewPortDesc.Height * fTargetScale, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	Create_DepthStencilView(TEXT("Target_Blur"), fTargetScale);
	Create_Viewport(TEXT("Target_Blur"), fTargetScale);

	/* For Distortion*/
	m_RenderTargets.emplace(TEXT("Target_Distortion"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Distortion_Depth"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f)));
	
	/* For Water*/
	m_RenderTargets.emplace(TEXT("Target_Water"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));

	/* For Terrain*/
	m_RenderTargets.emplace(TEXT("Target_TerrainFlag"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));

	/* For Cloud_Sky*/
	m_RenderTargets.emplace(TEXT("Target_Cloud_Sky"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Cloud"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));

	/* For SSR*/
	m_RenderTargets.emplace(TEXT("Target_SSR"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_SSR_Flag"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));

	/* For Outline*/
	//m_RenderTargets.emplace(TEXT("Target_Outline_Flag"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Outline"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Outline_Flag"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	
	/* For HDR*/
	m_RenderTargets.emplace(TEXT("Target_Luminance"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_DownSampling_128"), m_pTarget_Manager->Create_RenderTarget(128, 128, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_DownSampling_64"), m_pTarget_Manager->Create_RenderTarget(64, 64, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_DownSampling_32"), m_pTarget_Manager->Create_RenderTarget(32, 32, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_DownSampling_16"), m_pTarget_Manager->Create_RenderTarget(16, 16, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_DownSampling_8"), m_pTarget_Manager->Create_RenderTarget(8, 8, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_DownSampling_4"), m_pTarget_Manager->Create_RenderTarget(4, 4, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_DownSampling_2"), m_pTarget_Manager->Create_RenderTarget(2, 2, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_DownSampling_1"), m_pTarget_Manager->Create_RenderTarget(1, 1, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Luminance_Old"), m_pTarget_Manager->Create_RenderTarget(1, 1, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Luminance_Old_Copy"), m_pTarget_Manager->Create_RenderTarget(1, 1, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	Create_DepthStencilView_DowinScale(TEXT("Target_DownSampling_128"), 128);
	Create_DepthStencilView_DowinScale(TEXT("Target_DownSampling_64"), 64);
	Create_DepthStencilView_DowinScale(TEXT("Target_DownSampling_32"), 32);
	Create_DepthStencilView_DowinScale(TEXT("Target_DownSampling_16"), 16);
	Create_DepthStencilView_DowinScale(TEXT("Target_DownSampling_8"), 8);
	Create_DepthStencilView_DowinScale(TEXT("Target_DownSampling_4"), 4);
	Create_DepthStencilView_DowinScale(TEXT("Target_DownSampling_2"), 2);
	Create_DepthStencilView_DowinScale(TEXT("Target_DownSampling_1"), 1);
	Create_Viewport_DowinScale(TEXT("Target_DownSampling_128"), 128);
	Create_Viewport_DowinScale(TEXT("Target_DownSampling_64"), 64);
	Create_Viewport_DowinScale(TEXT("Target_DownSampling_32"), 32);
	Create_Viewport_DowinScale(TEXT("Target_DownSampling_16"), 16);
	Create_Viewport_DowinScale(TEXT("Target_DownSampling_8"), 8);
	Create_Viewport_DowinScale(TEXT("Target_DownSampling_4"), 4);
	Create_Viewport_DowinScale(TEXT("Target_DownSampling_2"), 2);
	Create_Viewport_DowinScale(TEXT("Target_DownSampling_1"), 1);


	/* For Final*/
	m_RenderTargets.emplace(TEXT("Target_Final"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_Environment"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	m_RenderTargets.emplace(TEXT("Target_ScreenEffect"), m_pTarget_Manager->Create_RenderTarget(m_ViewPortDesc.Width, m_ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	
#pragma endregion Add_RenderTarget


#pragma region Add_MRT

	/* For.MRT_Deferred */
	/* 조명연산이 필요한 객체들을 그릴 때, 추후 빛연산에 필요한 정보를 저장받기위한 렌더타겟들 (Diffuse + Normal ) */
	//FAILEDCHECK(Add_MRT(TEXT("Target_Priority"), TEXT("Target_Priority")));
	FAILEDCHECK(Add_MRT(TEXT("Target_AlphaBlend"), TEXT("Target_AlphaBlend")));
	FAILEDCHECK(Add_MRT(TEXT("Target_NonLight"), TEXT("Target_NonLight")));

	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred_UI"), TEXT("Target_UI")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred_UI"), TEXT("Target_Glow")));


	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Glow")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_ToonFlag")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Outline_Flag")));
	//FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_BlurFlag")));
	//FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_ShadeFlag")));
	FAILEDCHECK(Add_MRT(TEXT("Target_Normal"), TEXT("Target_Normal")));


	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred_Terrain"), TEXT("Target_Diffuse")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred_Terrain"), TEXT("Target_Normal")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred_Terrain"), TEXT("Target_Depth")));
	//FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred_Terrain"), TEXT("Target_ToonFlag")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred_Terrain"), TEXT("Target_Glow")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred_Terrain"), TEXT("Target_TerrainFlag")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred_Terrain"), TEXT("Target_Outline_Flag")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred_Terrain"), TEXT("Target_ToonFlag")));
	
	//FAILEDCHECK(Add_MRT(TEXT("MRT_Deferred_Terrain"), TEXT("Target_ShadeFlag")));


	FAILEDCHECK(Add_MRT(TEXT("Target_Glow_Blured"), TEXT("Target_Glow_Blured")));
	
	FAILEDCHECK(Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Effect")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Glow")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_Effect"), TEXT("Target_RimLight")));
	//FAILEDCHECK(Add_MRT(TEXT("MRT_Effect"), TEXT("Target_DistortionFlag")));

	FAILEDCHECK(Add_MRT(TEXT("Target_Distortion"), TEXT("Target_Distortion")));
	FAILEDCHECK(Add_MRT(TEXT("Target_Distortion"), TEXT("Target_Distortion_Depth")));
	
	/* For.MRT_LightAcc */
	/* 조명연산한 결과를 저장해놓기위한 타겟 (Shade + Specular) */
	FAILEDCHECK(Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade")));
	FAILEDCHECK(Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular")));

	/* For.Screen_Bloom */
	FAILEDCHECK(Add_MRT(TEXT("Target_Screen_Bloom"), TEXT("Target_Screen_Bloom")));
	FAILEDCHECK(Add_MRT(TEXT("Target_Screen_Bloom_Ori"), TEXT("Target_Screen_Bloom_Ori")));
	FAILEDCHECK(Add_MRT(TEXT("Target_Screen_Bloom_Blur"), TEXT("Target_Screen_Bloom_Blur")));

	/*For. Blur*/
	FAILEDCHECK(Add_MRT(TEXT("Target_BlurX"), TEXT("Target_BlurX")));
	FAILEDCHECK(Add_MRT(TEXT("Target_BlurY"), TEXT("Target_BlurY")));
	FAILEDCHECK(Add_MRT(TEXT("Target_Blur"), TEXT("Target_Blur")));

	/*For. Shadow*/
	FAILEDCHECK(Add_MRT(TEXT("Target_Shadow"), TEXT("Target_Shadow")));

	/*For. Water*/
	FAILEDCHECK(Add_MRT(TEXT("Target_Water"), TEXT("Target_Water")));
	
	/*For. Cloud*/
	FAILEDCHECK(Add_MRT(TEXT("Target_Cloud_Sky"), TEXT("Target_Cloud_Sky")));
	FAILEDCHECK(Add_MRT(TEXT("Target_Cloud"), TEXT("Target_Cloud")));
	

	/*For. Water_Surc*/
	FAILEDCHECK(Add_MRT(TEXT("Target_Water_Surface"), TEXT("Target_Normal")));
	FAILEDCHECK(Add_MRT(TEXT("Target_Water_Surface"), TEXT("Target_SSR_Flag")));
	FAILEDCHECK(Add_MRT(TEXT("Target_Water_Surface"), TEXT("Target_Depth")));
	//FAILEDCHECK(Add_MRT(TEXT("Target_Water_Surface"), TEXT("Target_Distortion")));

	/*For. SSR*/
	FAILEDCHECK(Add_MRT(TEXT("Target_SSR"), TEXT("Target_SSR")));
	
	/*For. Outline*/
	FAILEDCHECK(Add_MRT(TEXT("Target_Outline"), TEXT("Target_Outline")));


	/* For HDR*/
	FAILEDCHECK(Add_MRT(TEXT("Target_DownSampling_128"), TEXT("Target_DownSampling_128")));
	FAILEDCHECK(Add_MRT(TEXT("Target_DownSampling_64"), TEXT("Target_DownSampling_64")));
	FAILEDCHECK(Add_MRT(TEXT("Target_DownSampling_32"), TEXT("Target_DownSampling_32")));
	FAILEDCHECK(Add_MRT(TEXT("Target_DownSampling_16"), TEXT("Target_DownSampling_16")));
	FAILEDCHECK(Add_MRT(TEXT("Target_DownSampling_8"), TEXT("Target_DownSampling_8")));
	FAILEDCHECK(Add_MRT(TEXT("Target_DownSampling_4"), TEXT("Target_DownSampling_4")));
	FAILEDCHECK(Add_MRT(TEXT("Target_DownSampling_2"), TEXT("Target_DownSampling_2")));
	FAILEDCHECK(Add_MRT(TEXT("Target_DownSampling_1"), TEXT("Target_DownSampling_1")));
	FAILEDCHECK(Add_MRT(TEXT("Target_Luminance_Old"), TEXT("Target_Luminance_Old")));
	FAILEDCHECK(Add_MRT(TEXT("Target_Luminance_Old_Copy"), TEXT("Target_Luminance_Old_Copy")));
	FAILEDCHECK(Add_MRT(TEXT("Target_Luminance"), TEXT("Target_Luminance")));
	
	/*For. Final*/
	FAILEDCHECK(Add_MRT(TEXT("Target_Final"), TEXT("Target_Final")));
	FAILEDCHECK(Add_MRT(TEXT("Target_Environment"), TEXT("Target_Environment")));
	FAILEDCHECK(Add_MRT(TEXT("Target_ScreenEffect"), TEXT("Target_ScreenEffect")));
	//FAILEDCHECK(Add_MRT(TEXT("Target_AfterEffect"), TEXT("Target_AfterEffect")));
	


#pragma endregion Add_MRT


#ifdef _DEBUG
	FAILEDCHECK(Ready_Debug(TEXT("Target_Diffuse"), 50.0f, 50.f, 100.f, 100.f));
	FAILEDCHECK(Ready_Debug(TEXT("Target_Normal"), 50.0f, 150.f, 100.f, 100.f));
	FAILEDCHECK(Ready_Debug(TEXT("Target_Depth"), 50.0f, 250.f, 100.f, 100.f));
	FAILEDCHECK(Ready_Debug(TEXT("Target_Shade"), 150.0f, 50.f, 100.f, 100.f));
	FAILEDCHECK(Ready_Debug(TEXT("Target_Specular"), 150.0f, 150.f, 100.f, 100.f));

	//FAILEDCHECK(Ready_Debug(TEXT("Target_BlurX"), 350.0f, 50.f, 100.f, 100.f));
	//FAILEDCHECK(Ready_Debug(TEXT("Target_BlurY"), 350.0f, 150.f, 100.f, 100.f));


	FAILEDCHECK(Ready_Debug(TEXT("Target_Shadow"), 1050.f, 650.f, 100.f, 100.f));
	
	//FAILEDCHECK(Ready_Debug(TEXT("Target_Effect"), 1050.0f, 50.f, 100.f, 100.f));
	//FAILEDCHECK(Ready_Debug(TEXT("Target_Cloud"), 1050.0f, 150.f, 100.f, 100.f));
	//FAILEDCHECK(Ready_Debug(TEXT("Target_Cloud_Sky"), 1050.0f, 250.f, 100.f, 100.f));
	


#endif // _DEBUG


	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Render_Debug(const _tchar* pMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	list<class CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return S_OK;
	//__debugbreak();

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Render_Debug(pShader, pVIBuffer);
	}

	return S_OK;
}
#endif // _DEBUG

void CRenderer::Creat_DDSFromRT(const _tchar* pMRTTag, const _tchar* pPath)
{
	//USEGAMEINSTANCE;
	ID3D11Resource* pBuffer = nullptr;
	//pGameInstance->Get_BackBufferSRV()->GetResource(&pBackBuffer);
	CRenderTarget* pRT = Find_RenderTarget(pMRTTag);
	pRT->Get_SRV()->GetResource(&pBuffer);

	SaveDDSTextureToFile(m_pContext.Get(), pBuffer, pPath);

	//m_pCurRenderer->Find_RenderTarget(TEXT("Target_Diffuse"))->Get_SRV()->GetResource(&pBackBuffer);
	//SaveDDSTextureToFile(m_pContext.Get(), pBackBuffer, TEXT("../Bin/Temp/TempTex.dds"));
	//pBackBuffer->Release();
	//pBackBuffer = nullptr;

}


HRESULT CRenderer::Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pRenderTarget)
		__debugbreak();

	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(pMRTTag, MRTList);
	}
	else
	{
		pMRTList->push_back(pRenderTarget);
	}

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		__debugbreak();

	return pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);
}
#endif // _DEBUG

void CRenderer::Create_DepthStencilView(const _tchar* pTargetTag, _float fTargetScale)
{

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	_uint		iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);


	ID3D11DepthStencilView* pDepthStencilView = nullptr;
	ID3D11Texture2D* pDepthStencilTexture = nullptr;
	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = ViewPortDesc.Width * fTargetScale;
	TextureDesc.Height = ViewPortDesc.Height * fTargetScale;
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
	FAILEDCHECK(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &pDepthStencilView));

	Safe_Release(pDepthStencilTexture);

	m_DepthStencilViews.emplace(pTargetTag, pDepthStencilView);
}

void CRenderer::Create_DepthStencilView_DowinScale(const _tchar* pTargetTag, _uint iTargetScale)
{

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	_uint		iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);


	ID3D11DepthStencilView* pDepthStencilView = nullptr;
	ID3D11Texture2D* pDepthStencilTexture = nullptr;
	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iTargetScale;
	TextureDesc.Height = iTargetScale;
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
	FAILEDCHECK(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &pDepthStencilView));

	Safe_Release(pDepthStencilTexture);

	m_DepthStencilViews.emplace(pTargetTag, pDepthStencilView);
}

void CRenderer::Create_Viewport(const _tchar* pTargetTag, _float fTargetScale)
{
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	_uint		iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	//D3D11_VIEWPORT		m_pOldViewport[1];
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = ViewPortDesc.Width * fTargetScale;
	ViewPortDesc.Height = ViewPortDesc.Height * fTargetScale;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_Viewports.emplace(pTargetTag, ViewPortDesc);
}

void CRenderer::Create_Viewport_DowinScale(const _tchar* pTargetTag, _uint iTargetScale)
{
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	_uint		iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	//D3D11_VIEWPORT		m_pOldViewport[1];
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = iTargetScale;
	ViewPortDesc.Height = iTargetScale;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_Viewports.emplace(pTargetTag, ViewPortDesc);
}

CRenderTarget* CRenderer::Find_RenderTarget(const _tchar* pTargetTag)
{
	auto	iter = find_if(m_RenderTargets.begin(), m_RenderTargets.end(), CTag_Finder(pTargetTag));

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

ID3D11DepthStencilView* CRenderer::Find_DepthStencilView(const _tchar* pTargetTag)
{
	auto	iter = find_if(m_DepthStencilViews.begin(), m_DepthStencilViews.end(), CTag_Finder(pTargetTag));

	if (iter == m_DepthStencilViews.end())
	{
		__debugbreak();
		return nullptr;
	}
	
	return iter->second;
}

D3D11_VIEWPORT CRenderer::Find_Viewport(const _tchar* pTargetTag)
{
	auto	iter = find_if(m_Viewports.begin(), m_Viewports.end(), CTag_Finder(pTargetTag));

	if (iter == m_Viewports.end())
	{
		__debugbreak();
		return D3D11_VIEWPORT();
	}

	return iter->second;
}

list<CRenderTarget*>* CRenderer::Find_MRT(const _tchar* pMRTTag)
{
	auto	iter = find_if(m_MRTs.begin(), m_MRTs.end(), CTag_Finder(pMRTTag));

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

ID3D11ShaderResourceView* CRenderer::Get_SRV(const _tchar* pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		return nullptr;

	return pRenderTarget->Get_SRV();
}

CRenderer* CRenderer::Create()
{
	CRenderer* pInstance = new CRenderer();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRenderer::Clone(void* pArg)
{
	CRenderer* pInstance = new CRenderer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
	__super::Free();

	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
		{
			Safe_Release(pRenderTarget);
		}
		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);

	for (auto& Pair : m_DepthStencilViews)
		Safe_Release(Pair.second);

	m_RenderTargets.clear();

}


