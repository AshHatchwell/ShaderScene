#pragma once
#include <glm\mat4x4.hpp>
#include <map>
#include "ShaderProgram.h"
#include <string>

// WorldObject.h
// A class that contains a pointer to a mesh, and a shader program and transform
// with which to render it. Each contains individual

class Mesh;
class ShaderProgram;

class WorldObject {
public:
	WorldObject() : m_updateFunction(nullptr), m_mesh(nullptr), m_shaderProgram(nullptr), m_name("NewObject"),
    m_transform(1.f) {}
	virtual ~WorldObject() {}

	// TODO: Implement unique update functions for different WorldObjects.
	// Probably an awful way of doing this but I wanted to use function pointers for once :s
	//void update(float deltaTime) { if (m_updateFunction != nullptr) m_updateFunction(deltaTime); }
	//void setUpdateFunction(void(*updateFunction)(float deltaTime)) { m_updateFunction = updateFunction; }


	Mesh* getMesh() { return m_mesh; }
	ShaderProgram* getShaderProgram() { return m_shaderProgram; }
    // Get the data associated with a particular uniform
    void* getUniformData(char* uniformName);
	//const std::map<char*, void*>* getUniformDataMap() { return &m_uniformData; }
    // TODO: Const to prevent accidental changes
    glm::mat4* getTransform() { return &m_transform; }


	void setMesh(Mesh* newMesh);
	// Will reset m_uniformData!
	void setShaderProgram(ShaderProgram* newShaderProgram);
	// Sets the data to be attached to each uniform in the shaderprogram for this worldobject
	void setUniformData(char* uniformName, UniformType uniformType, void* uniformData);

protected:

	Mesh* m_mesh;
	ShaderProgram* m_shaderProgram;

	glm::mat4x4 m_transform;

	// Instances of data to fill shader uniforms
	// TODO: Should we have a unique one for each shape in the mesh?
    // TODO: Add boolean for if it should be refreshed each frame.
	std::map<char*, void*> m_uniformData;

	// TODO
	void (*m_updateFunction)(float deltaTime);

	std::string m_name;

private:

};