#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Event_Manager.h"
#include "Sound_Manager.h"
#include "Imgui_Manager.h"
#include "Effect_Manager.h"
#include "Item_Manager.h"
#include "Quest_Manager.h"
#include "Order_Manager.h"
#include "Minigame_Manager.h"

#include "Easing_Utillity.h"

/* For.State*/
#include "State_Handler.h"
#include "State_Manager.h"

/* For.UI*/
#include "UI_Cursor.h"
#include "UI_Loading_Screen.h"
#include "UI_Loading_Bar.h"
#include "UI_Loading_Anim.h"

/* For.Camera*/
#include "Camera_Free.h"
#include "Camera_Fixed.h"
#include "Camera_Target_Matrix.h"
#include "Camera_Target_Position.h"

#include "Player.h"
#include "Enemy.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	//D3D11_SAMPLER_DESC

	FAILEDCHECK(Initialize_Engine());

	// MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont
	//FAILEDCHECK(m_pGameInstance->Add_Font(TEXT("Font_NotoSans"), TEXT("../Bin/Resources/Fonts/Noto_Black.spriteFont")));
	FAILEDCHECK(m_pGameInstance->Add_Font(TEXT("Font_Hancomdodum"), TEXT("../Bin/Resources/Fonts/Hancomdodum.spriteFont")));
	FAILEDCHECK(m_pGameInstance->Add_Font(TEXT("Font_Hancomdodum_Bold"), TEXT("../Bin/Resources/Fonts/Noto_Bold.spriteFont")));

	FAILEDCHECK(CSound_Manager::Get_Instance()->Initialize());
	FAILEDCHECK(CItem_Manager::Get_Instance()->Initialize());
	FAILEDCHECK(CQuest_Manager::Get_Instance()->Initialize());
	FAILEDCHECK(COrder_Manager::Get_Instance()->Initialize());
	FAILEDCHECK(CMinigame_Manager::Get_Instance()->Initialize());

#if defined(USE_IMGUI)
	FAILEDCHECK(CImgui_Manager::Get_Instance()->Initialize());
#endif
	FAILEDCHECK(Ready_Prototype_Component());
	FAILEDCHECK(Ready_Prototype_Camera());
	FAILEDCHECK(Ready_Prototype_Loading());
	FAILEDCHECK(Open_Level(LEVEL_LOGO));
	
	//ID_UINT Temp;
	//Temp.iID = 10002;
	//Temp.iID = 3859;

	//MGR(CSound_Manager)->PlaySound(TEXT("Player_Swim_2.mp3"), CSound_Manager::CHANNEL_PLAYER, 1.f, true);


	//_vector vPos0 = { 20.f, 0.f, 0.f, 1.f };
	//_vector vPos1 = { 20.f, 0.f, 0.f, 1.f };
	//_vector vPos2 = { 0.f, 0.f, 30.f, 1.f };
	//_vector vPos3 = { 0.f, 0.f, 30.f, 1.f };

	//XMVectorCatmullRom();

	//ID_UINT	Sour(2,5,17);
	//_uint	Test = Sour.iID;

	//ID_UINT Temp = Test;
	//ID_UINT Temp(Test);

	//Temp = Test;

	//Test();

	////test
	///* For.Prototype_GameObject_BackGround */
	//FAILEDCHECK(m_pGameInstance->Add_GameObject_Prototype(ID_UINT(PLAYER), CPlayer::Create()));
	//FAILEDCHECK(m_pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY), CEnemy::Create()));

	//NULLCHECK(m_pGameInstance->Add_GameObject(LEVEL_STATIC, ID_UINT(PLAYER), ID_UINT(PLAYER)));
	//NULLCHECK(m_pGameInstance->Add_GameObject(LEVEL_STATIC, ID_UINT(ENEMY), ID_UINT(ENEMY)));

	return S_OK;
}

