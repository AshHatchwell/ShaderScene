#include "ShaderProgram.h"
#include "Shader.h"

#include "gl_core_4_4.h"

#include <iostream>


ShaderProgram::~ShaderProgram() {
	delete[] m_error;
	glDeleteProgram(m_programHandle);

	for (int i = 0; i < 5; ++i) {
		if (m_shaders[i] == nullptr)
			continue;
		delete m_shaders[i];
	}
}

bool ShaderProgram::setShader(int type, Shader* shader) {
	if (shader == nullptr)
		return false;
	if (shader->getType() != type) {
		// Wrong type of shader specified!
		return false;
	}

	switch (type) {
	case GL_VERTEX_SHADER:
		m_shaders[0] = shader;
		break;
	case GL_TESS_CONTROL_SHADER:
		m_shaders[1] = shader;
		break;
	case GL_TESS_EVALUATION_SHADER:
		m_shaders[2] = shader;
		break;
	case GL_GEOMETRY_SHADER:
		m_shaders[3] = shader;
		break;
	case GL_FRAGMENT_SHADER:
		m_shaders[4] = shader;
		break;
	default:
		// Invalid shader type!
		return false;
	}

	int err = GL_NO_ERROR;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << err << std::endl;
	}

	return true;
}

bool ShaderProgram::link() {
	// Create program and attach shaders
	m_programHandle = glCreateProgram();
	for (int i = 0; i < 5; ++i) {
		if (m_shaders[i] == nullptr)
			continue;
		// Handle and type
		glAttachShader(m_programHandle, m_shaders[i]->getHandle());
	}

	glLinkProgram(m_programHandle);

	// Check that it linked correctly
	int success = GL_TRUE;
	glGetProgramiv(m_programHandle, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {

		GLint logLength = 0;
		glGetProgramiv(m_programHandle, GL_INFO_LOG_LENGTH, &logLength);

		delete[] m_error;
		m_error = new char[logLength];
		glGetProgramInfoLog(m_programHandle, logLength, 0, m_error);
		std::cout << m_error << std::endl;

		int err = GL_NO_ERROR;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << err << std::endl;
		}

		return false;
	}

	int err = GL_NO_ERROR;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << err << std::endl;
	}

	return true;
}

int ShaderProgram::getUniformHandle(const char* name) {
	return glGetUniformLocation(m_programHandle, name);
}

UniformType ShaderProgram::getUniformType(char* name) {
	try {
		return m_uniforms.at(name);
	}
	catch (const std::out_of_range& oorerr) {
		// Uniform type can* never be set to UNIFORM_END, so works as an invalid type
		// *at least it shouldn't happen
		return UniformType::UNIFORM_END;
	}
}

void ShaderProgram::bind() {
	glUseProgram(m_programHandle);
}

void ShaderProgram::setUniform(char* uniformName, UniformType type) {
	if (type <= UNIFORM_START || type >= UNIFORM_END) {
		std::cout << "Tried to set invalid uniform type for " << uniformName << "!" << std::endl;
		return;
	}
	m_uniforms[uniformName] = type;
}