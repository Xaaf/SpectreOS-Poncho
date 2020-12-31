#ifndef BITMAP_H
#define BITMAP_H

#include <stddef.h>
#include <stdint.h>

class Bitmap
{
public:
	size_t size;
	uint8_t* buffer;
	bool operator[](uint64_t index);

public:
	void Set(uint64_t index, bool value);
};

#endif // BITMAP_H