void CMainApp::Tick(float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	m_fTimeAcc += fTimeDelta;

	CSound_Manager::Get_Instance()->Tick(fTimeDelta);
#if defined(USE_IMGUI)
	CImgui_Manager::Get_Instance()->Tick(fTimeDelta);
#endif
	m_pGameInstance->Tick_Engine(fTimeDelta);


}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance ||
		nullptr == m_pRenderer)
		__debugbreak();

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	GAMEINSTANCE->Draw_RenderGroup();

	m_pGameInstance->Render_Engine();

#if defined(USE_IMGUI)
	CImgui_Manager::Get_Instance()->Render();
#endif


//#ifdef _DEBUG
	++m_iNumRender;

	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumRender);
		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
	}
	// MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont
	//m_pGameInstance->Render_Font(TEXT("Font_Hancomdodum_Bold"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.4f);

	//wchar_t* szTemp;
	//wstring strSour; strSour.c_str()

	//m_pGameInstance->Render_Font(TEXT("Font_Hancomdodum_Bold"), TEXT("인벤토리"), _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 1.0f);

//#endif // _DEBUG

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (nullptr == m_pGameInstance)
		__debugbreak();

	CLevel_Loading* pLevel_Loading = CLevel_Loading::Create(eLevelID);
	if (nullptr == pLevel_Loading)
		__debugbreak();

	m_pGameInstance->Change_Level(LEVEL_LOADING, pLevel_Loading);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		__debugbreak();

	CState_Manager::Get_Instance()->Initialize();
	CEffect_Manager::Get_Instance()->Load_EffectsFile("EffectData");

	/* For.Prototype_Component_Renderer */
	//if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, RENDERER, CRenderer::Create())))
	//	__debugbreak();

	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(RENDERER), m_pRenderer = CRenderer::Create()));
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), CVIBuffer_Rect::Create()));
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_CUBE), CVIBuffer_Cube::Create()));
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT_INSTANCE), CVIBuffer_Rect_Instance::Create()));
	//FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_CLOUD_INSTANCE), CVIBuffer_Cloud_Instance::Create()));
