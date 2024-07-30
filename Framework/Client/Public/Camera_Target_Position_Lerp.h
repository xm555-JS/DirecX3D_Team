#pragma once

#include "Client_Defines.h"
#include "Camera_Target_Position.h"

BEGIN(Engine)
	class CNavigation;
	class CVIBuffer_Terrain;
END

BEGIN(Client)

class CCamera_Target_Position_Lerp final : public CCamera_Target_Position
{
public:
	CCamera_Target_Position_Lerp();
	CCamera_Target_Position_Lerp(const CCamera_Target_Position_Lerp& rhs);
	virtual ~CCamera_Target_Position_Lerp() = default;

public:
	CTransform*		Get_NextTransform() { return m_pNextTransfrom; }

	virtual void	Set_TargetObject(CGameObject* pTarget);
	void			Set_PosLerpRate(_float fLerpRate);
	void			Set_LookLerpRate(_float fLerpRate);

public:
	void			Set_NextOffset(_vector vNextOffset, _float fPower);


public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick_Camera(_float fTimeDelta) override;


private:
	CTransform*			m_pNextTransfrom = nullptr;
	CNavigation*		m_pNaviCom = nullptr;
	CVIBuffer_Terrain*	m_pTerrainBuffer = nullptr;



	_float				m_fOffsetMovePower = 1.f;


private:
	_float4				m_vPos = { 0.f, 0.f, 0.f, 1.f };
	_float4				m_vLook = { 0.f, 0.f, 0.f, 0.f };
	_float				m_fPosLerpRate = 1.f;
	_float				m_fLookLerpRate = 1.f;
	
	_float				m_fChaseTimeAcc = 0.f;


private:
	HRESULT SetUp_Components();
	
	_vector Calculate_TerrainPos(_vector vTargetPos, _vector vCamPos);

public:
	static CCamera_Target_Position_Lerp* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END