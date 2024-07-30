#pragma once


namespace Client 
{
	static const unsigned int g_iWinCX = 1280;
	static const unsigned int g_iWinCY = 720;

	static const float g_fFar = 1000.f;
	static const float g_fNear = 0.2f;

	constexpr int NUMSUNLAYER = 1;

	//static const unsigned int g_iWinCX = 1920;
	//static const unsigned int g_iWinCY = 1080;

}

#include "Client_Macro.h"
#include "Client_Enum.h"
#include "Client_Struct.h"
#include "Client_Functor.h"


#ifdef _DEBUG
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#endif // _DEBUG


extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Client;