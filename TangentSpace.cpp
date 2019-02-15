#include <glm/glm.hpp>

#include "TangentSpace.hpp"

#include <GL/glew.h>



void computeTangentBasis(
	const std::vector<glm::vec3>& compressed_vertices,
	const std::vector<glm::vec2>& compressed_uvs,
	const std::vector<GLushort>& inds,
	std::vector<glm::vec3>& compressed_tangents,
	std::vector<glm::vec3>& compressed_bitangents)
{
	compressed_tangents.resize(compressed_vertices.size());
    compressed_bitangents.resize(compressed_vertices.size());

	for (size_t i = 0; i < inds.size(); i += 3)
	{
		// Edges of the triangle : postion delta
		const glm::vec3 deltaPos1 = compressed_vertices[inds[i + 1]] - compressed_vertices[inds[i + 0]];
		const glm::vec3 deltaPos2 = compressed_vertices[inds[i + 2]] - compressed_vertices[inds[i + 0]];

		// UV delta
		const glm::vec2 deltaUV1 = compressed_uvs[inds[i + 1]] - compressed_uvs[inds[i + 0]];
		const glm::vec2 deltaUV2 = compressed_uvs[inds[i + 2]] - compressed_uvs[inds[i + 0]];

		// Solving equation system with Cramer's rule
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		const glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		const glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		// Set the same tangent for all three vertices of the triangle.
		// They will be merged later, in vboindexer.cpp
		compressed_tangents[inds[i + 0]] = tangent;
		compressed_tangents[inds[i + 1]] = tangent;
		compressed_tangents[inds[i + 2]] = tangent;

		// Same thing for binormals
		compressed_bitangents[inds[i + 0]] = bitangent;
		compressed_bitangents[inds[i + 1]] = bitangent;
		compressed_bitangents[inds[i + 2]] = bitangent;
	}
}