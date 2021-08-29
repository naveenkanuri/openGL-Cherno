//
// Created by naveen on 28/08/21.
//

/* Glew is the OpenGL extension wrangler. i.e. it has the openGL function declarations and
 * can link to the gpu drivers to get the functionality*/
/* GLFW is the platform independent library to create a window*/

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if(!(x)) __builtin_trap();
#define glCall(x) glClearError();\
    x;\
    ASSERT(glLogCall(#x, __FILE__, __LINE__))

static void glClearError()
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

static bool glLogCall(const char* function, const char* file, int line)
{
    while(GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") occurred in "
        << function << " at " <<
        file << ": " << line <<std::endl;
        return false;
    }

    return true;
}

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);
    std::string line;
    std::stringstream ss[2];

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT =1
    };
    ShaderType type = ShaderType::NONE;
    while(getline(stream, line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
            {
                //set vertex mode
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment") != std::string::npos)
            {
                // set to fragment mode
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str()};
}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
    /* create a shader of type vertex_shader and give its id */
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    /* So, you created a shader and gave me an id. Good.
     * Now, I will tell where the shader code is
     *
     * 1. the id of the shader, whose source I'm giving you.
     * If you already have a shader source in that id, replace that
     * 2. the number of strings in the array (shader source)
     * 3. the address of the pointer to the shader source string
     * (why can't you just take the pointer to source string, openGL?)
     * 4. If the length is null, the string is assumed to be null terminated
     * Refer http://docs.gl/gl4/glShaderSource for alternatives for 4th argument
     * */
    glCall(glShaderSource(id, 1, &src, nullptr));

    /* Now that you know the shader source string and other details, compile the shader*/
    glCall(glCompileShader(id));

    // error handling before returning the id

    int result;

    /* We are checking whether our shader is successfully compiled or not
     * actual function is glGetShader()
     * Here 'i' stands for id that it needs
     * and 'v' stands for the vector (int pointer) to store the result*/
    glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if(result == GL_FALSE) // our compilation failed
    {
        int length;
        // Get the info log length
        glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        char message[length]; // we are creating this on stack. alternative to create on stack is using alloca
        // char* message = (char*)alloca(length * sizeof(char));

        // fill the log in our message
        glCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to Compile " <<
                                         (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                                         << " shader!" << std::endl;
        std::cout << message << std::endl;

        /* No need of keeper our shader as its compilation failed*/
        glCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

static unsigned int createProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
    /* I want to write a shader program. So create one, and give me its id back*/
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    /* After compiling shaders, attach them to the above created program*/
    glCall(glAttachShader(program, vs));
    glCall(glAttachShader(program, fs));

    /* links the program object specified by program
     * If any shader objects of type GL_VERTEX_SHADER/GL_GEOMETRY_SHADER/GL_FRAGMENT_SHADER are attached to program,
     * they will be used to create an executable that will run on the programmable vertex/geometry/fragment processor respectively
     * */
    glCall(glLinkProgram(program));

    /* checks whether the executables in the program can execute given the current state of the program.
     * For both glLinkProgram and glValidateProgram,
     * the status of the validation operation is stored as part of the program's object state.
     * This will be set to GL_TRUE if validation succeeded and GL_FALSE otherwise. It can be queried by
     * calling glGetProgram() with arguments program and GL_VALIDATE_STATUS*/
    glCall(glValidateProgram(program));

    /* Now that we've linked our shaders to our program, we can flag them for deletion
     * frees the memory and invalidates the name associated with the shader object specified by shader.
     * This command effectively undoes the effects of a call to glCreateShader.*/
    glCall(glDeleteShader(vs));
    glCall(glDeleteShader(fs));

    return program;
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* enable vsync*/
    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK){
        std::cout << "Error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
            -0.5f, -0.5f, // 0
             0.5f, -0.5f, // 1
             0.5f,  0.5f, // 2
             -0.5f, 0.5f, // 3
    };

    unsigned int indices[] = {
            0, 1, 2, // draw first triangle with above indices
            2, 3, 0  // draw second triangle with above indices to make a full square
    };

    unsigned int buffer;
    /* I need 1 buffer. So give me one buffer. And put the address of the generated buffer
     * in the unsigned int $buffer variable so that buffer variable contains the ID of the
     * generated buffer*/
    glCall(glGenBuffers(1, &buffer));

    /* So you gave me a buffer id. Good. Now I am saying that that buffer id will refer to
     * an array of bytes*/
    glCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

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
    glCall(glBufferData(GL_ARRAY_BUFFER, 8* sizeof(float), positions, GL_STATIC_DRAW));

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
    glCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0));

    /* After describing all the above data, I need you to enable the glVertexAttribPointer();
     * Since, I described you about position attribute of my vertex above with index 0 right? (the first argument)
     * so, I need you to enable that.
     * */
    glCall(glEnableVertexAttribArray(0));

    unsigned int ibo; // index buffer
    glCall(glGenBuffers(1, &ibo)); // give me an id for my index buffer

    // bind the index buffer to an element array buffer
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    // my index buffer is of element array type, size is 6 unsigned ints,
    // pointer to my indices array, and hint is draw static
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    // directories are relative to the location of the executable. NOT to the main.cpp
    ShaderProgramSource source = parseShader("../res/shaders/Basic.shader");

    /* Ok now that you've read from shader file, create a shader program for me*/
    unsigned int shaderProgram = createProgram(source.vertexSource, source.fragmentSource);

    /* Now, use that shader program for our triangle*/
    glCall(glUseProgram(shaderProgram));

    /* Each uniform (variable) we write in our shader will get assigned an id (location)
     * So we are asking opengl to give us that id (location) after writing the glUseProgram()
     * 1. the first argument is the shaderProgram which has the uniform
     * 2. the second argument is the name of the uniform (we called our uniform u_Color in our shader)
     * */
    glCall(int location = glGetUniformLocation(shaderProgram, "u_Color"));
    ASSERT(location != -1);

    float r = 0.0f;
    float increment = 0.05f;

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
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Now that we got the location of the uniform (color vec4 in this case),
         * we are setting the value of that color uniform from our cpu
         * we are updating red channel value per draw call
         * */
        glCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        if(r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        // since we are using indices now, we are drawing elements instead of arrays
        // we are drawing triangles, using 6 indices, type unsigned int
        // since we already bound index buffer above as glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)
        // we can pass nullptr to 4th argument
        glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* delete the shader program now that our window is closed and program is about to exit*/
    glCall(glDeleteProgram(shaderProgram));

    glfwTerminate();
    return 0;
}
