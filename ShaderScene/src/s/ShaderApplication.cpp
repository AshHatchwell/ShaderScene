#include "ShaderApplication.h"
#include "AppGlobals.h"

#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include <time.h>
#include <iterator>
#include <vector>
#include <iostream>

#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "WorldObject.h"

using namespace AppGlobals;

int ShaderApplication::update() {
    if (glfwWindowShouldClose(m_window) || glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        return APP_CLOSED;

    glm::mat4* bunnymat = m_testObject->getTransform();
    *bunnymat = glm::rotate(*bunnymat, glm::radians(20.f) * deltaTime, glm::vec3(0.f, 1.f, 0.f));
    
    m_mainCamera->update();

    return APP_NO_ERROR;
}

int ShaderApplication::draw() {
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawObject(m_testObject);

    return APP_NO_ERROR;
}

void ShaderApplication::drawMesh(Mesh* mesh, ShaderProgram* program, const glm::mat4& transform) {
    // TODO: Multiple shape support, only renders first shape currently

    const MeshData* meshData = mesh->getMeshData(0);
    const std::vector<Vertex>* vertexData = mesh->getVertexData(0);
    const std::vector<unsigned int>* indexData = mesh->getIndexData(0);

    // Bind the VAO
    glBindVertexArray(meshData->VAO);

    // Bind the program (do this elsewhere for batches of meshes/same programs?)
    program->bind();

    // Assign the projection view uniform
    // TODO: We need a way to figure out what uniforms need assigning
    // Maybe do this elsewhere?
    int projectionViewUniform = program->getUniformHandle("ProjectionView");
    glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(*m_mainCamera->getProjectionView() * transform));

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, meshData->indexCount, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

void ShaderApplication::drawObject(WorldObject* objectToDraw) {
    // Get the mesh, program, and transform
    Mesh* mesh = objectToDraw->getMesh();
    ShaderProgram* shaderProgram = objectToDraw->getShaderProgram();
    glm::mat4* transform = objectToDraw->getTransform();

    setupUniforms(objectToDraw);

    shaderProgram->bind();

    // Get the mesh data
    const MeshData* meshData = mesh->getMeshData(0);
    const std::vector<Vertex>* vertexData = mesh->getVertexData(0);
    const std::vector<unsigned int>* indexData = mesh->getIndexData(0);

    glBindVertexArray(meshData->VAO);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, meshData->indexCount, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

}

void ShaderApplication::setupUniforms(WorldObject* object) {
    ShaderProgram* shaderProgram = object->getShaderProgram();
    const std::map<char*, UniformType>* shaderUniforms = shaderProgram->getUniformMap();

    // Standard ProjectionView uniform
    int PV = shaderProgram->getUniformHandle("ProjectionViewMatrix");
    int M = shaderProgram->getUniformHandle("ModelMatrix");
    int P = shaderProgram->getUniformHandle("ViewMatrix");
    if (PV != -1) {
        glUniformMatrix4fv(PV, 1, false, glm::value_ptr(*m_mainCamera->getProjectionView()));
        glUniformMatrix4fv(M, 1, false, glm::value_ptr(*object->getTransform()));
        glUniformMatrix4fv(P, 1, false, glm::value_ptr(*m_mainCamera->getProjection()));
    }


    // Iterate through uniforms in the shader
    for (auto& u : *shaderUniforms) {
        // Get uniform handle and type
        int uhandle = shaderProgram->getUniformHandle(u.first);
        UniformType utype = u.second;

        // typecasting <3
        switch (utype) {
        case UNIFORM_MAT4:
        {
            glm::mat4* umatrix4 = (glm::mat4*)object->getUniformData(u.first);
            // Default if undefined
            if (umatrix4 == nullptr) {
                glm::mat4 defaultMat = glm::mat4(1.f);
                glUniformMatrix4fv(uhandle, 1, false, glm::value_ptr(defaultMat));
            }
            else {
                glUniformMatrix4fv(uhandle, 1, false, glm::value_ptr(*umatrix4));
            }
        }
            break;

        case UNIFORM_FLOAT:
        {
            float* ufloat = (float*)object->getUniformData(u.first);
            // Default if undefined
            if (ufloat == nullptr) {
                float defaultFloat = 1.f;
                glUniform1f(uhandle, defaultFloat);
            }
            else {
                glUniform1f(uhandle, *ufloat);
            }
        }
            break;

        case UNIFORM_VEC4:
        {
            glm::vec4* uvector = (glm::vec4*)object->getUniformData(u.first);
            // Default if undefined
            if (uvector == nullptr) {
                glm::vec4 defaultVector = glm::vec4(1.f);
                glUniform4fv(uhandle, 1, glm::value_ptr(defaultVector));
            }
            else {
                glUniform4fv(uhandle, 1, glm::value_ptr(*uvector));
            }
        }
            break;

        default:
            // This ideally shouldn't happen
            std::cout << "Invalid uniform type in draw call!" << std::endl;
            continue;
        }
    }
}

int ShaderApplication::initialize() {
    m_testObject = new WorldObject();
    m_testMesh = new Mesh();

    m_mainCamera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
    m_mainCamera->setTransform(glm::translate(glm::vec3(0.f, 1.f, 5.f)));

    m_lightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    m_lightPosition = glm::vec4(3.f, 3.f, -3.f, 1.f);

    loadShaders();

    // Load mesh after shaders so that we can see shader errors without waiting
    m_testMesh->loadMesh("./data/models/bunnynormals.obj");

    // Set up the test object
    m_testObject->setMesh(m_testMesh);
    m_testObject->setShaderProgram(m_shaderPrograms["baseProgram"]);
    m_testObject->setUniformData("LightPosition", UniformType::UNIFORM_VEC4, (void*)&m_lightPosition);
    m_testObject->setUniformData("LightColor", UniformType::UNIFORM_VEC4, (void*)&m_lightColor);
    m_testObject->setUniformData("CameraPos", UniformType::UNIFORM_VEC4, (void*)m_mainCamera->getPosition());
    //m_testObject->setUniformData("ModelMatrix", UniformType::UNIFORM_VEC4, (void*)m_testObject->getTransform());

    return APP_NO_ERROR;
}

int ShaderApplication::terminate() {
    delete m_mainCamera;
    delete m_testMesh;
    delete m_testObject;

    std::vector<ShaderProgram*> programs;
    for (auto& iter : m_shaderPrograms) {
        programs.push_back(iter.second);
    }
    m_shaderPrograms.clear();

    for (auto& iter : programs) {
        delete iter;
    }
    programs.clear();

    return APP_NO_ERROR;
}

void ShaderApplication::loadShaders() {
    ShaderProgram* baseProgram = new ShaderProgram();
    Shader* baseVert = new Shader();
    Shader* baseFrag = new Shader();

    int error = 0;

    error = baseVert->loadShader(GL_VERTEX_SHADER, "././data/shaders/base/base.vert");
    if (error != 0) {
        delete baseVert;
        delete baseFrag;
        delete baseProgram;
        return;
    }

    error = baseFrag->loadShader(GL_FRAGMENT_SHADER, "././data/shaders/base/base.frag");
    if (error != 0) {
        delete baseVert;
        delete baseFrag;
        delete baseProgram;
        return;
    }

    if (baseProgram->setShader(GL_VERTEX_SHADER, baseVert) == false) {
        delete baseVert;
        delete baseFrag;
        delete baseProgram;
        return;
    }
    if (baseProgram->setShader(GL_FRAGMENT_SHADER, baseFrag) == false) {
        delete baseVert;
        delete baseFrag;
        delete baseProgram;
        return;
    }

    if (baseProgram->link() == false) {
        delete baseVert;
        delete baseFrag;
        delete baseProgram;
        return;
    }

    // Projectionview will be included in all shaders as standard, to streamline uniform attaching
    //baseProgram->setUniform("ProjectionView", UniformType::UNIFORM_MAT4);
    baseProgram->setUniform("LightPosition", UniformType::UNIFORM_VEC4);
    baseProgram->setUniform("LightColor", UniformType::UNIFORM_VEC4);
    baseProgram->setUniform("CameraPos", UniformType::UNIFORM_VEC4);

    m_shaderPrograms["baseProgram"] = baseProgram;

}