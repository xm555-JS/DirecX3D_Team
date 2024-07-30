#pragma once
#include "Client_Defines.h"
#include "Window_Imgui.h"

#if defined(USE_IMGUI)

BEGIN(Client)

class CWindow_Tool_Option final : public CWindow_Imgui
{

public:
	CWindow_Tool_Option();
	virtual ~CWindow_Tool_Option();

public: /*For Debug*/
	_float			Get_DebugFloat() { return m_fDebugfloat; }

	_float4			Get_DebugFloat4(_uint iIdx) { 
		if(iIdx < m_vDebugfloat4.size())
			return m_vDebugfloat4[iIdx];
		else
			return _float4(0.f, 0.f, 0.f, 0.f);
	}

	_fvector		Get_DebugVector(_uint iIdx) {
		if (iIdx < m_vDebugfloat4.size())
			return XMLoadFloat4(&m_vDebugfloat4[iIdx]);
		else
			return XMVectorSet(0.f, 0.f, 0.f, 0.f);
		
	}
	CAMERA_TYPE		Get_CameraType() { return (CAMERA_TYPE)m_iCameraType; }


public:
	// CWindow_Imgui을(를) 통해 상속됨
	virtual void Tick(_float fTimeDelta) override;
	virtual void Initialize() override;

public:
	void	ShowLevelList(_uint* iSelected, _uint iLevelIndex);
	
private:
	bool show_demo_window = true;
	bool show_another_window = false;

private: /*For Shader*/
	_bool						m_ShaderSelectState[SHADER_END];
	string						m_ShaderName[SHADER_END];


private: /*For Debug*/
	_float						m_fDebugfloat = 0.f;
	vector<_float4>				m_vDebugfloat4;

private: /*For Camera*/
	_int						m_iCameraType = CAMERA_END;


private:
	void	Show_Window_LevelSelect();
	void	Show_Window_Shader();
	void	Show_Window_Debug();
	void	Show_Window_Camera();
	

private: /*For Shader*/
	void	Setup_ShaderName();


	void	Show_Window_ShaderBloom();
	void	Show_Window_ShaderBlur();
	void	Show_Window_ShaderGlow();
	void	Show_Window_ShaderShadow();
	void	Show_Window_ShaderSaturation();
	void	Show_Window_ShaderBrightness();


public:
	virtual	void	Free() override;
};

END

#endif