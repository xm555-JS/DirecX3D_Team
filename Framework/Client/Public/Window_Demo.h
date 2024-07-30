#pragma once
#include "Client_Defines.h"
#include "Window_Imgui.h"

#if defined(USE_IMGUI)

BEGIN(Client)

class CWindow_Demo final : public CWindow_Imgui
{

public:
	CWindow_Demo();
	virtual ~CWindow_Demo();

public:
	// CWindow_Imgui을(를) 통해 상속됨
	virtual void Tick(_float fTimeDelta) override;
	virtual void Initialize() override;
	
private:
	bool show_demo_window = true;
	bool show_another_window = false;


public:
	virtual	void	Free() override;
};

END

#endif