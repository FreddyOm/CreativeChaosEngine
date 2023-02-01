#include "CCMath.h"

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
}