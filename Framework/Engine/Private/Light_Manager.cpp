#include "..\Public\Light_Manager.h"
#include "Light.h"
#include "GameObject.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

const LIGHTDESC * CLight_Manager::Get_LightDesc(_uint iIndex)
{
	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_LightDesc();
}

void CLight_Manager::Get_RenderLights(_fvector vPosition, list<CLight*>* pLightList)
{
	for (auto& pLight : m_Lights)
	{
		if (pLight->Is_RangeIn(vPosition))
			pLightList->push_back(pLight);
	}
}

_uint CLight_Manager::Get_LightNum(_fvector vPosition)
{
	_uint	iLightNum = 0;
	for (auto& pLight : m_Lights)
	{
		if (pLight->Is_RangeIn(vPosition))
			++iLightNum;
	}

	return iLightNum;
}

CLight * CLight_Manager::Add_Light(const LIGHTDESC & LightDesc)
{
	CLight*			pLight = CLight::Create(LightDesc);
	if (nullptr == pLight)
		__debugbreak();

	m_Lights.push_back(pLight);

	return pLight;
}

CLight* CLight_Manager::Add_Light(CLight* pLight)
{
	NULLCHECK(pLight);

	m_Lights.push_back(pLight);

	return pLight;
}

//void CLight_Manager::Remove_Light(CLight* pLight)
//{
//	m_RemoveLights.push_back(pLight);
//	//m_Lights.remove(pLight);
//	//m_ShadowLights.remove(pLight);
//	//m_RenderLights.remove(pLight);
//}

HRESULT CLight_Manager::Initialize()
{
	USEGAMEINSTANCE;
	pGameInstance->Add_CallbackList(this, LEVEL_CHANGE);


	return S_OK;
}

void CLight_Manager::Tick(_float fTimeDelta)
{
	list<CLight*>		RemoveLights;
	for (auto& pLight : m_Lights)
	{
		if (pLight->Is_Dead())
			RemoveLights.push_back(pLight);

		pLight->Tick(fTimeDelta);
	}

	for (auto& pLight : RemoveLights)
		m_Lights.remove(pLight);

}

void CLight_Manager::LateTick(_float fTimeDelta)
{
	list<CLight*>		RemoveLights;
	for (auto& pLight : m_Lights)
	{
		if (pLight->Is_Dead())
			RemoveLights.push_back(pLight);

		pLight->LateTick(fTimeDelta);
	}

	for (auto& pLight : RemoveLights)
	{
		m_Lights.remove(pLight);
		m_RenderLights.remove(pLight);
		m_ShadowLights.remove(pLight);
	}
}

HRESULT CLight_Manager::Render_Lights(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	for (auto& pLight : m_RenderLights)
	{
		pLight->Render(pShader, pVIBuffer);
	}
	//m_RenderLights.clear();

	return S_OK;
}

HRESULT CLight_Manager::Render_AllLights(_vector vPosition, CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	for (auto& pLight : m_Lights)
	{	
		if(pLight->Is_RangeIn(vPosition))
			pLight->Render(pShader, pVIBuffer);
	}

	return S_OK;
}

HRESULT CLight_Manager::Render_Shadows(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	for (auto& pLight : m_ShadowLights)
	{
		pLight->Render_Shadows(pShader, pVIBuffer);
	}
	//m_RenderLights.clear();

	return S_OK;
}

HRESULT CLight_Manager::Render_AllShadows(_vector vPosition, CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	for (auto& pLight : m_Lights)
	{
		if (pLight->Is_RangeIn(vPosition))
			pLight->Render_Shadows(pShader, pVIBuffer);
	}

	return S_OK;
}

void CLight_Manager::On_EventMessege(EVENTDESC tEvent)
{
	if (LEVEL_CHANGE == tEvent.eEventType)
	{
		Clear();
	}
}

void CLight_Manager::Clear()
{
	USEGAMEINSTANCE;
 	for (auto& pLight : m_Lights)
	{
		//pGameInstance->Add_DeadObject(pLight);
		Safe_Release(pLight);
	}
	m_Lights.clear();
	m_RenderLights.clear();
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	USEGAMEINSTANCE;
	pGameInstance->Remove_CallbackList(this, LEVEL_CHANGE);

	m_Lights.clear();
	m_RenderLights.clear();
}
