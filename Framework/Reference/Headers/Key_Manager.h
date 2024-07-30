#pragma once
#include "Base.h"

BEGIN(Engine)

enum ST_KEY
{
	KEY_NONE,
	KEY_DOWN,
	KEY_PRESS,
	KEY_UP
};

//enum ST_KEY_TOGGLE
//{
//	KEY_TOGGLE_DOWN,
//	KEY_TOGGLE_UP,
//};

class CKey_Manager final : public CBase
{
	DECLARE_SINGLETON(CKey_Manager)

public:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	HRESULT		Initialize();
	void		Tick();

public:
	bool		Key_Pressing(int _iKey);
	bool		Key_Down(int _iKey);
	bool		Key_Up(int _iKey);
	bool		Key_None(int _iKey);
	bool		Key_Toggle(int _iKey);

	void		Make_Key_Down(int _iKey);
	void		Make_Key_Up(int _iKey);
	void		Make_Toggle_On(int _iKey);
	void		Make_Toggle_Off(int _iKey);

	void		Key_Lock(int _iKey);
	void		Key_Unlock(int _iKey);

	void		All_Key_Lock();
	void		All_Key_Unlock();

private:
	ST_KEY		m_eKeyState[VK_MAX];
	_bool		m_eKeyToggleState[VK_MAX];
	_bool		m_eKeyLockState[VK_MAX];
};

END
