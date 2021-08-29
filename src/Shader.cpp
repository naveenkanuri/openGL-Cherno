//
// Created by naveen on 30/08/21.
//

#include "Shader.h"
#include "Renderer.h"
#include <fstream>
#include <sstream>
#include <iostream>

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

Shader::Shader(const std::string& filepath)
	:m_filepath(filepath), m_RendererID(0)
{
	// directories are relative to the location of the executable. NOT to the main.cpp
	ShaderProgramSource source = parseShader(filepath);

	/* Ok now that you've read from shader file, create a shader program for me*/
	m_RendererID = createProgram(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	/* delete the shader program now that our window is closed and program is about to exit*/
	glCall(glDeleteProgram(m_RendererID));
}

void Shader::bind() const
{
	glCall(glUseProgram(m_RendererID));
}

void Shader::unBind() const
{
	glCall(glUseProgram(0));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

int Shader::getUniformLocation(const std::string& name)
{
	if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	/* Each uniform (variable) we write in our shader will get assigned an id (location)
	 * So we are asking opengl to give us that id (location) after writing the glUseProgram()
	 * 1. the first argument is the shaderProgram which has the uniform
	 * 2. the second argument is the name of the uniform (we called our uniform u_Color in our shader)
	 * */
	glCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if(location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::parseShader(const std::string& filePath)
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

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
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

unsigned int Shader::createProgram(const std::string& vertexShader, const std::string& fragmentShader)
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