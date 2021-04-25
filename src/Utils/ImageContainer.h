#pragma once

#include <cstdint>
#include <string>

class ImageRGBA
{
public:
	ImageRGBA(const std::string& filename);
	
	int width{};
	int height{};

	char* GetDataPointer() { return m_data; }
private:

	char* m_data;
	size_t m_size;
};