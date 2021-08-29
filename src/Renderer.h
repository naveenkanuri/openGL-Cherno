//
// Created by naveen on 30/08/21.
//

#ifndef OPENGL_THECHERNO_RENDERER_H
#define OPENGL_THECHERNO_RENDERER_H

#include <GL/glew.h>

#define ASSERT(x) if(!(x)) __builtin_trap();
#define glCall(x) glClearError();\
    x;\
    ASSERT(glLogCall(#x, __FILE__, __LINE__))

void glClearError();
bool glLogCall(const char* function, const char* file, int line);

class Renderer {

};


#endif //OPENGL_THECHERNO_RENDERER_H
