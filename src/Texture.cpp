//
// Created by naveen on 30/08/21.
//

#include "Texture.h"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& filePath)
	: m_RendererID(0), m_FilePath(filePath),
	m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	/* OpenGL's coordinates start from bottom left. As our texture stores data from top, we need to flip it*/
	stbi_set_flip_vertically_on_load(1);

	/* Gets the texture data to our local buffer*/
	m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	glCall(glGenTextures(1, &m_RendererID));
	glCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	/* these four are necessary to resize/clamp our texture. if we don't write these, we may see a black texture*/
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));


	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));

	if(m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	glCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::bind(unsigned int slot) const
{
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::unBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
