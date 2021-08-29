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
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"



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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

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

    /* Generating Vertex Array Object and binding is mandatory in GL_CORE_PROFILE */
	VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
	VertexBufferLayout layout;
	layout.push<float>(2);
	va.addBuffer(vb, layout);


	IndexBuffer ib(indices, 6);

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

    /* Reset all our bindings*/
	va.unBind();
    glCall(glUseProgram(0));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

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

        glCall(glUseProgram(shaderProgram));
        /* Now that we got the location of the uniform (color vec4 in this case),
         * we are setting the value of that color uniform from our cpu
         * we are updating red channel value per draw call
         * */
        glCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        /* After resetting all our bindings, we just need to bind our vao
         * binding vertex buffer and setting up its layout becomes binding the vertex array object because
         * this vao contains all the state we actually need
         *
         * when we bind a vao, a vertex buffer and then specify the layout of vertex array
         * in glVertexAttribPointer(), vao gets linked to vertex buffer and the layout.
         * so calling glBindVertexArray(vao) is enough here
         * */
		va.bind();
		ib.bind();
        if(r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        // since we are using indices now, we are drawing elements instead of arrays
        // we are drawing triangles, using 6 indices, type unsigned int
        // since we already bound index buffer above as glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib)
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
