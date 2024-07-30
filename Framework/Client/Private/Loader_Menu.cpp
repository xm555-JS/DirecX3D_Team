#include "stdafx.h"
#include "..\Public\Loader_Menu.h"
#include "GameInstance.h"


CLoader_Menu::CLoader_Menu()
{
}

HRESULT CLoader_Menu::Initialize()
{
	return S_OK;
}

HRESULT CLoader_Menu::Loading_Object()
{
	USEGAMEINSTANCE;

	///* For.Prototype_GameObject_BackGround */
	//if (FAILED(pGameInstance->Add_GameObject_Prototype(ID_UINT(BACKGROUND),
	//	CBackGround::Create())))
	//	__debugbreak();

	///* For.Prototype_GameObject_Logo */
	//if (FAILED(pGameInstance->Add_GameObject_Prototype(ID_UINT(LOGO),
	//	CLogo::Create())))
	//	__debugbreak();

	///* For.Prototype_GameObject_Button */
	//if (FAILED(pGameInstance->Add_GameObject_Prototype(ID_UINT(BUTTON),
	//	CButton::Create())))
	//	__debugbreak();



	return S_OK;
}

HRESULT CLoader_Menu::Loading_Texture()
{
	USEGAMEINSTANCE;

	///* For.Prototype_Component_Texture_Default */
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_MENU, ID_UINT(TEXTURE, TEXTURE_DEFAULT),
	//	CTexture::Create(TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
	//	__debugbreak();

	///* For.Prototype_Component_Texture_BackGround */
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_MENU, ID_UINT(TEXTURE, TEXTURE_BACKGROUND),
	//	CTexture::Create(TEXT("../Bin/Resources/Textures/BackGround/BackGround%d.png"), 3))))
	//	__debugbreak();

	///* For.Prototype_Component_Texture_Logo */
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_MENU, ID_UINT(TEXTURE, TEXTURE_LOGO),
	//	CTexture::Create(TEXT("../Bin/Resources/Textures/Logo/Logo%d.png"), 1))))
	//	__debugbreak();

	return S_OK;
}

HRESULT CLoader_Menu::Loading_Model()
{
	USEGAMEINSTANCE;



	return S_OK;
}

HRESULT CLoader_Menu::Loading_Shader()
{
	USEGAMEINSTANCE;



	return S_OK;
}

CLoader_Menu * CLoader_Menu::Create()
{
	CLoader_Menu*		pInstance = new CLoader_Menu();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLoader_Menu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Menu::Free()
{
}

