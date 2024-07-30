#include "stdafx.h"
#include "..\Public\Loader_Edit_UI.h"
#include "GameInstance.h"

#include "Sky.h"
/* For.Camera */
#include "Camera_Free.h"
#include "Camera_Fixed.h"

/* For.UI */
#include "UI_Loading_Screen.h"
#include "UI_Cursor.h"


CLoader_Edit_UI::CLoader_Edit_UI()
{
}

HRESULT CLoader_Edit_UI::Initialize()
{
	return S_OK;
}

HRESULT CLoader_Edit_UI::Loading_Object()
{
	USEGAMEINSTANCE;

	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(SKY), CSky::Create()));

	/* For.Camera */
	//FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(CAMERA, CAMERA_FREE), CCamera_Free::Create()));
	//FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(CAMERA, CAMERA_FIXED), CCamera_Fixed::Create()));

	/* For.UI*/
	//FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_LOADING_SCREEN), CUI_Loading_Screen::Create()));


	return S_OK;
}

HRESULT CLoader_Edit_UI::Loading_Texture()
{
	USEGAMEINSTANCE;
	/* For.Prototype_Component_Texture_Sky */
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_UI, ID_UINT(TEXTURE, TEXTURE_SKY),
		CTexture::Create(TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 7)));



	return S_OK;
}

HRESULT CLoader_Edit_UI::Loading_Model()
{
	USEGAMEINSTANCE;

	return S_OK;
}

HRESULT CLoader_Edit_UI::Loading_Shader()
{
	USEGAMEINSTANCE;



	return S_OK;
}

CLoader_Edit_UI * CLoader_Edit_UI::Create()
{
	CLoader_Edit_UI*		pInstance = new CLoader_Edit_UI();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLoader_Edit_UI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Edit_UI::Free()
{
}

