#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Sound_Manager.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI abstract : public CGameObject
{
public:
	typedef struct UIDesc
	{
		UIDesc() {}
		UIDesc(_float _fInitX, _float _fInitY, _float _fInitSizeX, _float _fInitSizeY, _float _fAngle, _uint _iTextureIdx, _uint _iRenderLayer
			, _float2 _vDeltaPos = { 0.f,0.f }, CGameObject* _pGameObject = nullptr, _uint _iIdx = 0
			, _float2 _vFontPos = { 0.f,0.f }, _float4 _vFontColor = { 1.f,1.f,1.f,1.f }, _float _fFontSize = 1.f, _bool _bRenderFontText = false, _bool _bRenderFontNum = false)
		{
			fInitX = _fInitX;
			fInitY = _fInitY;
			fInitSizeX = _fInitSizeX;
			fInitSizeY = _fInitSizeY;
			fAngle = _fAngle;
			iTextureIdx = _iTextureIdx;
			iRenderLayer = _iRenderLayer;                                                                                   
			vFontPos = _vFontPos;
			vFontColor = _vFontColor;
			fFontSize = _fFontSize;
			bRenderFontText = _bRenderFontText;
			bRenderFontNum = _bRenderFontNum;
			vDeltaPos = _vDeltaPos;
			pGameObject = _pGameObject;
			iIdx = _iIdx;
		}

		_float			fInitX = 0.f, fInitY = 0.f, fInitSizeX = 0.f, fInitSizeY = 0.f, fAngle = 0.f;
		_uint			iTextureIdx = 0 , iRenderLayer = 0;
		_float2			vFontPos = { 0.f,0.f };
		_float4			vFontColor = { 0.f,0.f,0.f,0.f };
		_float			fFontSize = 0.f;
		_bool			bRenderFontText = false;
		_bool			bRenderFontNum = false;
		_float2			vDeltaPos = { 0.f, 0.f };
		//CGameObject* pGameObject = nullptr;
		CGameObject*	pGameObject = nullptr;
		_uint			iIdx = 0;

	}UIDESC;

public:
	CUI();
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	UIDESC	Get_UIDesc() { return m_tUIDesc; }
	_float2 Get_vUIPos() { return _float2(m_fX, m_fY); }
	_float2 Get_vUISize() { return _float2(m_fSizeX, m_fSizeY); }
	_uint	Get_iMaxTextureIdx() { return m_iMaxTextureIdx; }
	_bool	Get_bMoveWithFont() { return m_bMoveWithFont; }
	string  Get_FontText() { return m_strFontText; }
	wstring Get_Default_FontText() { return m_strDefault_FontText; }


	void Set_vUIPos(_float2 vUIPos) { m_fX = vUIPos.x; m_fY = vUIPos.y; }
	void Set_vUISize(_float2 vUISize) { m_fSizeX = vUISize.x; m_fSizeY = vUISize.y; }
	void Set_fUIAngle(_float fUIAngle) { m_tUIDesc.fAngle = fUIAngle; }
	void Set_iTextureIdx(_uint iTextureIdx) { m_tUIDesc.iTextureIdx = iTextureIdx; }
	void Set_FontText(string strText) {	m_strFontText = strText; }
	void Set_FontNum(_int iFontNum) { m_iFontNum = iFontNum; }
	void Set_FontPos(_float2 vFontPos) { m_tUIDesc.vFontPos = vFontPos; }
	void Set_FontSize(_float fScale) { m_tUIDesc.fFontSize = fScale; }
	void Set_FontColor(_float4 vColor) { m_tUIDesc.vFontColor = vColor; }
	void Set_bMoveWithFont(_bool bMoveWithFont) { m_bMoveWithFont = bMoveWithFont; }
	void Set_bRenderFontText(_bool bRenderFontText) { m_tUIDesc.bRenderFontText = bRenderFontText; }
	void Set_bRenderFontNum(_bool bRenderFontNum) { m_tUIDesc.bRenderFontNum = bRenderFontNum; }
	void Set_Delfault_FontText(wstring strText) { m_strDefault_FontText = strText; }
	void Set_GameObject(CGameObject* pGameObject) { m_tUIDesc.pGameObject = pGameObject; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta) {};
	virtual HRESULT Render();

protected:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	//CRenderer*				m_pRendererCom = nullptr;		
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

protected:
	UIDESC			m_tUIDesc;
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;

	_uint			m_iMaxTextureIdx = 0;
	_float4x4		m_ProjMatrix;

protected:
	_bool			m_bMoveWithFont = true;
	_float2			m_vFontDelta = { 0.f, 0.f };
	string			m_strFontText = "";
	wstring			m_strDefault_FontText = L"";
	_int			m_iFontNum = 0;

protected:
	_float			m_fFrameTime = 0.f;
	_uint			m_iSoundChannel = CSound_Manager::CH_END;

protected:
	DECL_ONCE(m_bSound0);
	DECL_ONCE(m_bSound1);
	DECL_ONCE(m_bSound2);
	DECL_ONCE(m_bSound3);
	DECL_ONCE(m_bSound4);
	DECL_ONCE(m_bSound5);

protected:
	void	MoveWithFont();
	void	Move_Frame(_float fFrameTime, _float fTimeDelta);
	_float2	SetUp_ViewportPos(CGameObject* pGameObject);
	_float2	SetUp_ViewportPos(_float4 vPos);
	void	Load_UIData(string strFileName);

protected:
	void	SetUp_UIpos();
	virtual HRESULT SetUp_Components() PURE;
	virtual HRESULT SetUp_ShaderResource();

public:
	virtual CGameObject* Clone(void* pArg) override { return nullptr; };
	virtual void Free() override;
};

END