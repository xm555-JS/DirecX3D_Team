#include "stdafx.h"
#include "..\Public\Terrain_Navi.h"

#include "GameInstance.h"
#include "Imgui_Manager.h"

CTerrain_Navi::CTerrain_Navi()
	: CGameObject()
{
}

CTerrain_Navi::CTerrain_Navi(const CTerrain_Navi & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain_Navi::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain_Navi::Initialize(void * pArg)
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

void CTerrain_Navi::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//if (KEY_DOWN('H'))
	//{
	//	_vector		vPickedPos {0.f, 0.f, 0.f, 0.f};
	//	pGameInstance->Picking(m_pVIBufferCom, m_pTransformCom, &vPickedPos);
	//	//pGameInstance->Picking(0.f, &vPickedPos);
	//	cout << XMVectorGetX(vPickedPos) << " " << XMVectorGetY(vPickedPos) << " " << XMVectorGetZ(vPickedPos) << endl;
	//}
}

void CTerrain_Navi::LateTick(_float fTimeDelta)
{
	//GAMEINSTANCE->Add_RenderGroup(RENDER_TERRAIN, this);
}

HRESULT CTerrain_Navi::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTerrain_Navi::SetUp_Components()
{
	//FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXNORTEX), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	//FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_TERRAIN, TYPE_DIFFUSE), ID_UINT(TEXTURE, TYPE_DIFFUSE), (CComponent**)&m_pTextureCom[TYPE_DIFFUSE]));
	//FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_TERRAIN, TYPE_FILTER), ID_UINT(TEXTURE, TYPE_FILTER), (CComponent**)&m_pTextureCom[TYPE_FILTER]));
	//FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_TERRAIN), ID_UINT(TEXTURE, TYPE_FILTER), (CComponent**)&m_pTextureCom[TYPE_FILTER]));
	//FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_TERRAIN), ID_UINT(TEXTURE, TYPE_BRUSH), (CComponent**)&m_pTextureCom[TYPE_BRUSH]));
	FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(VIBUFFER, BUFFER_NAVI), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CTerrain_Navi::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	//FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_SourDiffTexture", 0));
	//FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_DestDiffTexture", 0));
	FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_RDiffTexture", 6));
	FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_GDiffTexture", 0));
	FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_BDiffTexture", 3));
	FAILEDCHECK(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_BaseDiffTexture", 2));
	//FAILEDCHECK(m_pTextureCom[TYPE_BRUSH]->Set_ShaderResourceView(m_pShaderCom, "g_BrushTexture", 0));
	FAILEDCHECK(m_pTextureCom[TYPE_FILTER]->Set_ShaderResourceView(m_pShaderCom, "g_FilterTexture", 0));
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_FilterTexture", m_pFilterTexture)))
	//	__debugbreak();

	//_float2		vTest = { DEBUGFLOAT4.x, DEBUGFLOAT4.y };
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vOffset", &vTest, sizeof(_float2)));
	

	return S_OK;
}

HRESULT CTerrain_Navi::Create_FilterTexture()
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

CTerrain_Navi * CTerrain_Navi::Create()
{
	CTerrain_Navi*		pInstance = new CTerrain_Navi();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain_Navi");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CTerrain_Navi::Clone(void * pArg)
{
	CTerrain_Navi*		pInstance = new CTerrain_Navi(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain_Navi");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain_Navi::Free()
{
	__super::Free();

}
