//
// Created by naveen on 30/08/21.
//

#ifndef OPENGL_THECHERNO_INDEXBUFFER_H
#define OPENGL_THECHERNO_INDEXBUFFER_H


class IndexBuffer
{
private:
    unsigned int m_Renderer_ID;
    unsigned int m_Count; // number of indices the index buffer has
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unBind() const;

	inline unsigned int getCount() const { return m_Count; }
};


#endif //OPENGL_THECHERNO_INDEXBUFFER_H
