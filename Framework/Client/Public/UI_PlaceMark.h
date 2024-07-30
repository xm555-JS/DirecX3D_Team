#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_PlaceMark final : public CUI
{
public:
	enum DISTNUM { UNITS, TENS, HUNDREDS, THOUSANDS, METERS, DISTNUM_END };
public:
	CUI_PlaceMark();
	CUI_PlaceMark(const CUI_PlaceMark& rhs);
	virtual ~CUI_PlaceMark() = default;

public:
	void Set_Offset_TargetPosY(_float fOffset) { m_fOffset_TargetPosY = fOffset; }
	void Set_Able(_bool bAble) { m_bAble = bAble; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void On_Dead();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_bool	m_bExpansion = false;
	_float	m_fAccTime = 0.f;
	_float  m_fWaitTime = 0.f;

	_float4 m_vTargetPos = { 0.f,0.f,0.f,1.f };

	_bool	m_bRender = true;
	_bool   m_bAble = false;

	_float	m_fOffset_TargetPosY = 0.f;

private:
	class CUI_Dist_Num* m_pDistNum[DISTNUM_END];

private:
	void SetUp_Pos();
	void Emphasis_Mark(_float fTimeDelta);
	void SetUp_Mark_Effect(_float fTimeDelta);
	void SetUp_MarkArrow();

	void Create_Dist_Num();
	void SetUp_DistNum_Pos();

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_PlaceMark* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END