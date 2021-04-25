#pragma once

#include <map>
#include <array>
#include "Utils/math.h"

#include "Utils/ImageContainer.h"
#include "Components/gameComponents.h"

using NameToTexCoords = std::map<std::string, std::array<vec2, 4>>;


class TextureManager
{
public:
	TextureManager(const std::string& atlasPath, const std::string& metadatapath);
	
	std::array<vec2, 4>& GetTexCoords(const std::string& spriteName);
	// also what is the 
private:
	void CreateGLTexture();
	void LoadAtlas(const std::string filename);
	void LoadMetadata(const std::string filename);
	unsigned int m_texture;
	std::unique_ptr<ImageRGBA> m_texAtlas;
	NameToTexCoords m_texMapping;
};