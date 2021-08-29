//
// Created by naveen on 30/08/21.
//

#ifndef OPENGL_THECHERNO_VERTEXBUFFER_H
#define OPENGL_THECHERNO_VERTEXBUFFER_H

class VertexBuffer
{
private:
    unsigned int m_Renderer_ID;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unBind() const;
};


#endif //OPENGL_THECHERNO_VERTEXBUFFER_H
