#ifndef _H_TEXTURE_MANAGER_H
#define _H_TEXTURE_MANAGER_H

#include <allegro.h>
#include <winalleg.h>
#include <map>
#include <string>

class TextureManager
{
public:
	virtual ~TextureManager();
	static TextureManager* GetInstance();

private:
	TextureManager() { }
	TextureManager(const TextureManager& rhs) { }
	TextureManager& operator=(const TextureManager& rhs) { return *this; }

public:
	BITMAP* GetTexture(std::string fname);
	BITMAP* LoadTexture(std::string fname);
	bool IsTextureLoaded(std::string fname);

private:
	std::map<std::string, BITMAP*> m_textures;
};

#define TextureEngine TextureManager::GetInstance()

#endif