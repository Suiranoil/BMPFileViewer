#include "Image.h"

#include <stb_image.h>

#include "stb_image_write.h"

Image::Image(std::string_view path)
	: m_Path(path)
{
	int width, height, channels;
	uint8_t* data = stbi_load(path.data(), &width, &height, &channels, 4);

	m_Width = width;
	m_Height = height;
	m_Pixels = new Pixel[(size_t)m_Width * m_Height];

	std::memcpy(m_Pixels, data, (size_t)m_Width * m_Height * sizeof(Pixel));

	stbi_image_free(data);
}

Image::Image(uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height)
{
	m_Pixels = new Pixel[(size_t)m_Width * m_Height];
	m_Path = "";
}

Image::Image(const Image& other)
{
	m_Width = other.m_Width;
	m_Height = other.m_Height;
	m_Pixels = new Pixel[(size_t)m_Width * m_Height];
	std::memcpy(m_Pixels, other.m_Pixels, (size_t)m_Width * m_Height * sizeof(Pixel));
}

Image& Image::operator=(const Image& other)
{
	if (this == &other)
		return *this;

	delete[] m_Pixels;

	m_Width = other.m_Width;
	m_Height = other.m_Height;
	m_Pixels = new Pixel[(size_t)m_Width * m_Height];
	std::memcpy(m_Pixels, other.m_Pixels, (size_t)m_Width * m_Height * sizeof(Pixel));

	return *this;
}

void Image::Save(std::string_view path) const
{
	stbi_write_bmp(path.data(), m_Width, m_Height, 4, m_Pixels);
}

void Image::Fill(const Pixel& pixel)
{
	for (uint32_t i = 0; i < m_Width * m_Height; i++)
		m_Pixels[i] = pixel;
}

Image::~Image()
{
	delete[] m_Pixels;
}
