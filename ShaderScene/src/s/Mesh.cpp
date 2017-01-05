#include "Mesh.h"

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <assert.h>

#include <tiny_obj_loader.h>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

Mesh::~Mesh() {

}

Mesh* Mesh::createRectPrism(float width, float height, float depth, int textureID) {
	return nullptr;
}

// Loads in texture UVs, but we don't have textures yet
// Textures will be loaded in per shape
bool Mesh::loadMesh(const char* path, const std::vector<unsigned int>* textures) {
	// Load in the mesh
	std::vector<tinyobj::shape_t> meshShapes;
	std::vector<tinyobj::material_t> meshMaterials;
	std::string meshError;

	bool success = tinyobj::LoadObj(meshShapes, meshMaterials, meshError, path);
	// Make it crash yay
    if (!success)
        return false;

	// Resize our data vectors
	m_meshVertexData.resize(meshShapes.size());

	//Get the vertex data.
	for (unsigned int s = 0; s < meshShapes.size(); ++s) {
		//Resize the shape's Vertex vector.
		unsigned int vec3Total = meshShapes[s].mesh.positions.size() / 3;
		m_meshVertexData[s].resize(vec3Total);

		//Assign the position data.
		for (unsigned int i = 0; i < vec3Total; ++i) {
			m_meshVertexData[s][i].position = vec4(meshShapes[s].mesh.positions[i * 3], meshShapes[s].mesh.positions[i * 3 + 1], meshShapes[s].mesh.positions[i * 3 + 2], 1);
		}

		//Check for texcoords
		if (meshShapes[s].mesh.texcoords.size() > 0) {
			for (unsigned int i = 0; i < vec3Total; ++i) {
				m_meshVertexData[s][i].texCoord = vec2(meshShapes[s].mesh.texcoords[i * 2], meshShapes[s].mesh.texcoords[i * 2 + 1]);
			}
		}

		//Check for normals
		if (meshShapes[s].mesh.normals.size() > 0) {
			for (unsigned int i = 0; i < vec3Total; ++i) {
				m_meshVertexData[s][i].normal = vec4(meshShapes[s].mesh.normals[i * 3], meshShapes[s].mesh.normals[i * 3 + 1], meshShapes[s].mesh.normals[i * 3 + 2], 1);
			}
		}

		//Set Colour
		for (unsigned int i = 0; i < vec3Total; ++i) {
			//float col = sinf((float)i / m_meshVertexData[s].size());
			//float col = (float)i / m_meshVertexData[s].size();
            float col = 1.f;
			m_meshVertexData[s][i].color = vec4(col, col, col, 1.f);
		}
	}

	//Get the index data.
	m_meshIndexData.resize(meshShapes.size());
	for (unsigned int s = 0; s < meshShapes.size(); ++s) {
		m_meshIndexData[s].resize(meshShapes[s].mesh.indices.size());

		//Assign the index data.
		for (unsigned int i = 0; i < m_meshIndexData[s].size(); ++i) {
			m_meshIndexData[s][i] = meshShapes[s].mesh.indices[i];
		}
	}

	generateOpenGLBuffers(textures);
	return true;
}

void Mesh::generateOpenGLBuffers(const std::vector<unsigned int>* textures) {
	// Delete old buffers if they already exist
	if (m_meshData.size() > 0) {
		for (unsigned int s = 0; s < m_meshData.size(); ++s) {
			glDeleteVertexArrays(1, &m_meshData[s].VAO);
			glDeleteBuffers(1, &m_meshData[s].VBO);
			glDeleteBuffers(1, &m_meshData[s].IBO);
		}
		m_meshData.clear();
	}

	// Make a new set of buffers
	m_meshData.resize(m_meshVertexData.size());
	for (unsigned int s = 0; s < m_meshVertexData.size(); ++s) {
		
		m_meshData[s].vertexCount = m_meshVertexData[s].size();
		m_meshData[s].indexCount = m_meshIndexData[s].size();
		if (textures != nullptr) {
			if (s < textures->size())
				m_meshData[s].textureIndex = textures->at(s);
		}
		
		glGenVertexArrays(1, &m_meshData[s].VAO);
		// Start modifying the VAO
		glBindVertexArray(m_meshData[s].VAO);
		
		// Generate and fill buffers
		glGenBuffers(1, &m_meshData[s].VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshData[s].VBO);
		glBufferData(GL_ARRAY_BUFFER, m_meshData[s].vertexCount * sizeof(Vertex), &m_meshVertexData[s][0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_meshData[s].IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshData[s].IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_meshData[s].indexCount * sizeof(unsigned int), &m_meshIndexData[s][0], GL_STATIC_DRAW);

		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		// Colour
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

        // Normal
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) + sizeof(vec4)));

		glBindVertexArray(0);
	}
}