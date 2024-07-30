#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Fixed final : public CCamera
{
public:
	CCamera_Fixed();
	CCamera_Fixed(const CCamera_Fixed& rhs);
	virtual ~CCamera_Fixed() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick_Camera(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float			m_fMoveSpeed = 0.f;
	_float			m_fRotateSpeed = 0.f;

public:
	static CCamera_Fixed* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END