#include "..\Public\Shader.h"
#include "GameInstance.h"

CShader::CShader()
	: CComponent()
{
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;
}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_Passes(rhs.m_Passes)
{
	Safe_AddRef(m_pEffect);

	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;

	for (auto& PassDesc : m_Passes)
	{
		Safe_AddRef(PassDesc.pInputLayout);
	}

}

HRESULT CShader::Initialize_Prototype(const _tchar * pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement)
{
	_uint		iHLSLFlag = 0;

#ifdef _DEBUG
	iHLSLFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_OPTIMIZATION_LEVEL0;
#else
	iHLSLFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHLSLFlag, 0, m_pDevice.Get(), &m_pEffect, /*&pError*/nullptr)))
		__debugbreak();	

	ID3DX11EffectTechnique*		pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		__debugbreak();

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));
	
	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		__debugbreak();

	_uint		iNumPasses = TechniqueDesc.Passes;

	m_Passes.reserve(iNumPasses);

	for (_uint i = 0; i < iNumPasses; ++i)
	{
		PASSDESC			PassDesc;
		ZeroMemory(&PassDesc, sizeof(PASSDESC));

		PassDesc.pPass = pTechnique->GetPassByIndex(i);
		if (nullptr == PassDesc.pPass)
			__debugbreak();

		D3DX11_PASS_DESC		Pass;
		ZeroMemory(&Pass, sizeof(D3DX11_PASS_DESC));
		
		PassDesc.pPass->GetDesc(&Pass);		

		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElement, Pass.pIAInputSignature, Pass.IAInputSignatureSize, &PassDesc.pInputLayout)))
			__debugbreak();

		m_Passes.push_back(PassDesc);
	}

	return S_OK;
}

HRESULT CShader::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CShader::Set_RawValue(const char * pConstantName, const void * pData, _uint iDataSize)
{
	if (nullptr == m_pEffect)
		__debugbreak();

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		__debugbreak();

	return pVariable->SetRawValue(pData, 0, iDataSize);	

}

HRESULT CShader::Set_ShaderResourceView(const char * pConstantName, ID3D11ShaderResourceView * pSRV)
{
	if (nullptr == m_pEffect)
		__debugbreak();

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		__debugbreak();

	ID3DX11EffectShaderResourceVariable*	pVariable_SRV = pVariable->AsShaderResource();

	return pVariable_SRV->SetResource(pSRV);
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_Passes.size())
 		__debugbreak();

	m_pContext->IASetInputLayout(m_Passes[iPassIndex].pInputLayout);

	if (FAILED(m_Passes[iPassIndex].pPass->Apply(0, m_pContext.Get())))
		__debugbreak();

	return S_OK;
}

CShader * CShader::Create(const _tchar * pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement)
{
	CShader*		pInstance = new CShader();

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElement)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CShader::Clone(void * pArg)
{
	CShader*		pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	for (auto& PassDesc : m_Passes)
	{
		Safe_Release(PassDesc.pInputLayout);

		/*if (false == m_isCloned)
			Safe_Release(PassDesc.pPass);*/
	}

	Safe_Release(m_pEffect);

}
