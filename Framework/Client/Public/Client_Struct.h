#pragma once

namespace Client
{
	typedef struct UnitDesc
	{
		UnitDesc()
		{
			ePropertie = ELEMENTAL_END;
			fHP = 0.f;
			fAttack = 0.f;
			fSpeed = 0.f;
		}
		UnitDesc(ELEMENTAL_PROPERTIE _ePropertie, float _fHP, float _fAttack, float _fSpeed)
		{
			ePropertie = _ePropertie;
			fHP = _fHP;
			fAttack = _fAttack;
			fSpeed = _fSpeed;
		}

		ELEMENTAL_PROPERTIE ePropertie;
		float				fHP;
		float				fAttack;
		float				fSpeed;

	}UNITDESC;

}