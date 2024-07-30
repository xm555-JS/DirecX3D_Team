#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "imgui.h"

#if defined(USE_IMGUI)

BEGIN(Engine)
	class CGameObject;
END


BEGIN(Client)

class CWindow_Imgui abstract : public CBase
{
public:
	CWindow_Imgui();
	virtual ~CWindow_Imgui();

public:
	_bool		Is_Enable() { return m_bEnable; }

	void		Set_Disable() { m_bEnable = false; }
	void		Set_Enable() { m_bEnable = true; }

public:
	virtual	void	Tick(_float fTimeDelta)		PURE;
	virtual	void	Tick_Children(_float fTimeDelta);
	virtual void	Initialize()				PURE;
	virtual void	Initialize_Children();
	virtual void	Render() {};
	virtual void	On_EventMessege(EVENTDESC tEvent) {};

protected:
	void	HelpMarker(const char* desc);

protected:
	vector<CWindow_Imgui*>	m_pWindows;

protected:
	_bool			m_bEnable = false;

public:
	virtual	void	Free() PURE;
};

END

#endif