#ifdef _TEST_HAN
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_TRAIL), CVIBuffer_Trail::Create()));
#endif
	//FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_POINT_INSTANCE), CVIBuffer_Point_Instance::Create(20)));
	//FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_ERROR), CTexture::Create(TEXT("../Bin/Resources/Textures/ErrorTexture.png"))));
	//FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_CURSOR), CTexture::Create(TEXT("../Bin/Resources/Textures/UI/Cursor/Cursor%d.png"), 5)));
	//FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_NOISE), CTexture::Create(TEXT("../Bin/Resources/Textures/NoiseTexture/NoiseTexture_%d.dds"), 436)));
	//FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), CTexture::Create(TEXT("../Bin/Resources/Textures/Effect/Noise/NoiseTexture%d.png"), 661)));
	//FAILEDCHECK(m_pGameInstance->Add_GameObject_Prototype(ID_UINT(CURSOR), CCursor::Create()));
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TRANSFORM), CTransform::Create()));
	//FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(COLLIDER), CPhyXCollider::Create()));
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(STATE_HANDLER), CState_Handler::Create()));

	// 쉐이더 vector로 만들어서 들고 있게 만들어

	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEX),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
		__debugbreak();

	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxTexUI.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements)));

	//if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXCOLOR),
	//	CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxColor.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements))))
	//	__debugbreak();

	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXNORTEX),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements)));

	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements)));

	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements)));

	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXCUBETEX),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements)));

	/* For.Shader_VtxPointInstance */
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXRECTINSTANCE),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxRectInstance.hlsl"), VTXTEX_INSTANCE_DECLARATION::Element, VTXTEX_INSTANCE_DECLARATION::iNumElements)));

	/* For.Shader_VtxAnimInstance */
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIMINSTANCE),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxAnimInstance.hlsl"), VTXANIM_INSTANCE_DECLARATION::Element, VTXANIM_INSTANCE_DECLARATION::iNumElements)));

	/* For.Shader_VtxModelInstance */
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODELINSTANCE),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxModelInstance.hlsl"), VTXMODEL_INSTANCE_DECLARATION::Element, VTXMODEL_INSTANCE_DECLARATION::iNumElements)));

	/* For.Shader_VtxPointInstance */
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXPOINTINSTANCE),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl"), VTXPOINT_INSTANCE_DECLARATION::Element, VTXPOINT_INSTANCE_DECLARATION::iNumElements)));
	
	///* For.Shader_VtxAnimInstance */
	//FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIMINSTANCE),
	//	CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxAnimInstance.hlsl"), VTXANIM_INSTANCE_DECLARATION::Element, VTXANIM_INSTANCE_DECLARATION::iNumElements)));

	///* For.Shader_VtxModelInstance */
	//FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODELINSTANCE),
	//	CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxModelInstance.hlsl"), VTXMODEL_INSTANCE_DECLARATION::Element, VTXMODEL_INSTANCE_DECLARATION::iNumElements)));

	/* For.Shader_VtxModelEffect */
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODELEFFECT),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxModelEffect.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements)));

	/* For.Shader_VtxModelEffect */
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXWATER),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxTexWater.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements)));

	//if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_DEFERRED),
	//	CShader::Create(TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
	//	__debugbreak();

	//if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_BLUR),
	//	CShader::Create(TEXT("../Bin/ShaderFiles/Shader_Blur.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
	//	__debugbreak();

	//if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_DECAL),
	//	CShader::Create(TEXT("../Bin/ShaderFiles/Shader_Decal.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
	//	__debugbreak();

	//if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_SHADOW),
	//	CShader::Create(TEXT("../Bin/ShaderFiles/Shader_Shadow.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
	//	__debugbreak();
	//
	//if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_LIGHT),
	//	CShader::Create(TEXT("../Bin/ShaderFiles/Shader_Light.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
	//	__debugbreak();

	//if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(SHADER, SHADER_OUTLINE),
	//	CShader::Create(TEXT("../Bin/ShaderFiles/Shader_Outline.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
	//	__debugbreak();

	/* For.Prototype_Component_Collider_AABB */
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_AABB), CCollider::Create(CCollider::TYPE_AABB)));
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_OBB), CCollider::Create(CCollider::TYPE_OBB)));
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), CCollider::Create(CCollider::TYPE_SPHERE)));

	///* For.Cursor*/
	//_uint	iTextureIdx = 0;
	//if (nullptr == (m_pGameInstance->Add_GameObject(LEVEL_STATIC, ID_UINT(CURSOR), ID_UINT(CURSOR), &iTextureIdx)))
	//	__debugbreak();



	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Camera()
{
	/* For.Camera */
	FAILEDCHECK(m_pGameInstance->Add_GameObject_Prototype(ID_UINT(CAMERA, CAMERA_FREE), CCamera_Free::Create()));
	FAILEDCHECK(m_pGameInstance->Add_GameObject_Prototype(ID_UINT(CAMERA, CAMERA_FIXED), CCamera_Fixed::Create()));
	FAILEDCHECK(m_pGameInstance->Add_GameObject_Prototype(ID_UINT(CAMERA, CAMERA_TARGET_MATRIX), CCamera_Target_Matrix::Create()));
	FAILEDCHECK(m_pGameInstance->Add_GameObject_Prototype(ID_UINT(CAMERA, CAMERA_TARGET_POSITION), CCamera_Target_Position::Create()));

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Loading()
{
	/* GameObject */
	FAILEDCHECK(m_pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_CURSOR), CUI_Cursor::Create()));
	FAILEDCHECK(m_pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_LOADING_SCREEN), CUI_Loading_Screen::Create()));
	FAILEDCHECK(m_pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_LOADING_BAR), CUI_Loading_Bar::Create()));
	FAILEDCHECK(m_pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_LOADING_ANIM), CUI_Loading_Anim::Create()));


