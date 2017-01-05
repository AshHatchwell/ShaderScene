#include "WorldObject.h"

#include <iostream>

void WorldObject::setMesh(Mesh* newMesh) {
    if (newMesh != nullptr)
        m_mesh = newMesh;
}

void WorldObject::setShaderProgram(ShaderProgram* newShaderProgram) {
    if (newShaderProgram == nullptr)
        return;
    m_shaderProgram = newShaderProgram;
    m_uniformData.clear();
}

void WorldObject::setUniformData(char* uniformName, UniformType uniformType, void* uniformData) {
    // Check type first - shouldn't this be completed with the later check anyway?
    if (uniformType <= UniformType::UNIFORM_START || uniformType >= UniformType::UNIFORM_END) {
        std::cout << "Invalid uniform type for uniform: " << uniformName << " on object: " << m_name.c_str() << "!" << std::endl;
        return;
    }

    // Check that the uniform exists in our shader program and is the correct type
    if (m_shaderProgram == nullptr) {
        std::cout << "No shader set for object: " << m_name.c_str() << "!" << std::endl;
        return;
    }

    // Make sure the types match
    if (uniformType != m_shaderProgram->getUniformType(uniformName)) {
        std::cout << "Uniform types do not match for uniform: " << uniformName << " on object: " << m_name.c_str() << "!" << std::endl;
        return;
    }

    // Don't do anything if the data is null
    // TODO: Handle null for each type to allow setting to default
    // eg. textures default to white, matrices to identity, floats to 1
    // in case a uniform isn't set on an object
    if (uniformData == nullptr)
        return;

    m_uniformData[uniformName] = uniformData;

}

void* WorldObject::getUniformData(char* uniformName) {
    try {
        return m_uniformData.at(uniformName);
    }
    catch (const std::out_of_range* oorerr) {
        // If it doesn't exist, return nullptr - will need handling on the other end
        return nullptr;
    }
}