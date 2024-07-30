#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Solid abstract : public CGameObject
{
public:
	typedef struct UISolidDesc
	{
		UISolidDesc() {}
		UISolidDesc(CGameObject* _pGamaObject, _uint _iTextureIdx, _float2 _vDelta = { 0.f, 0.f })
		{
			pGameObject = _pGamaObject;
			iTextureIdx = _iTextureIdx;
			vDelta = _vDelta;
		}
		
		CGameObject* pGameObject = nullptr;
		_uint		 iTextureIdx = 0;
		_float2		 vDelta = {0.f, 0.f};

	}UISOLIDDESC;

public:
	_bool Get_Render() { return m_bRender; }

public:
	CUI_Solid();
	CUI_Solid(const CUI_Solid& rhs);
	virtual ~CUI_Solid() = default;

public:
	CGameObject* Get_Object() { return m_tUISolidDesc.pGameObject; }
	_float3		 Get_CurScale() { return m_vCurScale; }
	_float3		 Get_MinScale() { return m_vMinScale; }
	_float3		 Get_MaxScale() { return m_vMaxScale; }
	_bool		 Get_bRender() { return m_bRender; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta) {};
	virtual HRESULT Render();

protected:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;		
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

protected:
	UISOLIDDESC				m_tUISolidDesc;
	_bool					m_bRender = true;
	_float3					m_vCurScale;
	_float3					m_vMinScale;
	_float3					m_vMaxScale;

protected:
	void	Billboard();

protected:
	virtual HRESULT SetUp_Components() PURE;
	virtual HRESULT SetUp_ShaderResource();

public:
	virtual CGameObject* Clone(void* pArg) override { return nullptr; };
	virtual void Free() override;
};

END