//
// Created by naveen on 30/08/21.
//

#ifndef OPENGL_THECHERNO_VERTEXBUFFERLAYOUT_H
#define OPENGL_THECHERNO_VERTEXBUFFERLAYOUT_H

#include <vector>
#include "GL/glew.h"
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch(type)
		{
			case GL_FLOAT : 		return 4;
			case GL_UNSIGNED_INT : 	return 4;
			case GL_UNSIGNED_BYTE : return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		:m_Stride(0)
	{}


	template<class T>
	void push(unsigned int count)
	{
		//static_assert(false);
	}

	[[nodiscard]] inline const std::vector<VertexBufferElement>& getElements() const { return m_Elements;}
	inline unsigned int getStride() const { return m_Stride; }
};


#endif //OPENGL_THECHERNO_VERTEXBUFFERLAYOUT_H
