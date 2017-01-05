#pragma once
#include "MainApplication.h"
#include <map>
#include <glm\mat4x4.hpp>
#include <glm\vec4.hpp>

// ShaderApplication.h
// Application for loading meshes and applying shaders

class Mesh;
class Camera;
class ShaderProgram;
class WorldObject;

class ShaderApplication : public MainApplication {
public:

    ShaderApplication() : MainApplication() {}
    virtual ~ShaderApplication() {}

protected:
    virtual int update();
    virtual int draw();

    virtual int initialize();
    virtual int terminate();

    void loadShaders();

    // TODO: Move drawing into a seperate class?
    void drawMesh(Mesh* mesh, ShaderProgram* program, const glm::mat4& transform);

    void drawObject(WorldObject* objectToDraw);
    void setupUniforms(WorldObject* object);

private:

    // Test objects
    WorldObject*    m_testObject;
    Mesh*           m_testMesh;
    Camera*         m_mainCamera;
    glm::vec4       m_lightPosition;
    glm::vec4       m_lightColor;

    std::map<char*, ShaderProgram*> m_shaderPrograms;
};