#include "..\Public\Easing_Utillity.h"

#define PI 3.141592

CEasing_Utillity::CEasing_Utillity()
{
}

CEasing_Utillity::~CEasing_Utillity()
{
}

_vector CEasing_Utillity::Easing(_uint eEasingType, _vector vStartPoint, _vector vTargetPoint, _double dPassedTime, _double dTotalTime)
{
	if (eEasingType >= TYPE_End)
		return _vector();

	switch (eEasingType)
	{
	case Engine::CEasing_Utillity::TYPE_Linear:
		return Linear(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_QuadIn:
		return QuadIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_QuadOut:
		return  QuadOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_QuadInOut:
		return  QuadInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_CubicIn:
		return  CubicIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_CubicOut:
		return  CubicOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_CubicInOut:
		return  CubicInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_QuarticIn:
		return  QuarticIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_QuarticOut:
		return  QuarticOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_QuarticInOut:
		return QuarticInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_QuinticIn:
		return  QuinticIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_QuinticOut:
		return QuinticOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_QuinticInOut:
		return QuinticInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_SinIn:
		return SinIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_SinOut:
		return  sinfOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_SinInOut:
		return sinfInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_ExpoIn:
		return  ExpoIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_ExpoOut:
		return  ExpoOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_ExpoInOut:
		return  ExpoInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_CircularIn:
		return  CircularIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_CircularOut:
		return  CircularOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_CircularInOut:
		return  CircularInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_ElasticIn:
		return  ElasticEaseIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_ElasticOut:
		return  ElasticEaseOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_ElasticInOut:
		return  ElasticEaseInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_BounceIn:
		return  BounceEaseIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasing_Utillity::TYPE_BounceOut:
		return  BounceEaseOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	default:
		break;
	}

	return _vector();
}


_vector CEasing_Utillity::Linear(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector((vTargetPoint - vStartPoint) * fPassedTime / fTotalTime + vStartPoint);
}

_vector CEasing_Utillity::QuadIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector((vTargetPoint - vStartPoint) * fPassedTime*fPassedTime + vStartPoint);
}

_vector CEasing_Utillity::QuadOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector(-(vTargetPoint - vStartPoint) * fPassedTime*(fPassedTime - 2) + vStartPoint);
}

_vector CEasing_Utillity::QuadInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _vector((vTargetPoint - vStartPoint) / 2 * fPassedTime*fPassedTime + vStartPoint);
	fPassedTime--;
	return _vector(-(vTargetPoint - vStartPoint) / 2 * (fPassedTime*(fPassedTime - 2) - 1) + vStartPoint);
}

_vector CEasing_Utillity::CubicIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector((vTargetPoint - vStartPoint) * fPassedTime*fPassedTime*fPassedTime + vStartPoint);
}

_vector CEasing_Utillity::CubicOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _vector((vTargetPoint - vStartPoint) * (fPassedTime*fPassedTime*fPassedTime + 1) + vStartPoint);
}

_vector CEasing_Utillity::CubicInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return (vTargetPoint - vStartPoint) / 2 * fPassedTime*fPassedTime*fPassedTime + vStartPoint;
	fPassedTime -= 2;
	return _vector((vTargetPoint - vStartPoint) / 2 * (fPassedTime*fPassedTime*fPassedTime + 2) + vStartPoint);
}

_vector CEasing_Utillity::QuarticIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector((vTargetPoint - vStartPoint) * fPassedTime*fPassedTime*fPassedTime*fPassedTime + vStartPoint);
}

_vector CEasing_Utillity::QuarticOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _vector(-(vTargetPoint - vStartPoint) * (fPassedTime*fPassedTime*fPassedTime*fPassedTime - 1) + vStartPoint);
}

_vector CEasing_Utillity::QuarticInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _vector((vTargetPoint - vStartPoint) / 2 * fPassedTime*fPassedTime*fPassedTime*fPassedTime + vStartPoint);
	fPassedTime -= 2;
	return _vector(-(vTargetPoint - vStartPoint) / 2 * (fPassedTime*fPassedTime*fPassedTime*fPassedTime - 2) + vStartPoint);
}

_vector CEasing_Utillity::QuinticIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector((vTargetPoint - vStartPoint) * fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + vStartPoint);
}

_vector CEasing_Utillity::QuinticOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _vector((vTargetPoint - vStartPoint) * (fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + 1) + vStartPoint);
}

_vector CEasing_Utillity::QuinticInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _vector((vTargetPoint - vStartPoint) / 2 * fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + vStartPoint);
	fPassedTime -= 2;
	return _vector((vTargetPoint - vStartPoint) / 2 * (fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + 2) + vStartPoint);
}

_vector CEasing_Utillity::SinIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector(-(vTargetPoint - vStartPoint) * cosf(fPassedTime / fTotalTime * (PI / 2)) + (vTargetPoint - vStartPoint) + vStartPoint);
}

_vector CEasing_Utillity::sinfOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector((vTargetPoint - vStartPoint) * sinf(fPassedTime / fTotalTime * (PI / 2)) + vStartPoint);
}

