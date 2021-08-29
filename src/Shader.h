//
// Created by naveen on 30/08/21.
//

#ifndef OPENGL_THECHERNO_SHADER_H
#define OPENGL_THECHERNO_SHADER_H

#include <string>
#include <unordered_map>

struct ShaderProgramSource;

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_filepath;
	// caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unBind() const;

	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniform1i(const std::string& name, int value);

private:
	int getUniformLocation(const std::string& name);
	unsigned int createProgram(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int compileShader(unsigned int type, const std::string& source);
	ShaderProgramSource parseShader(const std::string& filePath);


};


#endif //OPENGL_THECHERNO_SHADER_H
