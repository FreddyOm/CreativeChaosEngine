#include "CRCHash.h"

namespace CCE::Math
{
	UINT64 CRCHash::HashValue(const char* data, size_t size)
	{
		UINT64 crc = 0;
		 
		// TODO: Check if this can be processed in parallel using SSE!
		for (UINT64 i = 0; i < size; i++)
			crc = CRC64_TABLE_REF[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);

		UINT64 tmp = crc;
		crc = 0;
		for (int i = 0; i < 64; i++) {
			crc = (crc << 1) | (tmp & 1);
			tmp >>= 1;
		}

		return crc;
		//return ReverseBits(crc, size);
	}

	UINT64 CRCHash::ReverseBits(UINT64 bits, size_t size)
	{
		UINT64 rBits = 0;

		// TODO: Check if anything right here can be processed in parallel using SSE
		for (short i = 0; i < size; i++)
		{
			if ((bits & (1 << i)) != 0)
			{
				rBits |= (1 << (63 - i));
			}
		}

		return rBits;
	}
}



/* update the MSB of crc value with next input byte 
crc = (crc ^ (curByte << (width - 8))) & castMask;
/* this MSB byte value is the index into the lookup table 
var pos = (crc >> (width - 8)) & 0xFF;
/* shift out this index 
crc = (crc << 8) & castMask;
/* XOR-in remainder from lookup table using the calculated index
crc = (crc ^ crcTable[pos]) & castMask; */