#pragma once



/* 컴포넌트들의 원형을 가지고 있는다. */
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"
#include "Collider.h"
#include "PhysXCollider.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Rect_Instance.h"
//#include "VIBuffer_Cloud_Instance.h"
#include "VIBuffer_Point_Instance.h"
#include "VIBuffer_Cube.h"
//#include "VIBuffer_ColorCube.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Terrain_Tess.h"
#include "VIBuffer_Terrain_Custom.h"
#include "VIBuffer_Terrain_Tess_Custom.h"
#include "VIBuffer_Trail.h"


BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
public:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Get_Component_Prototypes(ID_UINT iPrototypeID, _uint iLevelIndex, map<_uint, CComponent*>* pPrototypes, _uint iUseIDNum = 1);


	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Component_Prototype(_uint iLevelIndex, ID_UINT iPrototypeID, class CComponent* pPrototype);
	HRESULT Add_Component_Prototype(_uint iLevelIndex, ID_UINT iPrototypeID, CComponent* pPrototype, string strModelFileName);
	class CComponent* Clone_Component(_uint iLevelIndex, ID_UINT iPrototypeID, void* pArg);
	HRESULT	Get_Component_Prototypes(ID_UINT iPrototypeID, map<_uint, class CComponent*>* pPrototypes, _uint iUseIDNum = 1);
	HRESULT	Get_Component_Prototypes(_uint iLevel, ID_UINT iPrototypeID, map<_uint, class CComponent*>* pPrototypes, _uint iUseIDNum = 1);
	HRESULT	Get_Component_Prototypes(_uint iPrototypeID, map<_uint, class CComponent*>* pPrototypes, _uint iUseIDNum = 1);
	HRESULT	Get_Component_Prototypes(_uint iLevel, _uint iPrototypeID, map<_uint, class CComponent*>* pPrototypes, _uint iUseIDNum = 1);
public:
	void Clear(_uint iLevelIndex);

private:
	_uint			m_iNumLevels = 0;

	map<_uint, class CComponent*>* m_pPrototypes = nullptr;
	typedef map<_uint, class CComponent*>		PROTOTYPES;

private:
	class CComponent* Find_Components(_uint iLevelIndex, ID_UINT iPrototypeID);

public:
	virtual void Free() override;
};

END


// Object Manager랑 Component Manager랑 합친다.
// Object manager가 Component Prototype 소유하게 만들고 Component Proto는 레벨에 따라 분류 두지 않는다.
// Event 통해서 tick 호출하게 만들어라(Late tick도) (아마 Collision Mananger 처럼 자기 자신을 Tick 리트스에 올리는 식으로 하면 되지 않을까?)
// 
//