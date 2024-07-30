#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CVIBuffer;
class CTexture;
class CNavigation;
END

BEGIN(Client)
class CState_Handler;

class CUnit abstract : public CGameObject
{
public:
	CUnit();
	CUnit(const CUnit& rhs);
	virtual ~CUnit() = default;

public:
	UNITDESC*		Get_UnitInfo(){ return m_pInfo; }
	UNITDESC*		Get_UnitCurInfo() { return m_pCurInfo; }
	CGameObject*	Get_Target() { return m_pTarget; }
	void			Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	virtual void	Damaged(ELEMENTAL_PROPERTIE eElement, _float fPower) PURE;

protected:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CState_Handler*		m_pState_Handler = nullptr;
	CNavigation*		m_pNaviCom = nullptr;

protected:
	_uint				m_iNumMeshContainers = 0;

protected:
	UNITDESC*			m_pInfo = nullptr;		// 고정값
	UNITDESC*			m_pCurInfo = nullptr;	// 변동값
	_float				m_fPreHp = 0.f;

protected:
	CGameObject*		m_pTarget = nullptr;

protected:
	_float					m_fStopTime = 0.f;

//protected:
//	void	Create_UI_Place_Mark(_float fOffsetY = 0.f);

public:
	virtual void Free() override;
};

END