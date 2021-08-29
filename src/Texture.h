//
// Created by naveen on 30/08/21.
//

#ifndef OPENGL_THECHERNO_TEXTURE_H
#define OPENGL_THECHERNO_TEXTURE_H

#include "Renderer.h"
#include <string>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& filePath);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unBind() const;

	inline int getWidth() const { return m_Width;}
	inline int getHeight() const { return m_Height;}
};


#endif //OPENGL_THECHERNO_TEXTURE_H
