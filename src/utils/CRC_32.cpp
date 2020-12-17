#include "include/utils/CRC_32.h"
#include "utils/tdebug.h"

#define CRC_INIT_VALUE 0xFFFFFFFF

CCRC_32::CCRC_32()
{
	InitialCrcTable();
	m_CrcValue = CRC_INIT_VALUE;
}

CCRC_32::CCRC_32(unsigned long CrcValue)
{
	CCRC_32();
	m_CrcValue = CrcValue;
}


CCRC_32::~CCRC_32()
{
}

void CCRC_32::Reset(void)
{
	m_CrcValue = CRC_INIT_VALUE;
}

unsigned long CCRC_32::Calculate(const unsigned char * pData, unsigned int dataSize)
{
	// Calculate the CRC
	unsigned long CRC = m_CrcValue;

    int index = 0;
	while (dataSize--)
	{
//        if(index < 20)
//        {
//            TDEBUG("执行前Fireware:Calculate: m_CrcTable[(int)(CRC ^ TouchManager.byteToInt(pData[%d])) & 0xFF] = 0x%x  (CRC >> 8) = 0x%x",
//                   index,m_CrcTable[(CRC ^ *pData) & 0xFF],(CRC >> 8));
//        }

		CRC = (CRC >> 8) ^ m_CrcTable[(CRC ^ *pData) & 0xFF];
//        if(index < 20)
//        {
//            TDEBUG("执行后Fireware:Calculate: CRC = 0x%x", CRC);
//        }
//        index++;
		pData++;
	}
	m_CrcValue = CRC;
    TDEBUG("Fireware:Calculate: m_CrcValue = 0x%x", m_CrcValue);
	return GetCrcResult();
}

unsigned long CCRC_32::GetCrcResult(void)
{
	return (~m_CrcValue);
}



void CCRC_32::InitialCrcTable(void)
{
	// This is the official polynomial used by CRC-32 
	// in PKZip, WinZip and Ethernet. 
	const unsigned long ulPolynomial = 0x04C11DB7;

	// 256 values representing ASCII character codes.
	for (int i = 0; i <= 0xFF; i++)
	{
		m_CrcTable[i] = Reflect(i, 8) << 24;
//        TDEBUG("前Fireware:InitialCrcTable: m_CrcTable[%d] = %lu",i,m_CrcTable[i]);
		for (int j = 0; j < 8; j++)
		{
            if((m_CrcTable[i] & (1 << 31)) == 0)
            {
//                TDEBUG("前Fireware:InitialCrcTable: m_CrcTable[%d] = 0x%0x",i,m_CrcTable[i]);
//                TDEBUG("前Fireware:InitialCrcTable: (m_CrcTable[i] << 1) = 0x%0x",(m_CrcTable[i] << 1));
                m_CrcTable[i] = (m_CrcTable[i] << 1) ^ 0;
//                TDEBUG("执行0Fireware:InitialCrcTable: m_CrcTable[%d] = 0x%0x",i,m_CrcTable[i]);
            }
            else
            {
               /* TDEBUG("前Fireware:InitialCrcTable: m_CrcTable[%d] = 0x%0x",i,m_CrcTable[i]);
                TDEBUG("前Fireware:InitialCrcTable: (m_CrcTable[i] << 1) = 0x%0x",(m_CrcTable[i] << 1));*/
                m_CrcTable[i] = (m_CrcTable[i]  << 1) ^ ulPolynomial;
//                TDEBUG("执行1Fireware:InitialCrcTable: m_CrcTable[%d] = 0x%0x",i,m_CrcTable[i]);
            }
//			m_CrcTable[i] = (m_CrcTable[i] << 1) ^ (m_CrcTable[i] & (1 << 31) ? ulPolynomial : 0);
		}
//        TDEBUG("中Fireware:InitialCrcTable: m_CrcTable[%d] = %lu",i,m_CrcTable[i]);
		m_CrcTable[i] = Reflect(m_CrcTable[i], 32);
//        TDEBUG("后Fireware:InitialCrcTable: m_CrcTable[%d] = %lu",i,m_CrcTable[i]);
	}
//    for(int i = 0;i < 32 * 8;i++)
//    {
//        TDEBUG("m_CrcTable[%d] = %lu\t",i,m_CrcTable[i]);
//    }
}

unsigned long CCRC_32::Reflect(unsigned long ref, unsigned char ch)
{
//    TDEBUG("Fireware: Reflect: ref = %lu" , ref);
	unsigned long value = 0;
	// Swap bit 0 for bit 7
	// bit 1 for bit 6, etc.
	for (int i = 1; i < (ch + 1); i++)
	{
		if (ref & 1)
		{
			value |= 1 << (ch - i);
		}
		ref >>= 1;
    }
//    TDEBUG("Fireware: Reflect: value = %lu" , value);
    return value;
}