_vector CEasing_Utillity::sinfInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector(-(vTargetPoint - vStartPoint) / 2 * (cosf(PI*fPassedTime / fTotalTime) - 1) + vStartPoint);
}

_vector CEasing_Utillity::ExpoIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector((vTargetPoint - vStartPoint) * pow(2, 10.0f * (fPassedTime / fTotalTime - 1)) + vStartPoint);
}

_vector CEasing_Utillity::ExpoOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector((vTargetPoint - vStartPoint) * (-pow(2, -10 * fPassedTime / fTotalTime) + 1) + vStartPoint);
}

_vector CEasing_Utillity::ExpoInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _vector((vTargetPoint - vStartPoint) / 2 * pow(2, 10 * (fPassedTime - 1)) + vStartPoint);
	fPassedTime--;
	return _vector((vTargetPoint - vStartPoint) / 2 * (-pow(2, -10 * fPassedTime) + 2) + vStartPoint);
}

_vector CEasing_Utillity::CircularIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector(-(vTargetPoint - vStartPoint) * (sqrtf(1 - fPassedTime * fPassedTime) - 1) + vStartPoint);
}

_vector CEasing_Utillity::CircularOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _vector((vTargetPoint - vStartPoint) * sqrtf(1 - fPassedTime * fPassedTime) + vStartPoint);
}

_vector CEasing_Utillity::CircularInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _vector(-(vTargetPoint - vStartPoint) / 2 * (sqrtf(1 - fPassedTime * fPassedTime) - 1) + vStartPoint);
	fPassedTime -= 2;
	return _vector((vTargetPoint - vStartPoint) / 2 * (sqrtf(1 - fPassedTime * fPassedTime) + 1) + vStartPoint);
}

_vector CEasing_Utillity::ElasticEaseOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{

	if ((fPassedTime /= fTotalTime) == 1)
		return vStartPoint + (vTargetPoint - vStartPoint);

	_double p = fTotalTime * .3f;
	_double s = p / 4;

	return _vector((vTargetPoint - vStartPoint) * pow(2, -10 * fPassedTime) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p) + (vTargetPoint - vStartPoint) + vStartPoint);
}

_vector CEasing_Utillity::ElasticEaseIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	if ((fPassedTime /= fTotalTime) == 1)
		return vStartPoint + (vTargetPoint - vStartPoint);

	_double p = fTotalTime * .3f;
	_double s = p / 4.f;

	return _vector(-((vTargetPoint - vStartPoint) * pow(2, 10 * (fPassedTime -= 1)) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p)) + vStartPoint);
}

_vector CEasing_Utillity::ElasticEaseInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	if ((fPassedTime /= fTotalTime / 2) == 2)
		return _vector(vStartPoint + (vTargetPoint - vStartPoint));

	_double p = fTotalTime * (.3f * 1.5f);
	_double s = p / 4;

	if (fPassedTime < 1)
		return _vector(-.5f * ((vTargetPoint - vStartPoint) * pow(2, 10 * (fPassedTime -= 1)) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p)) + vStartPoint);
	return _vector((vTargetPoint - vStartPoint) * pow(2, -10 * (fPassedTime -= 1)) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p) * .5f + (vTargetPoint - vStartPoint) + vStartPoint);
}

_vector CEasing_Utillity::BounceEaseOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	if ((fPassedTime /= fTotalTime) < (1 / 2.75f))
		return _vector((vTargetPoint - vStartPoint) * (7.5625f * fPassedTime * fPassedTime) + vStartPoint);
	else if (fPassedTime < (2 / 2.75f))
		return _vector((vTargetPoint - vStartPoint) * (7.5625f * (fPassedTime -= (1.5f / 2.75f)) * fPassedTime + .75f) + vStartPoint);
	else if (fPassedTime < (2.5f / 2.75f))
		return _vector((vTargetPoint - vStartPoint) * (7.5625f * (fPassedTime -= (2.25f / 2.75f)) * fPassedTime + .9375f) + vStartPoint);
	else
		return _vector((vTargetPoint - vStartPoint) * (7.5625f * (fPassedTime -= (2.625f / 2.75f)) * fPassedTime + .984375f) + vStartPoint);
}


_vector CEasing_Utillity::BounceEaseIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector(BounceEaseOut(vTargetPoint, vStartPoint, fTotalTime - fPassedTime, fTotalTime));
}
//_float CEasing_Utillity::BounceEaseInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
//{
//	if (dPassedTime < dTotalTime * 0.5f)
//		return BounceEaseIn(vStartPoint, vTargetPoint * 0.5f, dPassedTime , dTotalTime * 0.5f);
//	else
//		return BounceEaseOut(vTargetPoint * 0.5f, vTargetPoint , dPassedTime-(dTotalTime * 0.5f) , dTotalTime * 0.5f);
//}
//
//_float CEasing_Utillity::BounceEaseOutIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime)
//{
//
//	if (dPassedTime < dTotalTime * 0.5f )
//		return BounceEaseOut(vStartPoint, vTargetPoint * 0.5f, dPassedTime * 0.5f, dTotalTime * 0.5f);
//	return BounceEaseIn(vTargetPoint * 0.5f, vTargetPoint, dPassedTime * 0.5f, dTotalTime * 0.5f);
//}

