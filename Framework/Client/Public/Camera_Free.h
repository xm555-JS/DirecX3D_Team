#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Free final : public CCamera
{
public:
	CCamera_Free();
	CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick_Camera(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float			m_fMoveSpeed = 0.f;
	_float			m_fRotateSpeed = 0.f;
	_bool			m_bIsMouseCanMove = true;

public:
	static CCamera_Free* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END