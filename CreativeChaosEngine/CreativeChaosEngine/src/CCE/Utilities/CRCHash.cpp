#include "CRCHash.h"

namespace CCE::Math
{
	unsigned long long CRCHash::HashValue(const char* data, unsigned long size)
	{
		unsigned long long crc = 0;

		for (unsigned long i = 0; i < size; i++)
			crc = CRC64_TABLE_REF[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);

		return ReverseBits(crc);
	}

	unsigned long long CRCHash::ReverseBits(unsigned long long bits)
	{
		unsigned long long rBits = 0;

		for (short i = 0; i < 64; i++)
		{
			if ((bits & (1 << i)) != 0)
			{
				rBits |= (1 << ((64 - 1) - i));
			}
		}

		return rBits;
	}
}