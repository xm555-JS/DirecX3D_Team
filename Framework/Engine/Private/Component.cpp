#include "..\Public\Component.h"

CComponent::CComponent()
	: m_isCloned(false)
{

}

CComponent::CComponent(const CComponent & Prototype)
	: m_isCloned(true)
	, m_strModelFileName(Prototype.m_strModelFileName)
	, m_iPrototypeID(Prototype.m_iPrototypeID)
{

}

HRESULT CComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CComponent::Initialize(void * pArg)
{
	return S_OK;
}

void CComponent::Free()
{
}