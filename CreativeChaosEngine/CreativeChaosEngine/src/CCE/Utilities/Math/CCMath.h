#pragma once
#include "../../Core.h"
#include <cmath>
#include <DirectXMath.h>
#include "../../Analysis/Debug.h"

namespace CCE::Math
{
	struct CCE_API CCMath
	{
		static void Add(int* a, int* b, int* res) noexcept;
		static void Add(long* a, long* b, long* res) noexcept;
		static void Add(double* a, double* b, double* res) noexcept;
		static void Add(float* a, float* b, float* res) noexcept;

		static void Substract(int* a, int* b, int* res) noexcept;
		static void Substract(long* a, long* b, long* res) noexcept;
		static void Substract(double* a, double* b, double* res) noexcept;
		static void Substract(float* a, float* b, float* res) noexcept;
		
		template<typename T>
		static T Clamp(const T value, const T min, const T max) noexcept
		{
			DASSERT(max > min, "The max value must not be less than the min value!");
			if (value < min)
			{
				return min;
			}
			else if (value > max)
			{
				return max;
			}
			else
			{
				return value;
			}
		}

		static DirectX::XMFLOAT3 Clamp(const DirectX::XMFLOAT3 value, const DirectX::XMFLOAT3 min, const DirectX::XMFLOAT3 max) noexcept
		{
			float x = value.x > max.x ? max.x : value.x;
			float y = value.y > max.y ? max.y : value.y;
			float z = value.z > max.z ? max.z : value.z;

			x = x < min.x ? min.x : x;
			y = y < min.y ? min.y : y;
			z = z < min.z ? min.z : z;

			return {x, y, z};
		}

		static float Clamp01(const float value) noexcept;

		template<typename T>
		static T& Min(const T& first, const T& second)
		{
			return first > second ? second : first;
		}

		template<typename T>
		static T& Max(const T& first, const T& second)
		{
			return first < second ? second : first;
		}

		template<typename T>
		static T& Abs(const T& value)
		{
			return value > 0.0f ? value : -value;
		}
	};
}
