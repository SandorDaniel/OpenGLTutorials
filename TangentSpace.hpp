#pragma once

#include <vector>

#include <GL/glew.h>



// assumption: the previous compression (aka indexing) took different normals into account (for example a vertex position of a cube occurs at least three times in compressed_vertices)
void computeTangentBasis(
	const std::vector<glm::vec3>& compressed_vertices,
	const std::vector<glm::vec2>& compressed_uvs,
	const std::vector<GLushort>& inds,
	std::vector<glm::vec3>& compressed_tangents,
	std::vector<glm::vec3>& compressed_bitangents);