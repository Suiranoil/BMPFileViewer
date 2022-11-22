#pragma once

#include <cstdint>

struct Pixel
{
	uint8_t R, G, B, A;

	Pixel() = default;
	Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	bool operator==(const Pixel& other) const;
	bool operator!=(const Pixel& other) const;
	uint8_t& operator[](uint32_t index);
};
