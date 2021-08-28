//
// Created by naveen on 28/08/21.
//

/* Glew is the OpenGL extension wrangler. i.e. it has the openGL function declarations and
 * can link to the gpu drivers to get the functionality*/
#include "GL/glew.h"
/* GLFW is the platform independent library to create a window*/
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        std::cout << "Error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] = {
            -0.5f, -0.5f,
             0.f,   0.5f,
             0.5f, -0.5f
    };

    unsigned int buffer;
    /* I need 1 buffer. So give me one buffer. And put the address of the generated buffer
     * in the unsigned int $buffer variable so that buffer variable contains the ID of the
     * generated buffer*/
    glGenBuffers(1, &buffer);

    /* So you gave me a buffer id. Good. Now I am saying that that buffer id will refer to
     * an array of bytes*/
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

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
    glBufferData(GL_ARRAY_BUFFER, 6* sizeof(float), positions, GL_STATIC_DRAW);

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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);

    /* After describing all the above data, I need you to enable the glVertexAttribPointer();
     * Since, I described you about position attribute of my vertex above with index 0 right? (the first argument)
     * so, I need you to enable that.
     * */
    glEnableVertexAttribArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Refer docs.gl
         * Well, I need you to draw something I want on the screen
         * First I need you to draw from an Array. Which array you might ask?
         * The one that I bound to you by calling glBindBuffer().
         * I know that I can call glBindBuffer() multiple times in the code. So pick the currently bound one.
         *
         * Now that you got the array of bytes, I want you to draw them as triangles
         * The triangle's starting index is 0 and the number of indices to be rendered is 3
         * */
        glDrawArrays(GL_TRIANGLES, 0, 3);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}