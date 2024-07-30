#include "stdafx.h"
#include "..\Public\Terrain.h"

#include "GameInstance.h"

#include "Light.h"

#include "Imgui_Manager.h"

CTerrain::CTerrain()
	: CGameObject()
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void * pArg)
{
	USEGAMEINSTANCE;
	m_pContext = CONTEXT;
	m_pDevice = DEVICE;

	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());
	//FAILEDCHECK(Create_FilterTexture());

	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);

	return S_OK;
}

void CTerrain::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//if (KEY_DOWN('H'))
	//{
	//	_vector		vPickedPos {0.f, 0.f, 0.f, 0.f};
	//	pGameInstance->Picking(m_pVIBufferCom, m_pTransformCom, &vPickedPos);
	//	//pGameInstance->Picking(0.f, &vPickedPos);
	//	cout << XMVectorGetX(vPickedPos) << " " << XMVectorGetY(vPickedPos) << " " << XMVectorGetZ(vPickedPos) << endl;
	//}

	//if (KEY_DOWN('H'))
	//	cout << ++m_iDiffuseIdx << endl;

	//if (KEY_DOWN('J'))
	//	cout << --m_iDiffuseIdx << endl;
}

void CTerrain::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_TERRAIN, this);
}

HRESULT CTerrain::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

#if defined(_TESSELLATION)
	m_pShaderCom->Begin(2);
#else
	m_pShaderCom->Begin(0);
#endif

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTerrain::Render(void* pArg)
{
	RENDERDESC	Desc;
	memcpy(&Desc, pArg, sizeof(RENDERDESC));
	

	if (RENDERTYPE_DEPTH == Desc.eType)
	{
		FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));

		CLight* pTargetLight = (CLight*)Desc.lParam;
		pTargetLight->SetUp_DepthComponents(m_pShaderCom);

		m_pShaderCom->Begin(3);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CTerrain::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXNORTEX), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_TERRAIN, TYPE_DIFFUSE), ID_UINT(TEXTURE, TYPE_DIFFUSE), (CComponent**)&m_pTextureCom[TYPE_DIFFUSE]));
	FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_TERRAIN, TYPE_FILTER), ID_UINT(TEXTURE, TYPE_FILTER), (CComponent**)&m_pTextureCom[TYPE_FILTER]));
	//FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_TERRAIN), ID_UINT(TEXTURE, TYPE_FILTER), (CComponent**)&m_pTextureCom[TYPE_FILTER]));
	//FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_TERRAIN), ID_UINT(TEXTURE, TYPE_BRUSH), (CComponent**)&m_pTextureCom[TYPE_BRUSH]));

//#if defined(_TESSELLATION)
//	FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(VIBUFFER, BUFFER_TERRAIN_TESS_CUSTOM), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));
//#else
	FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(VIBUFFER, BUFFER_TERRAIN), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));
//#endif

	return S_OK;
}

HRESULT CTerrain::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float)));

	//_float fScale = DEBUGFLOAT;
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fScale", &fScale, sizeof(_float)));

	

	//FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_SourDiffTexture", 0));
	//FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_DestDiffTexture", 0));
	FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_RDiffTexture", 6));
	FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_GDiffTexture", 0));
	FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_BDiffTexture", 3));
	FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_BaseDiffTexture", 2));
	//FAILEDCHECK(m_pTextureCom[TYPE_BRUSH]->Set_ShaderResourceView(m_pShaderCom, "g_BrushTexture", 0));
	FAILEDCHECK(m_pTextureCom[TYPE_FILTER]->Set_ShaderResourceView(m_pShaderCom, "g_FilterTexture", 0));
	FAILEDCHECK(m_pTextureCom[TYPE_FILTER]->Set_ShaderResourceView(m_pShaderCom, "g_FilterTexture_2", 1));


	
	FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_RDiffTexture_2", 8));
	FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_GDiffTexture_2", 7));



	//_float2		vTest = { DEBUGFLOAT4.x, DEBUGFLOAT4.y };
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vOffset", &vTest, sizeof(_float2)));
	

	return S_OK;
}

HRESULT CTerrain::Create_FilterTexture()
{
	ID3D11Texture2D*			pTexture2D = nullptr;


	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 256;
	TextureDesc.Height = 256;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pTexture2D)))
		__debugbreak();

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	for (_uint i = 0; i < 256; ++i)
	{
		for (_uint j = 0; j < 256; ++j)
		{
			_uint iIndex = i * 256 + j;

			if (j < 128)
				((_uint*)SubResource.pData)[iIndex] = D3DCOLOR_ABGR(255, 255, 255, 255);
			else
				((_uint*)SubResource.pData)[iIndex] = D3DCOLOR_ABGR(255, 0, 0, 0);
		}
	}


	m_pContext->Unmap(pTexture2D, 0);
	FAILEDCHECK(SaveDDSTextureToFile(m_pContext.Get(), pTexture2D, TEXT("../Bin/Test.dds")));
	FAILEDCHECK(m_pDevice->CreateShaderResourceView(pTexture2D, nullptr, &m_pFilterTexture));

	Safe_Release(pTexture2D);

	return S_OK;
}

CTerrain * CTerrain::Create()
{
	CTerrain*		pInstance = new CTerrain();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain*		pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

}
