//
// Created by naveen on 30/08/21.
//

#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    glCall(glGenBuffers(1, &m_Renderer_ID)); // give me an id for my index buffer

    // bind the index buffer to an element array buffer
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_ID));
    // my index buffer is of element array type, size is 6 unsigned ints,
    // pointer to my indices array, and hint is draw static
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	glCall(glDeleteBuffers(1, &m_Renderer_ID));
}

void IndexBuffer::bind() const
{
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_ID));
}

void IndexBuffer::unBind() const
{
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
