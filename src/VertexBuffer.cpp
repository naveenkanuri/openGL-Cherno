//
// Created by naveen on 30/08/21.
//

#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    /* I need 1 buffer. So give me one buffer. And put the address of the generated buffer
     * in the unsigned int m_Renderer_ID variable so that buffer variable contains the ID of the
     * generated buffer*/
    glCall(glGenBuffers(1, &m_Renderer_ID));
    /* So you gave me a buffer id. Good. Now I am saying that that buffer id will refer to
     * an array of bytes*/
    glCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID));
    /* Ok, as you know that buffer id refers to an array of bytes, I will tell you
     * what the actual size of that buffer is. I know that I need not tell you right away and
     * that I can update the data later, but I already know that I'm interested in
     * drawing a triangle first, I will tell you about the data.
     *
     * So, Refer http://docs.gl/gl4/glBufferData for documentation
     * 1. the buffer data is Array of bytes
     * 2. the size of data is 6 times sizeof float because my positions (of triangle) array has 6 floats
     * 3. the pointer to the actual data is the variable positions
     * 4. I'm hinting you that the draw type is static.
     * i.e., I'm not changing the data, but you have to draw the same data every loop.
     * */
    // notice the memory improvement by using indices.
    // we now need only 4 vertices (8 floats) instead of 6 (12 floats)
    glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    glCall(glDeleteBuffers(1, &m_Renderer_ID));
}

void VertexBuffer::bind() const
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID));
}

void VertexBuffer::unBind() const
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
