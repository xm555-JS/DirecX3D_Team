#pragma once
#include "Client_Defines.h"
#include "Window_Imgui.h"

#if defined(USE_IMGUI)

BEGIN(Client)

class CWindow_LevelInfo final : public CWindow_Imgui
{

public:
	CWindow_LevelInfo();
	virtual ~CWindow_LevelInfo();


public:
	void	Add_Object(SAVEDESC* ObjectDesc);
	void	Delete_Object(CGameObject* pObject);

public:
	// CWindow_Imgui을(를) 통해 상속됨
	virtual void Tick(_float fTimeDelta) override;
	virtual void Initialize() override;

public: /* For. Widget Bar*/
	void	InputBox_FindLevel();
	//void	ListBox_LevelList();
	void	ListBox_ObjectList();
	void	Button_SaveLevel();
	void	Button_LoadLevel();
	void	Button_RefreshLevel();
	
private:
	char							m_szLvNameBuffer[255];
	
	vector<const char*>				m_vecLevels;
	vector<SAVEDESC>				m_Objects;

	map<_uint, _uint>				m_ObjectsNum;

	typedef	vector<SAVEDESC>		OBJECTS;

	size_t							m_Object_CurIndex = 0;

	CWindow_Imgui*					m_pWindow_ObjectDesc = nullptr;

private:
	HRESULT		Save_ObjectInfo(wstring wstrFileName, OBJECTS Objects);


public:
	virtual	void	Free() override;

};

END

#endif