#include "CRCHash.h"

namespace CCE::Math
{
	unsigned long long CRCHash::HashValue(const char* data, size_t size)
	{
		unsigned long long crc = 0;

		// TODO: Check if this can be processed in parallel using SSE!
		for (unsigned long i = 0; i < size; i++)
			crc = CRC64_TABLE_REF[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);

		return ReverseBits(crc, size);
	}

	unsigned long long CRCHash::ReverseBits(unsigned long long bits, size_t size)
	{
		unsigned long long rBits = 0;

		// TODO: Check if anything right here can be processed in parallel using SSE
		for (short i = 0; i < size; i++)
		{
			if ((bits & (1 << i)) != 0)
			{
				rBits |= (1 << ((64 - 1) - i));
			}
		}

		return rBits;
	}
}