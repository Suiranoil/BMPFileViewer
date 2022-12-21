#pragma once

#include <string>

#include "Pixel.h"

class Image
{
private:
	uint32_t m_Width, m_Height;
	Pixel* m_Pixels;

	std::string m_Path;

public:
	explicit Image(std::string_view path);
	Image(uint32_t width, uint32_t height);
	Image(const Image& other);
	~Image();

	Image& operator=(const Image& other);

	void Save(std::string_view path) const;

	[[nodiscard]] const Pixel& GetPixel(uint32_t x, uint32_t y) const { return m_Pixels[y * m_Width + x]; }
	void SetPixel(uint32_t x, uint32_t y, const Pixel& pixel) { m_Pixels[y * m_Width + x] = pixel; }

	void Fill(const Pixel& pixel);

	[[nodiscard]] uint32_t GetWidth() const { return m_Width; }
	[[nodiscard]] uint32_t GetHeight() const { return m_Height; }
	[[nodiscard]] const Pixel* GetPixels() const { return m_Pixels; }

	[[nodiscard]] const std::string& GetPath() const { return m_Path; }
};