#define UIPATH(path) towstring(strPath + path).c_str()
	string strPath = "../Bin/Resources/Textures/UI/";

	/* Texture */
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_CURSOR),
		CTexture::Create(UIPATH("Cursor/Cursor%d.dds"), 4)));
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_LOADING_SCREEN),
		CTexture::Create(UIPATH("Loading/Loading_Screen/Loading_Screen%d.png"), 22)));
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_LOADING_BAR),
		CTexture::Create(UIPATH("Loading/Loading_Bar/Loading_Bar%d.png"), 2)));
	FAILEDCHECK(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_LOADING_ANIM),
		CTexture::Create(UIPATH("Loading/Loading_Anim/Anim0/Loading%d.png"), 30)));

	return S_OK;
}

HRESULT CMainApp::Initialize_Engine()
{
	// Engine
	GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.isWindowMode = GRAPHICDESC::MODE_WIN;

	FAILEDCHECK(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc));


	// PipeLine
	VIEWPORTDESC	ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof(VIEWPORTDESC));
	ViewportDesc.Width = g_iWinCX;
	ViewportDesc.Height = g_iWinCY;
	m_pGameInstance->Set_Viewport(ViewportDesc);


	// Transform ID
	m_pGameInstance->Set_TransformID(ID_UINT(TRANSFORM));

	// Add Tick, LateTick Group
	FAILEDCHECK(m_pGameInstance->Reserve_TickContainer(TG_END));
	FAILEDCHECK(m_pGameInstance->Reserve_LateTickContainer(LTG_END));
	FAILEDCHECK(m_pGameInstance->Reserve_AnimContainer(ANIM_END));
	FAILEDCHECK(m_pGameInstance->Reserve_MoveCam());

	return S_OK;
}

void CMainApp::Add_MapModelPrototypeFromOneFolder(string Path, _int eModelID, _matrix TransformMatrix)
{
	stringvec vecPath, vecFile_fbx;
	read_directory(Path, vecPath);
	for (auto& File : vecPath)
	{
		if (!File.substr(File.length() - 3).compare("fbx") || !File.substr(File.length() - 3).compare("FBX"))
		{
			vecFile_fbx.push_back(File);
		}
		else if (!File.substr(File.length() - 5).compare("bMesh"))
		{
			if (!File.substr(File.length() - 5).compare("bMesh"))
			{
				File = File.substr(0, File.length() - 5) + "fbx";
			}
			vecFile_fbx.push_back(File);
		}
	}
	_uint iModelNum3 = 1;
	_uint iModelNum4 = 0;
	for (auto& FileName : vecFile_fbx)
	{
		FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, eModelID, iModelNum3++, iModelNum4), CModel::Create(TYPE_NONANIM, Path.c_str(), FileName.c_str(), TransformMatrix), FileName));
		if (255 < iModelNum3)
		{
			iModelNum3 = 1;
			iModelNum4++;
		}
	}
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MessageBox(g_hWnd, TEXT("Failed to Created : CMainApp"), TEXT("System Error"), MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pGameInstance);

	CSound_Manager::Get_Instance()->Destroy_Instance();
	CState_Manager::Get_Instance()->Destroy_Instance();
	CItem_Manager::Get_Instance()->Destroy_Instance();
	CQuest_Manager::Get_Instance()->Destroy_Instance();
	COrder_Manager::Get_Instance()->Destroy_Instance();
	CMinigame_Manager::Get_Instance()->Destroy_Instance();
#if defined(USE_IMGUI)
	CImgui_Manager::Get_Instance()->Destroy_Instance();
#endif
	CEffect_Manager::Get_Instance()->Destroy_Instance();
	CGameInstance::Release_Engine();
}
