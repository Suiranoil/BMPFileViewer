#include "Pixel.h"

Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	this->R = r;
	this->G = g;
	this->B = b;
	this->A = a;
}

bool Pixel::operator==(const Pixel& other) const
{
	return R == other.R && G == other.G && B == other.B && A == other.A;
}

bool Pixel::operator!=(const Pixel& other) const
{
	return !(*this == other);
}

uint8_t& Pixel::operator[](uint32_t index)
{
	switch (index)
	{
	case 0:
		return R;
	case 1:
		return G;
	case 2:
		return B;
	case 3:
		return A;
	default:
		return R;
	}
}
