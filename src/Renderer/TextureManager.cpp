#include "TextureManager.h"
#include "Utils/stringHelper.h"

#include <fstream>

#include "nlohmann/json.hpp"
#include "gl/glew.h"


using json = nlohmann::json;

TextureManager::TextureManager(const std::string& atlasPath, const std::string& metadatapath)
{
	LoadAtlas(atlasPath);

	CreateGLTexture();

	LoadMetadata(metadatapath);
}

std::array<vec2, 4>& TextureManager::GetTexCoords(const std::string& spriteName)
{
	assert(m_texMapping.find(spriteName) != m_texMapping.end());

	if (m_texMapping.find(spriteName) == m_texMapping.end())
		return std::array<vec2, 4>();

	return m_texMapping[spriteName];
}

void TextureManager::CreateGLTexture()
{
	glGenTextures(1, &m_texture);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_texAtlas->width, m_texAtlas->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texAtlas->GetDataPointer());

}

void TextureManager::LoadAtlas(const std::string filename)
{
	m_texAtlas = std::make_unique<ImageRGBA>(filename);
}

void TextureManager::LoadMetadata(const std::string filename)
{
	std::ifstream stream(filename);
	
	json metadata;
	stream >> metadata;
	
	auto totalSize = metadata["meta"]["size"];
	float totalWidth = totalSize["w"];
	float totalHeight = totalSize["h"];

	for (auto& sprite : metadata["frames"].items())
	{
		//std::cout << sprite.key() << std::endl;

		std::string spriteName = sprite.key();
		stripFileEnding(spriteName);

		auto& fileCoords = sprite.value()["frame"];

		std::array<vec2, 4> coords;

		float x = fileCoords["x"];
		float y = fileCoords["y"];
		float w = fileCoords["w"];
		float h = fileCoords["h"];

		coords[0].x = x / totalWidth;
		coords[0].y = y / totalHeight;

		coords[1].x = (x + w) / totalWidth;
		coords[1].y = y / totalHeight;

		coords[2].x = (x + w) / totalWidth;
		coords[2].y = (y + h) / totalHeight;

		coords[3].x = x / totalWidth;
		coords[3].y = (y + h) / totalHeight;

		
		m_texMapping.insert({ spriteName, coords });
	}

}
