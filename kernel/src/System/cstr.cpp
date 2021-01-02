#include "cstr.h"

char uIntToStringOutput[128];
const char* to_string(uint64_t value)
{
	uint8_t size;
	uint64_t sizeTest = value;

	while (sizeTest / 10 > 0)
	{
		sizeTest /= 10;
		size++;
	}

	uint8_t index = 0;

	while (value / 10 > 0)
	{
		uint8_t remainder = value % 10;
		value /= 10;

		uIntToStringOutput[size - index] = remainder + '0';

		index++;
	}

	uint8_t remainder = value % 10;
	uIntToStringOutput[size - index] = remainder + '0';
	uIntToStringOutput[size + 1] = 0;

	return uIntToStringOutput;
}

char intToStringOutput[128];
const char* to_string(int64_t value)
{
	uint8_t isNegative = 0;

	if (value < 0) 
	{
		isNegative = 1;
		value *= -1;
		intToStringOutput[0] = '-';
	}

	uint8_t size;
	uint64_t sizeTest = value;

	while (sizeTest / 10 > 0)
	{
		sizeTest /= 10;
		size++;
	}

	uint8_t index = 0;

	while (value / 10 > 0)
	{
		uint8_t remainder = value % 10;
		value /= 10;

		intToStringOutput[isNegative + size - index] = remainder + '0';

		index++;
	}

	uint8_t remainder = value % 10;
	intToStringOutput[isNegative + size - index] = remainder + '0';
	intToStringOutput[isNegative + size + 1] = 0;

	return intToStringOutput;
}

const char* to_string(double value)
{
	return to_string(value, 2);
}

char doubleToStringOutput[128];
const char* to_string(double value, uint8_t decimals)
{
	if (decimals > 20) decimals = 20;

	char* intPtr = (char*)to_string((int64_t)value);
	char* doublePtr = doubleToStringOutput;

	if (value < 0)
	{
		value *= -1;
	}

	while (*intPtr != 0)
	{
		*doublePtr = *intPtr;
		
		intPtr++;
		doublePtr++;
	}

	*doublePtr = '.';
	doublePtr++;

	double newValue = value - (int)value;

	for (uint8_t i = 0; i < decimals; i++)
	{
		newValue *= 10;
		*doublePtr = (int)newValue + '0';
		newValue -= (int)newValue;
		doublePtr++;
	}

	*doublePtr = 0;
	return doubleToStringOutput;
}

char hexToStringOutput64[128];
const char* to_hstring(uint64_t value)
{
	uint64_t* valuePtr = &value;
	uint8_t* ptr;
	uint8_t temp;
	uint8_t size = 8 * 2 - 1;

	for (uint8_t i; i < size; i++)
	{
		ptr = ((uint8_t*)valuePtr + i);
		temp = ((*ptr & 0xF0) >> 4);

		hexToStringOutput64[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');

		temp = ((*ptr & 0x0F));
		hexToStringOutput64[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');
	}

	hexToStringOutput64[size + 1] = 0;
	return hexToStringOutput64;
}

char hexToStringOutput32[128];
const char* to_hstring(uint32_t value)
{
	uint32_t* valuePtr = &value;
	uint8_t* ptr;
	uint8_t temp;
	uint8_t size = 4 * 2 - 1;

	for (uint8_t i; i < size; i++)
	{
		ptr = ((uint8_t*)valuePtr + i);
		temp = ((*ptr & 0xF0) >> 4);

		hexToStringOutput32[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');

		temp = ((*ptr & 0x0F));
		hexToStringOutput32[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');
	}

	hexToStringOutput32[size + 1] = 0;
	return hexToStringOutput32;
}

char hexToStringOutput16[128];
const char* to_hstring(uint16_t value)
{
	uint16_t* valuePtr = &value;
	uint8_t* ptr;
	uint8_t temp;
	uint8_t size = 2 * 2 - 1;

	for (uint8_t i; i < size; i++)
	{
		ptr = ((uint8_t*)valuePtr + i);
		temp = ((*ptr & 0xF0) >> 4);

		hexToStringOutput16[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');

		temp = ((*ptr & 0x0F));
		hexToStringOutput16[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');
	}

	hexToStringOutput16[size + 1] = 0;
	return hexToStringOutput16;
} 

char hexToStringOutput8[128];
const char* to_hstring(uint8_t value)
{
	uint8_t* valuePtr = &value;
	uint8_t* ptr;
	uint8_t temp;
	uint8_t size = 1 * 2 - 1;

	for (uint8_t i; i < size; i++)
	{
		ptr = ((uint8_t*)valuePtr + i);
		temp = ((*ptr & 0xF0) >> 4);

		hexToStringOutput8[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');

		temp = ((*ptr & 0x0F));
		hexToStringOutput8[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');
	}

	hexToStringOutput8[size + 1] = 0;
	return hexToStringOutput8;
}