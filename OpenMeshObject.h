#pragma once

#include "Object.h"
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <glm\glm.hpp>

typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh;

class OpenMeshObject : public Object
{
protected:
	float _diff[3];
	float _maxDiff;
	float _midPoint[3], _lower[3], _higher[3];
	std::vector<float> _verticesRgb;
	const char* _meshFile;
	Mesh _mesh;

	// Load a mesh from the given input file
	bool loadMesh();

	float normalize(float value, int axis);
	float normalize_colour(float value, int axis);
	void OpenMeshObject::computeBoundingBox();
	void calculate_vertex_normal(const Mesh::VertexHandle &vHandle, const Mesh::FaceHandle &fHandle);

public:
	OpenMeshObject(const char* vShaderFile,
		const char* fShaderFile,
		const glm::vec3 & position,
		const glm::vec4 & color,
		const char* file_name,
		const char* textureIMG);

	virtual void init();

	virtual ~OpenMeshObject();
};

