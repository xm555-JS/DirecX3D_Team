#include "..\Public\Target_Manager.h"
#include "RenderTarget.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{
}

//ID3D11ShaderResourceView* CTarget_Manager::Get_SRV(const _tchar* pTargetTag)
//{
//	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);
//
//	if (nullptr == pRenderTarget)
//		return nullptr;
//
//	return pRenderTarget->Get_SRV();
//}

HRESULT CTarget_Manager::Initialize()
{
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;

	return S_OK;
}

CRenderTarget* CTarget_Manager::Create_RenderTarget(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	CRenderTarget*		pRenderTarget = CRenderTarget::Create(iWidth, iHeight, eFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return nullptr;

	return pRenderTarget;
}

HRESULT CTarget_Manager::Begin_MRT(list<CRenderTarget*>* pMRTList, ID3D11DepthStencilView* pDepthStencilView)
{
	//list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	//if (nullptr == pMRTList)
	//	__debugbreak();

	if (nullptr == pMRTList)
		__debugbreak();

	if (pMRTList->size() >= 8)
		__debugbreak();

	ID3D11ShaderResourceView* pSRVs[8] = { nullptr };

	m_pContext->PSSetShaderResources(0, 8, pSRVs);

	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	_uint		iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		RTVs[iNumRTVs++] = pRenderTarget->Get_RTV();
	}

	/* 기존에 바인딩되어있던(백버퍼 + 깊이스텐실버퍼)를 얻어온다. */
	m_pContext->OMGetRenderTargets(1, &m_pBackBufferView, &m_pDepthStencilView);

	/* 렌더타겟들을 장치에 바인딩한다. */
	if (nullptr == pDepthStencilView)
		m_pContext->OMSetRenderTargets(iNumRTVs, RTVs, m_pDepthStencilView);
	else
		m_pContext->OMSetRenderTargets(iNumRTVs, RTVs, pDepthStencilView);


	return S_OK;
}

HRESULT CTarget_Manager::BeginOverwrite_MRT(list<CRenderTarget*>* pMRTList, ID3D11DepthStencilView* pDepthStencilView)
{
	//list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	//if (nullptr == pMRTList)
	//	__debugbreak();

	if (nullptr == pMRTList)
		__debugbreak();

	if (pMRTList->size() >= 8)
		__debugbreak();

	ID3D11ShaderResourceView* pSRVs[8] = { nullptr };

	m_pContext->PSSetShaderResources(0, 8, pSRVs);

	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	_uint		iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		RTVs[iNumRTVs++] = pRenderTarget->Get_RTV();
	}

	/* 기존에 바인딩되어있던(백버퍼 + 깊이스텐실버퍼)를 얻어온다. */
	m_pContext->OMGetRenderTargets(1, &m_pBackBufferView, &m_pDepthStencilView);

	/* 렌더타겟들을 장치에 바인딩한다. */
	if (nullptr == pDepthStencilView)
		m_pContext->OMSetRenderTargets(iNumRTVs, RTVs, m_pDepthStencilView);
	else
		m_pContext->OMSetRenderTargets(iNumRTVs, RTVs, pDepthStencilView);


	return S_OK;
}

HRESULT CTarget_Manager::End_MRT()
{
	m_pContext->OMSetRenderTargets(1, &m_pBackBufferView, m_pDepthStencilView);

	Safe_Release(m_pBackBufferView);
	Safe_Release(m_pDepthStencilView);

	return S_OK;
}

void CTarget_Manager::Free()
{
}
