#pragma once
#include "../Core.h"

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
	
		static float Clamp(const float value, const float min, const float max) noexcept;
		static float Clamp01(const float value) noexcept;
	};
}


