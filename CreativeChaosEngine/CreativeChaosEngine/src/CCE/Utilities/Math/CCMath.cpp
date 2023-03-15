#include "CCMath.h"
#include "../../Analysis/Debug.h"

namespace CCE::Math
{
	void CCMath::Add(int* a, int* b, int* res) noexcept
	{
		*res = *a + *b;
	}

	void CCMath::Add(long* a, long* b, long* res) noexcept
	{
		*res = *a + *b;
	}

	void CCMath::Add(double* a, double* b, double* res) noexcept
	{
		*res = *a + *b;
	}

	void CCMath::Add(float* a, float* b, float* res) noexcept
	{
		*res = *a + *b;
	}

	void CCMath::Substract(int* a, int* b, int* res) noexcept
	{
		*res = *a - *b;
	}

	void CCMath::Substract(long* a, long* b, long* res) noexcept
	{
		*res = *a - *b;
	}

	void CCMath::Substract(double* a, double* b, double* res) noexcept
	{
		*res = *a - *b;
	}

	void CCMath::Substract(float* a, float* b, float* res) noexcept
	{
		*res = *a - *b;
	}

	float CCMath::Clamp(const float value, const float min, const float max) noexcept
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

	float CCMath::Clamp01(const float value) noexcept
	{
		return Clamp(value, 0.0f, 1.0f);
	}
}