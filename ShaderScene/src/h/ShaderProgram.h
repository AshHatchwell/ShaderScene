#pragma once
#include <map>

// ShaderProgram.h
// A class that can contains a series of shaders, and can be used to render a mesh.
// All uniforms MUST be assigned using setUniform() or terrible things will happen!

class Shader;

enum UniformType {
	UNIFORM_START,
	UNIFORM_MAT4,
	UNIFORM_FLOAT,
    UNIFORM_VEC4,
	UNIFORM_END,
};

struct UniformData {
	UniformType type;
	void* object;
};

class ShaderProgram {
public:

	ShaderProgram() : m_programHandle(0), m_error(nullptr) {
		m_shaders[0] = m_shaders[1] = m_shaders[2] = m_shaders[3] = m_shaders[4] = nullptr;
	}
	~ShaderProgram();

	bool setShader(int type, Shader* shader);

	bool link();
	
	void bind();

	void setUniform(char* uniformName, UniformType type);

	int getUniformHandle(const char* name);

	UniformType getUniformType(char* name);
	
    const std::map<char*, UniformType>* getUniformMap() { return &m_uniforms; }

	unsigned int getProgramHandle() const { return m_programHandle; }

private:

	// vertex = 0, tess control = 1, tess eval = 2, geometry = 3, fragment = 4
	Shader*			m_shaders[5];
	unsigned int	m_programHandle;
	char*			m_error;

	// For tracking and setting uniforms
	std::map<char*, UniformType> m_uniforms;
};