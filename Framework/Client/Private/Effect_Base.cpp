#include "stdafx.h"
#include "Effect_Base.h"
#include "GameInstance.h"
#include "Easing_Utillity.h"

#include "Model.h"
#include "HIerarchyNode.h"

CEffect_Base::CEffect_Base()
{
}

CEffect_Base::CEffect_Base(const CEffect_Base& rhs)
	: CGameObject(rhs)
{
}

CEffect_Base* CEffect_Base::Create()
{
	CEffect_Base* pInstance = new CEffect_Base();
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Base");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Base::Clone(void* pArg)
{
	CEffect_Base* pInstance = new CEffect_Base(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Base::Free()
{
	__super::Free();
}