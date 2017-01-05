#include "Shader.h"

#include "gl_core_4_4.h"

#include <stdio.h>
#include <iostream>

Shader::~Shader() {
	if (m_error != nullptr)
		delete[] m_error;
	glDeleteShader(m_shaderHandle);
}

int Shader::loadShader(unsigned int type, const char* fileName) {

	switch (type) {
	case GL_VERTEX_SHADER:
		break;
	case GL_TESS_CONTROL_SHADER:
		break;
	case GL_TESS_EVALUATION_SHADER:
		break;
	case GL_GEOMETRY_SHADER:
		break;
	case GL_FRAGMENT_SHADER:
		break;
	default:
		// Invalid type!
		return 1;
	}

	m_type = type;

	// Create shader handle
	m_shaderHandle = glCreateShader(type);

	// Open the file
	FILE* file = nullptr;
	if (fopen_s(&file, fileName, "rb") != 0) {
		// Failed to open file
		glDeleteShader(m_shaderHandle);
		return 2;
	}
	// Get size
	fseek(file, 0, SEEK_END);
	unsigned int size = ftell(file);
	if (size == 0) {
		fclose(file);
		glDeleteShader(m_shaderHandle);
		return 3;
	}
	// Create array and read
	char* source = new char[size + 1];
	fseek(file, 0, SEEK_SET);
	fread_s(source, size + 1, sizeof(char), size, file);
	fclose(file);
	// Append null terminator
	source[size] = '\0';


	// Create the shader
	glShaderSource(m_shaderHandle, 1, (const char**)&source, 0);
	glCompileShader(m_shaderHandle);

	delete[] source;

	// Check if compiling was successful
	int success = GL_TRUE;
	glGetShaderiv(m_shaderHandle, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		int logLength = 0;
		glGetShaderiv(m_shaderHandle, GL_INFO_LOG_LENGTH, &logLength);

		delete[] m_error;
		m_error = new char[logLength];
		glGetShaderInfoLog(m_shaderHandle, logLength, 0, m_error);
		std::cout << m_error << std::endl;
		return 4;
	}

	int err = GL_NO_ERROR;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << err << std::endl;
	}

	return 0;
}