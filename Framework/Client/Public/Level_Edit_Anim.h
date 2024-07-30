#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
	class CCamera;
END

BEGIN(Client)

class CLevel_Edit_Anim final : public CLevel
{
public:
	CLevel_Edit_Anim();
	virtual ~CLevel_Edit_Anim() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();
public:
	virtual	vector<CCamera*>* Get_FreeCameras() override { return &m_Cameras[CAMERA_FREE]; }
	virtual	void	Add_FreeCamera() override;


public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Sky(ID_UINT iLayerID);
	HRESULT Ready_Layer_Camera(ID_UINT iLayerID);
	HRESULT Ready_Layer_BackGround(ID_UINT iLayerID);
	HRESULT	Ready_Layer_Player(ID_UINT iID);
	HRESULT Ready_Layer_Fixedbody(ID_UINT iLayerID);
	

	
private:
	vector<CCamera*>	m_Cameras[CAMERA_END];

public:
	static CLevel_Edit_Anim* Create();
	virtual void Free() override;
};

END