//
// Created by naveen on 28/08/21.
//

/* Glew is the OpenGL extension wrangler. i.e. it has the openGL function declarations and
 * can link to the gpu drivers to get the functionality*/
/* GLFW is the platform independent library to create a window*/

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

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

	Shader shader("../res/shaders/Basic.shader");
	shader.bind();
	shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    /* Reset all our bindings*/
	va.unBind();
	vb.unBind();
	ib.unBind();
	shader.unBind();

	Renderer renderer;

    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
		renderer.clear();

		shader.bind();
		/* Now that we got the location of the uniform (color vec4 in this case),
		 * we are setting the value of that color uniform from our cpu
		 * we are updating red channel value per draw call
		 * */
		shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

		renderer.draw(va, ib, shader);

        if(r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
