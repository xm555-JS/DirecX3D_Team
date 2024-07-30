#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
public:
	CLevel_Loading();
	virtual ~CLevel_Loading() = default;

public:
	LEVEL	Get_NextLevel() { return m_eNextLevel; }

public:
	virtual HRESULT Initialize(LEVEL eNextLevel);
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

private:
	LEVEL					m_eNextLevel = LEVEL_END;
	class CLoader*			m_pLoader = nullptr;
	class CBackGround*		m_pBackGround = nullptr;
	class CLoading_Anim*	m_pLoading_Anim = nullptr;

	_float					m_fAccTime = 0.f;

private:
	class CUI_Loading_Bar* m_pLoadingBar = nullptr;

private:
	HRESULT Ready_Layer_UI(ID_UINT iLayerID);
	HRESULT	Ready_Layer_Camera(ID_UINT iLayerID);

public:
	static CLevel_Loading* Create(LEVEL eNextLevel);
	virtual void Free() override;
};

END