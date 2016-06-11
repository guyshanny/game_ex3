#include "OpenMeshObject.h"

#include <OpenMesh/Core/IO/MeshIO.hh>

OpenMeshObject::OpenMeshObject(const char* vShaderFile,
	const char* fShaderFile,
	const glm::vec3 & position,
	const glm::vec4 & color,
	const char* file_name,
	const char* textureIMG = "") :
	Object(vShaderFile, fShaderFile, position, color, textureIMG),
	_meshFile(file_name)
{
}

void OpenMeshObject::init()
{
	Object::init();
	loadMesh();
	computeBoundingBox();

	//add normal attribute to the mesh and calculate it.
	_mesh.request_face_normals();
	_mesh.request_vertex_normals();
	_mesh.update_normals();

	// face vertex iterator: this iterator goes through all the vertices_rgb which belong to the face
	Mesh::FaceVertexIter fvIter;

	// go over all the faces
	for (Mesh::FaceIter fIter = _mesh.faces_begin(); fIter != _mesh.faces_end(); ++fIter)
	{
		Mesh::Point center(0, 0, 0);
		/* for each face - go over all the vertices_rgb that belong to it and compute their
		average position (center) of face */
		for (fvIter = _mesh.fv_iter(*fIter); fvIter.is_valid(); ++fvIter)
		{
			//sort out vertices_rgb
			Mesh::Point& p = _mesh.point(*fvIter);
			_vertices.push_back(glm::vec4(normalize(p[0], 0),
				normalize(p[1], 1),
				normalize(p[2], 2),
				1));

			//sort out colours
			for (int i = 0; i < 3; i++)
			{
				_verticesRgb.push_back(normalize_colour(p[i], i));
			}
			_verticesRgb.push_back(1.f);
			calculate_vertex_normal(*fvIter, *fIter);
		}
	}
	_mesh.release_face_normals();
	_mesh.release_vertex_normals();
}

// Load a mesh from the given input file
void OpenMeshObject::loadMesh()
{
	if (!OpenMesh::IO::read_mesh(_mesh, _meshFile))
	{
		// if we didn't make it, exit...
		std::cerr << "Error loading mesh, Aborting." << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**axis: 0 x, 1 y, 2 z ->normalises the value correctly based on proportional magnitudes of the axes. */
float OpenMeshObject::normalize(float value, int axis)
{
	return ((value - _lower[axis]) / (_diff[axis])) * 2 - 1;
}

float OpenMeshObject::normalize_colour(float value, int axis)
{
	return ((value - _lower[axis]) / (_diff[axis]));
}

void OpenMeshObject::calculate_vertex_normal(const Mesh::VertexHandle &vHandle, const Mesh::FaceHandle &fHandle)
{
	OpenMesh::VectorT<float, 3> meshNorm;
	meshNorm = _mesh.calc_vertex_normal(vHandle);
	_vertices.push_back(glm::vec4(meshNorm[0], meshNorm[1], meshNorm[2], 0));
}

/** This function computes the geometrical center and the axis aligned bounding box of the object.
* The bounding box is represented by the lower left and upper right corners. */
void OpenMeshObject::computeBoundingBox()
{
	// Vertex iterator is an iterator which goes over all the vertices_rgb of the mesh:
	Mesh::VertexIter vertexIter;
	// This is used to store the geometrical position of the vertices_rgb of the mesh:
	Mesh::Point p, center;
	const float fm = std::numeric_limits<float>::max();
	Mesh::Point lowerLeft(fm, fm, fm);
	Mesh::Point upperRight(-fm, -fm, -fm);
	vertexIter = _mesh.vertices_begin();
	lowerLeft = upperRight = _mesh.point(*vertexIter);
	// This is how to go over all the vertices_rgb in the mesh:

	for (vertexIter = _mesh.vertices_begin(); vertexIter != _mesh.vertices_end(); ++vertexIter)
	{
		// This is how to get the extrema associated with the set of vertices_rgb:
		p = _mesh.point(*vertexIter);
		center += p;
		for (int i = 0; i < 3; i++)
		{
			lowerLeft[i] = std::min(lowerLeft[i], p[i]);
			upperRight[i] = std::max(upperRight[i], p[i]);
		}
	}

	//calculate the differences and the mid points.
	for (int i = 0; i<3; i++)
	{
		_lower[i] = lowerLeft[i];
		_higher[i] = upperRight[i];
		_diff[i] = upperRight[i] - lowerLeft[i];
		_midPoint[i] = (lowerLeft[i] + upperRight[i]) / 2;
	}
	//calculate the max difference (i.e the 'largest' axis)
	_maxDiff = std::max(_diff[0], _diff[1]);
	_maxDiff = std::max(_maxDiff, _diff[2]);
}

OpenMeshObject::~OpenMeshObject()
{
}
