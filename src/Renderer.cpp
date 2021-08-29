//
// Created by naveen on 30/08/21.
//

#include "Renderer.h"
#include <iostream>

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
