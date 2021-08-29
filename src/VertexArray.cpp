//
// Created by naveen on 30/08/21.
//

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	glCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	glCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	bind(); // bind vertex array
	vb.bind(); // bind buffer array
	const auto& elements = layout.getElements(); // bind layouts
	unsigned int offset = 0;
	for(unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		/* A little intro to a vertex:
		* A vertex can have multiple attributes like position, texture coordinates, normals etc.
		* So for each attribute, I will have to call glVertexAttribPointer() describing that vertex's attribute
		* 1. The first argument is the index of the attribute in that vertex.
		* Since this example deals with vertex having only position attribute, this is 0
		* 2. The second argument is the size of the attribute. NOT IN BYTES BUT IN COUNT.
		* In our example, each position attribute has 2 elements. (say pos x and y). So this is 2
		* 3. The third argument is asking about the type of the data in the attribute.
		* In our example, clearly this is float type. So this is GL_FLOAT
		* 4. The fourth argument is asking whether to normalize this data
		* In our example, we are already using floats that are normalized. So this is GL_FALSE
		* We will use this if our data is say color attribute and ranges b/w 0 and 255, and
		* we want OpenGL to normalize our data
		* 5. The fifth argument is asking about the stride of each vertex.
		* Starting from 0, how many bytes I have to move to get to the next vertex's same attribute?
		* Remember that a vertex has many attributes and if so,
		* we have to call glVertexAttribPointer() multiple times with first argument 0 for first attribute, 1 for second attribute and so on
		* In our example, our vertex has only one attribute i.e. position, and
		* we have to travel 2 * float size to get to the next vertex's position attribute.
		* 6. The sixth argument is asking about the offset of the attribute in this vertex
		* i.e. how many bytes I have to travel from the beginning of each vertex to get to the
		* data of this attribute?
		* For e.g. say, if I have a vertex with 3 attributes:
		* position (3 floats = 12 bytes), texture coordinates (2 floats = 8 bytes) and normals (3 floats = 12 bytes)
		* and I'm describing texture coordinates in glVertexAttribPointer()
		* Obviously, the first argument will be 1 (as position's will be 0)
		* the stride in fifth argument will be 32 (12 + 8 + 12) because I have to travel 32 bytes again to get to
		* the next vertex's texture coordinates again
		* the offset in sixth argument will be 12. I have to travel 12 bytes from each vertex's start to get to
		* the same vertex's texture coordinates attribute. (because the first 12 bytes are taken by vertex's position attribute)
		* */

		/* Phew, that was a mouthful. Since this vertex has only one attribute i.e. position,
		 * 1. index of the attribute is 0,
		 * 2. count is 2 (each vertex has 2 elements pos x and y)
		 * 3. they are of type float
		 * 4. they are already normalized, so false
		 * 5. to go to next vertex's position attribute you have to travel 8 bytes (2 * float)
		 * 6. the offset of position attribute from the vertex's beginning in this vertex is 0
		 */
		glCall(glVertexAttribPointer(i, element.count, element.type,
				element.normalized, layout.getStride(), reinterpret_cast<const void*>(offset)));

		offset+= element.count * VertexBufferElement::getSizeOfType(element.type);

		/* After describing all the above data, I need you to enable the glVertexAttribPointer();
		 * Since, I described you about position attribute of my vertex above with index 0 right? (the first argument)
		 * so, I need you to enable that.
		 * */
		glCall(glEnableVertexAttribArray(i));
	}

}

void VertexArray::bind() const
{
	glCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unBind() const
{
	glCall(glBindVertexArray(0));
}
