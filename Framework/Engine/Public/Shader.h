#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
public:
	typedef struct tagPasses
	{
		ID3DX11EffectPass*			pPass = nullptr;		
		ID3D11InputLayout*			pInputLayout = nullptr;
	}PASSDESC;

	

private:
	CShader();
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Set_RawValue(const char* pConstantName, const void* pData, _uint iDataSize);
	HRESULT Set_ShaderResourceView(const char* pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT Begin(_uint iPassIndex);

private:
	ID3DX11Effect*					m_pEffect = nullptr;

	vector<PASSDESC>				m_Passes;
	typedef vector<PASSDESC>		PASSES;


private:
	ComPtr<ID3D11Device>				m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>		m_pContext = nullptr;


public:
	static CShader* Create(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END