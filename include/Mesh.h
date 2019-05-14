#ifndef MESH_H
#define MESH_H

#include "OpenEXR/ImathVec.h"
#include <vector>


using std::vector;
using namespace Imath;



template<class T>
class Mesh
{

public:
	Mesh() {}
	~Mesh()	{}

	vector<Vec3<T>> vertices;
	// someType faces;

	void setRandomVerts( unsigned int count, unsigned int seed )
	{
		srand( seed );
		vertices.resize( count );
		for ( auto &v : vertices)
		{
			v[0] = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
			v[1] = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
			v[2] = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
		}
	};
};




#endif  //MESH_H
