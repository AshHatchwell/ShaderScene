#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <vector>

// Mesh.h
// Loads .obj files using tinyobjloader, and parses
// the data into a usable form.

struct Vertex {
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
};

struct MeshData {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
	unsigned int vertexCount;
	unsigned int indexCount;
	unsigned int textureIndex;
};

class Mesh {
public:
	static Mesh* createRectPrism(float width, float height, float depth, int textureID = -1);

	Mesh() {}
	virtual ~Mesh();
	bool loadMesh(const char* path, const std::vector<unsigned int>* textures = nullptr);

	const MeshData* getMeshData(unsigned int shape) { return shape > m_meshData.size() ? nullptr : &m_meshData[shape]; }
	const std::vector<Vertex>* getVertexData(unsigned int shape) { return shape > m_meshVertexData.size() ? nullptr : &m_meshVertexData[shape]; }
	const std::vector<unsigned int>* getIndexData(unsigned int shape) { return shape > m_meshIndexData.size() ? nullptr : &m_meshIndexData[shape]; }

protected:

	std::vector<MeshData> m_meshData;
	std::vector<std::vector<Vertex>> m_meshVertexData;
	std::vector<std::vector<unsigned int>> m_meshIndexData;

	void generateOpenGLBuffers(const std::vector<unsigned int>* textures = nullptr);
	
};