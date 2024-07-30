#include "..\Public\Texture.h"
#include "Shader.h"
#include "GameInstance.h"

CTexture::CTexture()
	: CComponent()
{
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_SRVs(rhs.m_SRVs)
{
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;

	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

HRESULT CTexture::Set_ShaderResourceView(CShader * pShader, const char* pConstantName, _uint iIndex)
{
	if (0 == m_SRVs.size())
		return S_OK;

	if (iIndex >= m_SRVs.size())
		__debugbreak();

	return pShader->Set_ShaderResourceView(pConstantName, m_SRVs[iIndex]);
}

HRESULT CTexture::Initialize_Prototype(string& strPath)
{
	stringvec m_vecTextureFiles;
	StringvecClear(m_vecTextureFiles);
	read_directory(strPath, m_vecTextureFiles);

	m_iType = TYPE_TEXTURE;

	m_SRVs.reserve(m_vecTextureFiles.size());

	for (auto& File : m_vecTextureFiles)
	{
		ID3D11ShaderResourceView*			pSRV = nullptr;
		
		HRESULT		hr = 0;

		if (!File.substr(File.length() - strlen(".dds")).compare(".dds"))
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice.Get(), towstring(strPath + File).c_str(), nullptr, &pSRV);
		
		else if (!File.substr(File.length() - strlen(".png")).compare(".png"))
			hr = DirectX::CreateWICTextureFromFile(m_pDevice.Get(), towstring(strPath + File).c_str(), nullptr, &pSRV);

		if (FAILED(hr))
			__debugbreak();

		m_SRVs.push_back(pSRV);
		
	}

	return S_OK;
}

HRESULT CTexture::Initialize_Prototype(const _tchar * pTextureFilePath, _uint iNumTextures)
{
	_tchar		szTextureFilePath[MAX_PATH] = TEXT("");

	m_iType = TYPE_TEXTURE;

	wstring	strTextureFilePath = pTextureFilePath;
	sprintf_s(m_pTextureFilePath, string(strTextureFilePath.begin(), strTextureFilePath.end()).c_str());
	//m_pTextureFilePath = string(strTextureFilePath.begin, strTextureFilePath.end).c_str();
	m_iNumTextures = iNumTextures;

	m_SRVs.reserve(iNumTextures);

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		ID3D11ShaderResourceView*			pSRV = nullptr;

		wsprintf(szTextureFilePath, pTextureFilePath, i);

		_tchar			szExt[MAX_PATH] = TEXT("");

		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		HRESULT		hr = 0;


		if (!lstrcmp(szExt, TEXT("")))
		{
			continue;
		}

		if (!lstrcmp(szExt, TEXT(".dds")))		
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice.Get(), szTextureFilePath, nullptr, &pSRV);
		
		else		
			hr = DirectX::CreateWICTextureFromFile(m_pDevice.Get(), szTextureFilePath, nullptr, &pSRV);

		if (FAILED(hr))
			__debugbreak();

		m_SRVs.push_back(pSRV);
		
	}

	return S_OK;
}

HRESULT CTexture::Initialize_Prototype(_float4 vColor, _uint iNumTextures)
{
	m_iType = TYPE_COLOR;
	m_vColor = vColor;
	m_iNumTextures = iNumTextures;
	//ComPtr<ID3D11ShaderResourceView> texSRV;
	//
	//	static const uint32_t s_pixel = 0xffc99aff;

	//	D3D11_SUBRESOURCE_DATA initData = { &s_pixel, sizeof(uint32_t), 0 };

	//	D3D11_TEXTURE2D_DESC desc = {};
	//	desc.Width = desc.Height = desc.MipLevels = desc.ArraySize = 1;
	//	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//	desc.SampleDesc.Count = 1;
	//	desc.Usage = D3D11_USAGE_IMMUTABLE;
	//	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	//	ComPtr<ID3D11Texture2D> tex;
	//	HRESULT hr = m_pDevice->CreateTexture2D(&desc, &initData, tex.GetAddressOf());

	//	if (SUCCEEDED(hr))
	//	{
	//		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	//		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//		SRVDesc.Texture2D.MipLevels = 1;

	//		hr = m_pDevice->CreateShaderResourceView(tex.Get(),
	//			&SRVDesc, texSRV.GetAddressOf());
	//	}

	//	if (FAILED(hr))
	//		int a = 0;
	//		// error!
	//


	m_SRVs.reserve(iNumTextures);

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		ID3D11ShaderResourceView*			pSRV = nullptr;
		ID3D11Texture2D*					pTexture = nullptr;
		D3D11_TEXTURE2D_DESC				TextureDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC		SRVDesc = {};

		//_float4			vTempColor = {0.f, 0.f, 1.f, 1.f};

		m_iColor = ConvertColor(vColor);

		//static const uint32_t s_pixel = 0xffc99aff;

		D3D11_SUBRESOURCE_DATA SubresourceData = { &m_iColor, sizeof(_uint), 0 };


		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		
		TextureDesc.Width = 1;
		TextureDesc.Height = 1;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;

		TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags = 0;
		TextureDesc.MiscFlags = 0;


		if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubresourceData, &pTexture)))
			__debugbreak();


		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		if (FAILED(m_pDevice->CreateShaderResourceView(pTexture, &SRVDesc, &pSRV)))
			__debugbreak();

		Safe_Release(pTexture);


		//m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D);
		//m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &pSRV);

		m_SRVs.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void * pArg)
{

	return S_OK;
}

CTexture* CTexture::Make_ColorTexture()
{
	return nullptr;
}

CTexture * CTexture::Create(string& strPath)
{
	CTexture*		pInstance = new CTexture();

	if (FAILED(pInstance->Initialize_Prototype(strPath)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTexture * CTexture::Create(const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CTexture*		pInstance = new CTexture();

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTexture * CTexture::Create(_float4 vColor, _uint iNumTextures)
{
	CTexture*		pInstance = new CTexture();

	if (FAILED(pInstance->Initialize_Prototype(vColor, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTexture * CTexture::Create(_uint iSavetype, HANDLE hFile)
{
	DWORD			dwByte = 0;
	CTexture*		pInstance = new CTexture();

	if (TYPE_TEXTURE == iSavetype)
	{

		DWORD iPathLength;
		ReadFile(hFile, &iPathLength, sizeof(DWORD), &dwByte, nullptr);
		
		
		//wstring	strTextureFilePath;
		//strTextureFilePath.resize(PathLength);
		//m_pTextureFilePath
		char			szFullPath[MAX_PATH] = "";
		ReadFile(hFile, &szFullPath, sizeof(char) * iPathLength, &dwByte, nullptr);
		string strFullPath = szFullPath;
		wstring wstrFullPath = wstring(strFullPath.begin(), strFullPath.end());

		if (FAILED(pInstance->Initialize_Prototype(wstrFullPath.c_str(), 1)))
		{
			MSG_BOX("Failed to Created : CTexture");
			Safe_Release(pInstance);
		}
	}
	else if (TYPE_COLOR == iSavetype)
	{

		_float4 vColor;
		ReadFile(hFile, &vColor, sizeof(_float4), &dwByte, nullptr);

		if (FAILED(pInstance->Initialize_Prototype(vColor, 1)))
		{
			MSG_BOX("Failed to Created : CTexture");
			Safe_Release(pInstance);
		}
	}

	return pInstance;
}

CComponent* CTexture::Clone(void * pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_SRVs)
		Safe_Release(pSRV);

	m_SRVs.clear();

	Safe_Release(m_pTexture2D);
}

