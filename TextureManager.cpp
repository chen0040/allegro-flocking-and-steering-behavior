#include "TextureManager.h"

TextureManager::~TextureManager()
{
	std::map<std::string, BITMAP*>::iterator pos;
	for(pos=m_textures.begin(); pos!=m_textures.end(); ++pos)
	{
		destroy_bitmap(pos->second);
	}
}

TextureManager* TextureManager::GetInstance()
{
	static TextureManager theInstance;
	return &theInstance;
}

BITMAP* TextureManager::LoadTexture(std::string fname)
{
	std::map<std::string, BITMAP*>::iterator pos=m_textures.find(fname);
	if(pos != m_textures.end())
	{
		return pos->second;
	}
	BITMAP* pTexture=load_bitmap(fname.c_str(), NULL);
	m_textures[fname]=pTexture;  

	return pTexture;
}

BITMAP* TextureManager::GetTexture(std::string fname)
{
	return LoadTexture(fname);
}

bool TextureManager::IsTextureLoaded(std::string fname)
{
	
	std::map<std::string, BITMAP*>::iterator pos=m_textures.find(fname);
	if(pos != m_textures.end())
	{
		return true;
	}
	return false;
}