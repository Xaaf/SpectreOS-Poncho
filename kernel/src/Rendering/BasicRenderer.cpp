#include "BasicRenderer.h"

BasicRenderer::BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_font)
{
	this->targetFramebuffer = targetFramebuffer;
	this->psf1_font = psf1_font;
	this->colour = 0xffffffff;
	this->cursorPosition = { 0, 0 };
}

void BasicRenderer::PutChar(char chr, unsigned int xOffset, unsigned int yOffset)
{
	unsigned int* pixelPtr = (unsigned int*)targetFramebuffer->baseAddress;
	char* fontPtr = (char*)psf1_font->glyphBuffer + (chr * psf1_font->psf1_header->charsize);

	for (unsigned long y = yOffset; y < yOffset + 16; y++)
	{
		for (unsigned long x = xOffset; x < xOffset + 8; x++)
		{
			if ((*fontPtr & (0b10000000 >> (x - xOffset))) > 0)
			{
				*(unsigned int*)(pixelPtr + x + (y * targetFramebuffer->pixelsPerScanline)) = colour;
			}
		}

		fontPtr++;
	}
}

void BasicRenderer::Print(const char* str)
{
	char* chr = (char*)str;

	while (*chr != 0)
	{
		PutChar(*chr, cursorPosition.x, cursorPosition.y);
		cursorPosition.x += 8;

		if (cursorPosition.x + 8 > targetFramebuffer->width)
		{
			cursorPosition.x = 0;
			cursorPosition.y += 16;
		}

		chr++;
	}
}