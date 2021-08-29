//
// Created by naveen on 30/08/21.
//

#include "Renderer.h"
#include <iostream>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

void glClearError()
{
    /* To allow for distributed implementations, there may be several error flags.
     * If any single error flag has recorded an error, the value of that flag is returned
     * and that flag is reset to GL_NO_ERROR when glGetError is called.
     * If more than one flag has recorded an error, glGetError returns and clears an arbitrary error flag value.
     * Thus, glGetError should always be called in a loop,
     * until it returns GL_NO_ERROR, if all error flags are to be reset.
     * */
    while (glGetError() != GL_NO_ERROR); // reset all error flags
}

bool glLogCall(const char* function, const char* file, int line)
{
    while(GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (0x0" << std::hex << error << std::dec << ") occurred in "
                  << function << " at " <<
                  file << ": " << line <<std::endl;
        return false;
    }

    return true;
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	/* After resetting all our bindings, we just need to bind our vao
	 * binding vertex buffer and setting up its layout becomes binding the vertex array object because
	 * this vao contains all the state we actually need
	 *
	 * when we bind a vao, a vertex buffer and then specify the layout of vertex array
	 * in glVertexAttribPointer(), vao gets linked to vertex buffer and the layout.
	 * so calling glBindVertexArray(vao) is enough here
	 * */
	shader.bind();
	va.bind();
	ib.bind();

	// since we are using indices now, we are drawing elements instead of arrays
	// we are drawing triangles, using 6 indices, type unsigned int
	// since we already bound index buffer above as glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib)
	// we can pass nullptr to 4th argument
	glCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const
{
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT);
}
