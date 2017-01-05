#pragma once

// Shader.h
// Loads in a single shader file. Can be added to a program
// and used to render a mesh, as long as a vertex and fragement
// shader are present.

class Shader {
public:

	Shader() : m_shaderHandle(0), m_type(0), m_error(nullptr) {}
	~Shader();

	int loadShader(unsigned int type, const char* fileName);

	unsigned int getHandle() const	{ return m_shaderHandle; }

	unsigned int getType() const	{ return m_type; }

	const char*  getError() const	{ return m_error; }

private:

	// Shader handle
	unsigned int	m_shaderHandle;
	// vertex, tess control, tess eval, geometry, fragment
	unsigned int	m_type;
	char*			m_error;
};