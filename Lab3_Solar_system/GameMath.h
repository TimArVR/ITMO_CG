#pragma once
#include <cmath>
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

template <typename T>
constexpr auto sq(const T& x)
{
	return x * x;
}

template<typename T>
T wrap_angle(T theta)
{
	const T mod = fmod(theta, DirectX::XM_2PI);
	if (mod > (T)DirectX::XM_PI)
	{
		return mod - DirectX::XM_2PI;
	}
	else if (mod < (T)DirectX::XM_PI)
	{
		return mod + DirectX::XM_2PI;
	}
	return mod;
}

template<typename T>
constexpr T interpolate(const T& src, const T& dst, float alpha)
{
	return src + (dst - src) * alpha;
}

template<typename T>
constexpr T to_rad(T deg)
{
	return deg * DirectX::XM_PI / (T)180.0;
}

template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
	assert(!(hi < lo));
	return (v < lo) ? lo : (hi < v) ? hi : v;
}
