#include "..\Public\CustomFont.h"
#include "GameInstance.h"

CCustomFont::CCustomFont()
{
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;
}

HRESULT CCustomFont::Initialize(const _tchar * pFontFilePath)
{
	m_pBatch = new SpriteBatch(m_pContext.Get());
	m_pFont = new SpriteFont(m_pDevice.Get(), pFontFilePath);

	return S_OK;
}

HRESULT CCustomFont::Render(const _tchar* pString, const _float2& vPosition, _fvector vColor, _float fSize)
{
	if (nullptr == m_pBatch ||
		nullptr == m_pFont)
		__debugbreak();

	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch, pString, vPosition, vColor, 0.f, XMFLOAT2(0.f, 0.f), fSize);

	m_pBatch->End();

	return S_OK;
}

CCustomFont * CCustomFont::Create(const _tchar * pFontFilePath)
{
	CCustomFont*		pInstance = new CCustomFont();

	if (FAILED(pInstance->Initialize(pFontFilePath)))
	{
		MSG_BOX("Failed to Created : CCustomFont");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CCustomFont::Free()
{
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);
}
