#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CLoading_Anim : public CBase
{
public:
	typedef enum tagAnimType
	{
		ANIM_DEFAULT,
		ANIM_EMOTICON,
		ANIM_INFINITYDOT,

		ANIM_END
	}ANIMTYPE;

protected:
	CLoading_Anim();
	virtual ~CLoading_Anim() = default;

public:
	_bool	Is_Finish() { return m_isFinished; }

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	void	Set_CenterPosition();


protected:
	_float		m_fXPosition = 0.f;
	_float		m_fYPosition = 0.f;
	_float		m_fScreenRate = 0.f;

protected:
	_bool		m_isFinished = false;

private:
	ANIMTYPE	m_eCurAnimNum = ANIM_END;

	CLoading_Anim*	m_pCurAnim = nullptr;

public:
	static CLoading_Anim* Create(void * pArg);
	virtual void Free() override;
};

END