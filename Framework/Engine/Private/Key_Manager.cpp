#include "..\Public\Key_Manager.h"

IMPLEMENT_SINGLETON(CKey_Manager)

CKey_Manager::CKey_Manager()
{
	ZeroMemory(m_eKeyState, sizeof(ST_KEY) * VK_MAX);
	ZeroMemory(m_eKeyToggleState, sizeof(_bool) * VK_MAX);
	//ZeroMemory(m_eKeyLockState, sizeof(m_eKeyLockState));
	for (auto& elem : m_eKeyLockState)
		elem = true;
}

bool CKey_Manager::Key_Pressing(int _iKey)
{
	if ((KEY_PRESS == m_eKeyState[_iKey]) &&
		m_eKeyLockState[_iKey])
	{
		return true;
	}
	return false;
}

bool CKey_Manager::Key_Down(int _iKey)
{
	if ((KEY_DOWN == m_eKeyState[_iKey]) &&
		m_eKeyLockState[_iKey])
	{
		return true;
	}
	return false;
}

bool CKey_Manager::Key_Up(int _iKey)
{
	if ((KEY_UP == m_eKeyState[_iKey]) &&
		m_eKeyLockState[_iKey])
	{
		return true;
	}
	return false;
}

bool CKey_Manager::Key_None(int _iKey)
{
	if ((KEY_NONE == m_eKeyState[_iKey]) &&
		m_eKeyLockState[_iKey])
	{
		return true;
	}
	return false;
}

bool CKey_Manager::Key_Toggle(int _iKey)
{
	// 이전에 누른적이 있고, 호출시점에는 눌려있지 않다면	
	//if ((GetKeyState(_iKey) & 0x1) == 0x1)
	//if ((KEY_TOGGLE_DOWN == m_eKeyToggleState[_iKey])) //이걸로 어케 바꾸지
	//	return true;
	//else
	//	return false;
	return m_eKeyToggleState[_iKey] && m_eKeyLockState[_iKey];
}

void CKey_Manager::Key_Lock(int _iKey)
{
	m_eKeyLockState[_iKey] = false;
}

void CKey_Manager::Key_Unlock(int _iKey)
{
	m_eKeyLockState[_iKey] = true;
}

void CKey_Manager::Make_Key_Down(int _iKey)
{
	m_eKeyState[_iKey] = KEY_DOWN;
}

void CKey_Manager::Make_Key_Up(int _iKey)
{
	m_eKeyState[_iKey] = KEY_UP;
}

void CKey_Manager::Make_Toggle_On(int _iKey)
{
	m_eKeyToggleState[_iKey] = true;
}

void CKey_Manager::Make_Toggle_Off(int _iKey)
{
	m_eKeyToggleState[_iKey] = false;
}

void CKey_Manager::All_Key_Lock()
{
	for (auto& elem : m_eKeyLockState)
		elem = false;
}

void CKey_Manager::All_Key_Unlock()
{
	for (auto& elem : m_eKeyLockState)
		elem = true;
}

HRESULT CKey_Manager::Initialize()
{
	for (auto iter : m_eKeyState)
		iter = KEY_NONE;


	return S_OK;
}

void CKey_Manager::Tick()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		// 이전 x
		if ((KEY_NONE == m_eKeyState[i]) || (KEY_UP == m_eKeyState[i]))
		{
			// 지금 ㅇ
			if (GetAsyncKeyState(i) & 0x8000)
			{
				m_eKeyState[i] = KEY_DOWN;

				// 토글 리스트에 입력
				m_eKeyToggleState[i] = !m_eKeyToggleState[i];

			}
			// 지금 x
			else
			{
				m_eKeyState[i] = KEY_NONE;
			}
		}
		// 이전 ㅇ
		else
		{
			// 지금 ㅇ
			if (GetAsyncKeyState(i) & 0x8000)
			{
				m_eKeyState[i] = KEY_PRESS;
			}
			// 지금 x
			else
			{
				m_eKeyState[i] = KEY_UP;
			}
		}
	}
}
