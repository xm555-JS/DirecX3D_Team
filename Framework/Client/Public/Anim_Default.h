#pragma once
#include "Client_Defines.h"
#include "Loading_Anim.h"

BEGIN(Client)

class CAnim_Default final : public CLoading_Anim
{
private:
	CAnim_Default();
	virtual ~CAnim_Default() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_uint		m_iDotCount = 0;
	_uint		m_iMaxDotCount = 0;
	_float		m_fTimeAcc = 0.f;
	_float		m_fMotionChangeTime = 0.f;

	_float		m_fMotionChangeAcc = 0.f;
	_float		m_fPlayTime = 0.f;


public:
	static CLoading_Anim* Create(void * pArg);
	virtual void Free() override;
};

END