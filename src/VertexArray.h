//
// Created by naveen on 30/08/21.
//

#ifndef OPENGL_THECHERNO_VERTEXARRAY_H
#define OPENGL_THECHERNO_VERTEXARRAY_H

class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void bind() const;
	void unBind() const;
};


#endif //OPENGL_THECHERNO_VERTEXARRAY_H
