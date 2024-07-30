#include "..\Public\Font_Manager.h"
#include "CustomFont.h"

IMPLEMENT_SINGLETON(CFont_Manager)

CFont_Manager::CFont_Manager()
{
}

HRESULT CFont_Manager::Add_Font(const _tchar* pFontTag, const _tchar * pFontFilePath)
{
	if (nullptr != Find_Font(pFontTag))
		__debugbreak();

	CCustomFont*		pFont = CCustomFont::Create(pFontFilePath);
	if (nullptr == pFont)
		__debugbreak();

	m_Fonts.emplace(pFontTag, pFont);

	return S_OK;
}

HRESULT CFont_Manager::Render_Font(const _tchar * pFontTag, const _tchar * pString, const _float2 & vPosition, _fvector vColor, _float fSize)
{
	CCustomFont*		pFont = Find_Font(pFontTag);
	if (nullptr == pFont)
		__debugbreak();

	return pFont->Render(pString, vPosition, vColor, fSize);	
}

CCustomFont * CFont_Manager::Find_Font(const _tchar * pFontTag)
{
	auto	iter = find_if(m_Fonts.begin(), m_Fonts.end(), CTag_Finder(pFontTag));

	if (iter == m_Fonts.end())
		return nullptr;

	return iter->second;	
}

void CFont_Manager::Free()
{
	for (auto& Pair : m_Fonts)
		Safe_Release(Pair.second);

	m_Fonts.clear();
}
