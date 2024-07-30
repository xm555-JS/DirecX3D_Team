#include "..\Public\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{
}

HRESULT CInput_Device::Initialize(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		__debugbreak();

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		__debugbreak();

	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		__debugbreak();
	if (FAILED(m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		__debugbreak();
	if (FAILED(m_pKeyBoard->Acquire()))
		__debugbreak();

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		__debugbreak();

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		__debugbreak();
	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		__debugbreak();
	if (FAILED(m_pMouse->Acquire()))
		__debugbreak();

	return S_OK;
}

HRESULT CInput_Device::SetUp_DeviceState()
{
	if (nullptr == m_pKeyBoard ||
		nullptr == m_pMouse)
		__debugbreak();	

	m_pKeyBoard->GetDeviceState(256, &m_KeyState);

	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

	return S_OK;
}

void CInput_Device::Free()
{
	Safe_Release(m_pInputSDK);

	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
}
