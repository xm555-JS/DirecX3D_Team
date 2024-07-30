#pragma once

#include "Base.h"

BEGIN(Engine)

class CCustomFont final : public CBase
{
public:
	//typedef struct FontDesc
	//{
	//	//FontDesc(const _tchar* _pString, _float2 _vPosition, _float4 _vColor, _float _fSize)
	//	//{
	//	//	pString = _pString;
	//	//	vPosition = _vPosition;
	//	//	vColor = _vColor;
	//	//	fSize = _fSize;
	//	//}
	//	//const _tchar* pString;
	//	wstring pString;
	//	_float2 vPosition;
	//	_float4 vColor;
	//	_float fSize;
	//}FONTDESC;
	
private:
	CCustomFont();
	virtual ~CCustomFont() = default;

public:
	HRESULT Initialize(const _tchar* pFontFilePath);
	HRESULT Render(const _tchar* pString, const _float2& vPosition, _fvector vColor, _float fSize);

private:
	ComPtr<ID3D11Device>				m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>			m_pContext = nullptr;

private:
	SpriteBatch*				m_pBatch = nullptr;
	SpriteFont*					m_pFont = nullptr;

public:
	static CCustomFont* Create(const _tchar* pFontFilePath);
	virtual void Free() override;
};

END