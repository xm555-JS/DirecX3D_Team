#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	typedef enum tagSaveDesc
	{
		TYPE_TEXTURE,
		TYPE_COLOR,

		TYPE_END
	}SAVEDESC;	

private:
	CTexture();
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	HRESULT Set_ShaderResourceView(class CShader* pShader, const char* pConstantName, _uint iIndex = 0);
	vector<ID3D11ShaderResourceView*> Get_SRV(void) { return m_SRVs; }

	_uint		Get_TextureType() { return m_iType; }
	char*		Get_TextureFilePath() { return m_pTextureFilePath; }
	_float4		Get_TextureColor() { return m_vColor; }
	_uint		Get_NumTexture() { return m_iNumTextures; }

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize_Prototype(_float4	vColor, _uint iNumTextures);
	virtual HRESULT Initialize_Prototype(string& strPath);
	virtual HRESULT Initialize(void* pArg) override;

public:
	CTexture*	Make_ColorTexture();


private:
	vector<ID3D11ShaderResourceView*>			m_SRVs;
	typedef vector<ID3D11ShaderResourceView*>	SRVS;
	ID3D11Texture2D*							m_pTexture2D = nullptr;
	_uint										m_iColor = 0;

	_uint										m_iType = TYPE_END;
	char										m_pTextureFilePath[MAX_PATH];
	_uint										m_iNumTextures = 0;
	_float4										m_vColor;

private:
	ComPtr<ID3D11Device>				m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>			m_pContext = nullptr;

public:
	static CTexture* Create(const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	static CTexture* Create(_float4	vColor, _uint iNumTextures = 1);
	static CTexture* Create(_uint iSavetype, HANDLE hFile);
	static CTexture* Create(string& strPath);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END