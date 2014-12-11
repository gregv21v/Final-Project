#include "TextureManager.h"

TextureManager::~TextureManager()
{
	for (auto it = _textures.begin(); it != _textures.end(); ++it)
	{
		delete it->second;
	}
}


void TextureManager::add(std::string location, std::string name)
{
	Texture * texture = new Texture();
	texture->loadFromFile(location);
	_textures.emplace(name, texture);
}


void TextureManager::loadAll()
{
	for (auto it = _textures.begin(); it != _textures.end(); ++it)
	{
		it->second->load();
	}
}


Texture * TextureManager::get(std::string name)
{
	return _textures[name];
}