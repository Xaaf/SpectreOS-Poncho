#ifndef BASICRENDERER_H
#define BASICRENDERER_H

#include "../Math.h"

#include "Framebuffer.h"
#include "SimpleFonts.h"

class BasicRenderer
{
public:
	Point cursorPosition;
	Framebuffer* targetFramebuffer;
	PSF1_FONT* psf1_font;

	unsigned int colour;

public:
	BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_font);

	void PutChar(char chr, unsigned int xOffset, unsigned int yOffset);
	void Print(const char* str);
};

#endif // BASICRENDERER_H