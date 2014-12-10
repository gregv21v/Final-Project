#pragma once
#include <vector>
#include <unordered_map>
#include <string>

#include "Texture.h"


class TextureManager
{
public:

	~TextureManager();

	void add(std::string location, std::string name);
	void loadAll();
	Texture * get(std::string name);

private:
	std::unordered_map<std::string, Texture *> _textures;

};

